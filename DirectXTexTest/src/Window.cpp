//--------------------------------------------------------------
//! @file   Window.cpp
//! @brief  ウィンドウ管理用クラスの実装
//! @author つきの
//--------------------------------------------------------------
#include "Window.h"
//--------------------------------------------------------------
//! @brief ウィンドウプロシージャ
//--------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		// ウィンドウが破棄されたときの処理
	case WM_DESTROY:
		PostQuitMessage(0); // WM_QUIT を送る
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

//--------------------------------------------------------------
//! brief コンストラクタ：指定された幅と高さでウィンドウを生成する
//--------------------------------------------------------------
Window::Window(HINSTANCE hInstance, int width, int height) {
	// WNDCLASS 構造体を初期化（ゼロクリア）
	WNDCLASS wc = {};

	// ウィンドウプロシージャを指定（ここでは既定の DefWindowProc を使用）
	wc.lpfnWndProc = WindowProc;

	// アプリケーションインスタンスハンドルを設定
	wc.hInstance = hInstance;

	// ウィンドウクラス名を設定（この名前で後から CreateWindowEx に渡す）
	wc.lpszClassName = L"D3D11WindowClass";

	// ウィンドウクラスを登録（OS にこのクラスを認識させる）
	RegisterClass(&wc);

	// 実際にウィンドウを生成
	hwnd = CreateWindowEx(
		0,                      // 拡張スタイル（今回はなし）
		wc.lpszClassName,       // 登録したクラス名
		L"DirectXTex Tutorial",      // ウィンドウタイトル
		WS_OVERLAPPEDWINDOW,    // スタイル（通常のオーバーラップウィンドウ）
		CW_USEDEFAULT,          // X座標（デフォルト）
		CW_USEDEFAULT,          // Y座標（デフォルト）
		width,                  // 幅
		height,                 // 高さ
		nullptr,                // 親ウィンドウなし
		nullptr,                // メニューハンドルなし
		hInstance,              // インスタンスハンドル
		nullptr                 // 追加データなし
	);

	// ウィンドウを表示
	ShowWindow(hwnd, SW_SHOW);
}

//--------------------------------------------------------------
//! @brief ウィンドウハンドルを取得
//--------------------------------------------------------------
HWND Window::GetHandle() const {
	return hwnd;
}

//--------------------------------------------------------------
//! @brief Windows メッセージを処理する
//--------------------------------------------------------------
bool Window::ProcessMessages() {
	// メッセージ構造体
	MSG msg = {};
	// メッセージキューにある全てのメッセージを処理
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		// WM_QUITが来たらfalseを返して終了
		if (msg.message == WM_QUIT) return false;

		// キーボード入力などを変換
		TranslateMessage(&msg);

		// メッセージをウィンドウプロシージャに送る
		DispatchMessage(&msg);
	}
	// メッセージがなければtrueを返してループ継続
	return true;
}