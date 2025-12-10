//--------------------------------------------------------------
//! @file   main.cpp
//! @brief  DirectXTexチュートリアル用プロジェクトのエントリポイント
//! @author つきの
//--------------------------------------------------------------
#include "Window.h"          // ウィンドウ管理用クラスのヘッダをインクルード
#include "Renderer.h"        // DirectX11 レンダラークラスのヘッダをインクルード
#include "TextureLoader.h"   // テクスチャローダークラスのヘッダをインクルード
#include "graphics/MeshUtils.h"// メッシュ生成ユーティリティのヘッダをインクルード
//--------------------------------------------------------------
// Windows アプリケーションのエントリポイント
//! @param hInstance       現在のアプリケーションインスタンスハンドル
//! @param hPrevInstance   以前のインスタンス（常に nullptr）
//! @param lpCmdLine       コマンドライン引数
//! @param nCmdShow        ウィンドウ表示状態
//--------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance,       // 現在のアプリケーションインスタンスハンドル
	HINSTANCE hPrevInstance,   // 以前のインスタンス（常に nullptr）
	LPSTR lpCmdLine,           // コマンドライン引数（ANSI文字列）
	int nCmdShow)              // ウィンドウ表示状態（最大化/最小化など）
{
	// 幅800、高さ600のウィンドウを生成
	Window window(hInstance, 800, 600);

	// ウィンドウハンドルを渡してDirectX11Rendererを初期化
	Renderer renderer(window.GetHandle(), RendererConfig::Default());

	// 画像読み込み
	auto texture = TextureLoader::LoadTexture(renderer.GetDevice(), L"assets/sample.png");

	// メッシュ生成
	auto quadMesh = CreateQuadMesh(renderer.GetDevice());

	// メッセージループ
	while (window.ProcessMessages()) {
		//背景をRGBA(0.1, 0.2, 0.3, 1.0)でクリア
		renderer.Clear(0.1f, 0.2f, 0.3f, 1.0f);
		// SRV をピクセルシェーダーにセット
		renderer.GetContext()->PSSetShaderResources(0, 1, texture.GetAddressOf());
		// 四角形メッシュを描画
		quadMesh.Draw(renderer.GetContext());
		// バッファを画面に表示（スワップチェインのPresent）
		renderer.Present();
	}

	// 終了
	return 0;
}