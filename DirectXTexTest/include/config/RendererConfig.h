//--------------------------------------------------------------
//! @file   RendererConfig.h
//! @brief  DirectX11レンダラーの初期化設定構造体
//! @author つきの
//--------------------------------------------------------------
#pragma once
#include <d3d11.h>   // DXGI_FORMAT を使うため
//--------------------------------------------------------------
//! @struct RendererConfig
//! @brief DirectX11レンダラーの初期化設定
//! @details ウィンドウサイズやフォーマット、VSyncなどの基本設定を保持する
//--------------------------------------------------------------
struct RendererConfig {
    UINT width = 1280;   //!< ウィンドウ幅（デフォルト1280）
    UINT height = 720;   //!< ウィンドウ高さ（デフォルト720）
    DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM; //!< カラーバッファのフォーマット
    bool vsync = true;   //!< 垂直同期の有効/無効
    UINT bufferCount = 2; //!< スワップチェインのバッファ数（通常2）
    UINT refreshRate = 60; //!< リフレッシュレート（Hz）
    bool windowed = true;  //!< ウィンドウモード（true=ウィンドウ、false=フルスクリーン）

    //--------------------------------------------------------------
    //! @brief デフォルト設定を返す
    //--------------------------------------------------------------
    static RendererConfig Default() {
        return RendererConfig();
    }

    //--------------------------------------------------------------
    //! @brief カスタム設定を生成するヘルパー
    //! @param w 幅
    //! @param h 高さ
    //! @param vsyncEnabled VSync有効/無効
    //--------------------------------------------------------------
    static RendererConfig Custom(UINT w, UINT h, bool vsyncEnabled) {
        RendererConfig cfg;
        cfg.width = w;
        cfg.height = h;
        cfg.vsync = vsyncEnabled;
        return cfg;
    }
};