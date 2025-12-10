//--------------------------------------------------------------
//! @file     TextureLoader.h
//! @brief    DirectXTexライブラリを使用してテクスチャ使用するユーティリティの定義
//! @author   つきの
//--------------------------------------------------------------
#pragma once
#include <string>
#include <wrl/client.h>     // Microsoft::WRL::ComPtr（スマートポインタ）を使うためのヘッダ
#include <d3d11.h>          // Direct3D11の基本インターフェース定義
//--------------------------------------------------------------
//! @class TextureLoader
//! @brief テクスチャローダークラス
//! @details DirectXTexライブラリを使用してテクスチャ使用するユーティリティクラス
//--------------------------------------------------------------
class TextureLoader {
public:
    //--------------------------------------------------------------
	//! @brief テクスチャを読み込み、シェーダーリソースビューを作成して返す
	//! @param device	[in] Direct3D11デバイス
	//! @param filename [in] テクスチャファイル名（パスを含む）
	//! @return シェーダーリソースビューのスマートポインタ
	//! @throw std::runtime_error 読み込みやSRV作成に失敗した場合
    //--------------------------------------------------------------
    static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> LoadTexture(
        ID3D11Device* device, const std::wstring& filename);
};