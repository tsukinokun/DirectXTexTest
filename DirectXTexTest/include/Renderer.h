//--------------------------------------------------------------
//! @file Renderer.h
//! @brief DirectX11の描画管理クラスのヘッダ
//! @author つきの
//--------------------------------------------------------------
#pragma once
#include <d3d11.h>				   // Direct3D 11 の基本インターフェース定義
#include <wrl/client.h>			   // Microsoft::WRL::ComPtr（スマートポインタ）を使うためのヘッダ
#include <memory>		　		   // std::unique_ptrのためのヘッダ
#include "config/RendererConfig.h" // レンダラー初期化設定構造体の定義
#include "graphics/Mesh.h"		   // メッシュデータ構造体の定義
//--------------------------------------------------------------
//! @class Renderer
//! @brief DirectX11の描画管理クラス
//--------------------------------------------------------------
class Renderer {
public:
	//--------------------------------------------------------------
	// コンストラクタ
	//! @param hwnd [in] ウィンドウハンドル
	//! @param config [in] レンダラー初期化設定構造体
	//! @throw std::runtime_error デバイスやスワップチェインの生成に失敗した場合
	//--------------------------------------------------------------
	Renderer(HWND hwnd, const RendererConfig& config);

	//--------------------------------------------------------------
	// 画面を指定色でクリア
	//! @param r 赤成分（0.0〜1.0）
	//! @param g 緑成分（0.0〜1.0）
	//! @param b 青成分（0.0〜1.0）
	//! @param a アルファ成分（0.0〜1.0）
	//--------------------------------------------------------------
	void Clear(float r, float g, float b, float a);

	//--------------------------------------------------------------
	// バッファを画面に表示
	//--------------------------------------------------------------
	void Present();

	//--------------------------------------------------------------
	// Direct3D11デバイスを取得
	//! @return デバイスの生ポインタ
	//--------------------------------------------------------------
	ID3D11Device* GetDevice() const;

	//--------------------------------------------------------------
	// Direct3D11デバイスコンテキストを取得
	//! @return ID3D11DeviceContext* デバイスコンテキストの生ポインタ
	//--------------------------------------------------------------
	ID3D11DeviceContext* GetContext() const;

	//--------------------------------------------------------------
	// テクスチャ付き矩形を描画する 
	//--------------------------------------------------------------
	void DrawMesh(const Mesh& mesh, ID3D11ShaderResourceView* textureSRV);

private:
	Microsoft::WRL::ComPtr<ID3D11Device> device;                    // Direct3D11デバイス
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;            // Direct3D11デバイスコンテキスト
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;				// スワップチェイン
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTarget;	// レンダーターゲットビュー
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout; // 入力レイアウト
	// シェーダーとサンプラーステート
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;	// 頂点シェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;		// ピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;	// サンプラーステート
};