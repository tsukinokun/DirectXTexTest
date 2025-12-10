//--------------------------------------------------------------
//! @file   Mesh.h
//! @brief  DirectX11用の汎用メッシュ構造体
//! @author つきの
//--------------------------------------------------------------
#pragma once
#include <d3d11.h>      // Direct3D11の基本インターフェース定義  
#include <wrl/client.h> // Microsoft::WRL::ComPtr（スマートポインタ）を使うためのヘッダ

//--------------------------------------------------------------
//! @struct Mesh
//! @brief 頂点バッファとインデックスバッファを保持する汎用メッシュ
//--------------------------------------------------------------
struct Mesh {
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer; //!< 頂点バッファ
    Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;  //!< インデックスバッファ
    UINT stride = 0;        //!< 頂点1つ分のサイズ（例: sizeof(Vertex)）
    UINT offset = 0;        //!< バッファ先頭からのオフセット
    UINT indexCount = 0;    //!< インデックス数
    D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST; //!< プリミティブタイプ

    //--------------------------------------------------------------
    //! @brief メッシュを描画する
    //! @param context デバイスコンテキスト
    //--------------------------------------------------------------
    void Draw(ID3D11DeviceContext* context) const {
        // 頂点バッファをセット
        context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

        // インデックスバッファをセット
        context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

        // プリミティブタイプをセット
        context->IASetPrimitiveTopology(topology);

        // 描画
        context->DrawIndexed(indexCount, 0, 0);
    }
};