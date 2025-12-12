//------------------------------------------------------------------------------
//! @file Renderer.cpp
//! @brief DirectX11の描画管理クラスの実装
//! @author つきの
//------------------------------------------------------------------------------
#include "Renderer.h"           // Rendererクラスのヘッダをインクルード
#include "graphics/Vertex.h"    // 頂点データ構造体の定義をインクルード
#include <stdexcept>
#include <d3dcompiler.h> // D3DCompileFromFile 用
//------------------------------------------------------------------------------
//! @brief コンストラクタ：DirectX11デバイスとスワップチェインを初期化
//------------------------------------------------------------------------------
Renderer::Renderer(HWND hwnd, const RendererConfig& config) {
    // スワップチェインの設定
    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = config.bufferCount;              // バックバッファ数
    scd.BufferDesc.Width = config.width;               // 幅
    scd.BufferDesc.Height = config.height;             // 高さ
    scd.BufferDesc.Format = config.format;             // フォーマット
    scd.BufferDesc.RefreshRate.Numerator = config.refreshRate;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // バックバッファを描画ターゲットとして使用
    scd.OutputWindow = hwnd;                           // 出力先ウィンドウハンドル
    scd.SampleDesc.Count = 1;                          // マルチサンプリングなし
    scd.SampleDesc.Quality = 0;
    scd.Windowed = config.windowed ? TRUE : FALSE;     // ウィンドウモード/フルスクリーン

    // Direct3D11 デバイスとスワップチェインを生成
	UINT falgs = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
    if (FAILED(D3D11CreateDeviceAndSwapChain(
        nullptr,                       // アダプタ（nullptr = デフォルト GPU）
        D3D_DRIVER_TYPE_HARDWARE,      // ハードウェアドライバを使用
        nullptr,                       // ソフトウェアドライバなし
        falgs,                             // フラグなし
        nullptr, 0,                    // フィーチャーレベル指定なし（自動選択）
        D3D11_SDK_VERSION,             // SDK バージョン
        &scd,                          // スワップチェイン設定
        swapChain.GetAddressOf(),      // スワップチェインの出力
        device.GetAddressOf(),         // デバイスの出力
        nullptr,                       // 実際のフィーチャーレベルは不要
        context.GetAddressOf()))) {    // デバイスコンテキストの出力
        throw std::runtime_error("Failed to create device and swap chain");
    }

    // バックバッファを取得
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));

    // バックバッファからレンダーターゲットビューを作成
    device->CreateRenderTargetView(backBuffer.Get(), nullptr, renderTarget.GetAddressOf());

    // レンダーターゲットをバインド
    context->OMSetRenderTargets(1, renderTarget.GetAddressOf(), nullptr);
	
    // ビューポートを設定
    D3D11_VIEWPORT vp = {};
    vp.Width = static_cast<float>(config.width);
    vp.Height = static_cast<float>(config.height);
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    context->RSSetViewports(1, &vp);

    // -------------------------------
    // シェーダーのコンパイル
    // -------------------------------
    Microsoft::WRL::ComPtr<ID3DBlob> vsBlob;
    Microsoft::WRL::ComPtr<ID3DBlob> psBlob;
    Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

    if (FAILED(D3DCompileFromFile(
        L"shaders/TextureVS.hlsl", nullptr, nullptr,
        "main", "vs_5_0", 0, 0,
        &vsBlob, &errorBlob))) {
        if (errorBlob) OutputDebugStringA((char*)errorBlob->GetBufferPointer());
        throw std::runtime_error("Failed to compile vertex shader");
    }

    if (FAILED(D3DCompileFromFile(
        L"shaders/TexturePS.hlsl", nullptr, nullptr,
        "main", "ps_5_0", 0, 0,
        &psBlob, &errorBlob))) {
        if (errorBlob) OutputDebugStringA((char*)errorBlob->GetBufferPointer());
        throw std::runtime_error("Failed to compile pixel shader");
    }

    // -------------------------------
    // シェーダーの作成
    // -------------------------------
    device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, vertexShader.GetAddressOf());
    device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, pixelShader.GetAddressOf());

    // -------------------------------
    // 入力レイアウトの作成
    // -------------------------------
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    device->CreateInputLayout(
        layout,
        ARRAYSIZE(layout),
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        inputLayout.GetAddressOf()
    );
}
//------------------------------------------------------------------------------
//! @brief 画面を指定色でクリア
//------------------------------------------------------------------------------
void Renderer::Clear(float r, float g, float b, float a) {
	// クリアカラーを設定して画面をクリア
	float color[4] = { r, g, b, a };
	context->ClearRenderTargetView(renderTarget.Get(), color);
}

//------------------------------------------------------------------------------
//! @brief バッファを画面に表示
//------------------------------------------------------------------------------
void Renderer::Present() {
	// スワップチェインのバッファを画面に表示
	swapChain->Present(1, 0);
}

//--------------------------------------------------------------
//! @brief Direct3D11デバイスを取得
//--------------------------------------------------------------
ID3D11Device* Renderer::GetDevice() const {
	return device.Get();
}

//--------------------------------------------------------------
//! @brief Direct3D11デバイスコンテキストを取得
//--------------------------------------------------------------
ID3D11DeviceContext* Renderer::GetContext() const {
	return context.Get();
}

//--------------------------------------------------------------
//! @brief テクスチャ付き矩形を描画する
//--------------------------------------------------------------
void Renderer::DrawMesh(const Mesh& mesh, ID3D11ShaderResourceView* textureSRV) {
	// 入力レイアウトをセット
	context->IASetInputLayout(inputLayout.Get());
    
    // 頂点バッファをセット
    context->IASetVertexBuffers(0, 1, mesh.vertexBuffer.GetAddressOf(), &mesh.stride, &mesh.offset);

    // インデックスバッファをセット
    context->IASetIndexBuffer(mesh.indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

    // プリミティブタイプ（三角形リスト）
    context->IASetPrimitiveTopology(mesh.topology);

    // シェーダーをセット（事前にコンパイル＆作成済みのものを使用）
    context->VSSetShader(vertexShader.Get(), nullptr, 0);
    context->PSSetShader(pixelShader.Get(), nullptr, 0);

    // テクスチャをピクセルシェーダーにバインド
    context->PSSetShaderResources(0, 1, &textureSRV);

    // サンプラーステートをセット（必要なら）
    context->PSSetSamplers(0, 1, samplerState.GetAddressOf());

    // 描画
    context->DrawIndexed(mesh.indexCount, 0, 0);
}