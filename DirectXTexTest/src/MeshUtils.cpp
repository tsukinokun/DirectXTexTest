//--------------------------------------------------------------
//! @file  MeshUtils.cpp
//! @brief メッシュ生成ユーティリティの実装
//! @authorつきの
//--------------------------------------------------------------
#include "graphics/MeshUtils.h"
#include "graphics/Vertex.h"
//--------------------------------------------------------------
//! @brief 四角形メッシュを生成
//--------------------------------------------------------------
Mesh CreateQuadMesh(ID3D11Device* device) {
	Mesh mesh;

	// 頂点データ（XY平面上の矩形）
	Vertex vertices[] = {
	{ DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) }, // 左下
	{ DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) }, // 右下
	{ DirectX::XMFLOAT3(0.5f,  0.5f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) }, // 右上
	{ DirectX::XMFLOAT3(-0.5f,  0.5f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) }, // 左上
	};

	UINT indices[] = { 0, 1, 2, 0, 2, 3 };

	// 頂点バッファ
	D3D11_BUFFER_DESC vbDesc = {};
	vbDesc.Usage = D3D11_USAGE_DEFAULT;
	vbDesc.ByteWidth = sizeof(vertices);
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vbData = {};
	vbData.pSysMem = vertices;
	device->CreateBuffer(&vbDesc, &vbData, mesh.vertexBuffer.GetAddressOf());

	// インデックスバッファ
	D3D11_BUFFER_DESC ibDesc = {};
	ibDesc.Usage = D3D11_USAGE_DEFAULT;
	ibDesc.ByteWidth = sizeof(indices);
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA ibData = {};
	ibData.pSysMem = indices;
	device->CreateBuffer(&ibDesc, &ibData, mesh.indexBuffer.GetAddressOf());

	// メッシュ情報
	mesh.stride = sizeof(Vertex);
	mesh.offset = 0;
	mesh.indexCount = _countof(indices);
	mesh.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	return mesh;
}