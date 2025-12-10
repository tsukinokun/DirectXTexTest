//--------------------------------------------------------------
//! @file     TextureLoader.cpp
//! @brief    DirectXTexライブラリを使用してテクスチャ使用するユーティリティの実装
//! @author   つきの
//--------------------------------------------------------------
#include "TextureLoader.h"
#include <DirectXTex.h>		// DirectXTexライブラリのヘッダ
#include <stdexcept>		// std::runtime_errorのためのヘッダ
//--------------------------------------------------------------
//! @brief テクスチャを読み込み、シェーダーリソースビューを作成して返す
//--------------------------------------------------------------
Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> TextureLoader::LoadTexture(
	ID3D11Device* device, const std::wstring& filename) {
	// テクスチャファイルをDirectXTexで読み込む
	DirectX::ScratchImage image;
	if (FAILED(DirectX::LoadFromWICFile(filename.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, image))) {
		throw std::runtime_error("Failed to load texture");
	}
	// シェーダーリソースビューを作成して返す
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
	if (FAILED(DirectX::CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(),
		image.GetMetadata(), srv.GetAddressOf()))) {
		throw std::runtime_error("Failed to create SRV");
	}
	// 作成したSRVを返す
	return srv;
}