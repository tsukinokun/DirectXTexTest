//-------------------------------------------------------------
//! @file TextureVS.hlsl
//! @brief 頂点シェーダー（テクスチャマッピング用）
//! @author つきの
//-------------------------------------------------------------

// 頂点シェーダー入力
struct VSInput
{
    float3 position : POSITION; // 頂点位置 (x,y,z)
    float2 uv : TEXCOORD0; // テクスチャ座標 (u,v)
};

// ピクセルシェーダーへ渡す出力
struct PSInput
{
    float4 position : SV_POSITION; // スクリーン座標 (必須)
    float2 uv : TEXCOORD0; // テクスチャ座標をそのまま渡す
};

// 頂点シェーダー本体
PSInput main(VSInput input)
{
    PSInput output;
    // 3D座標を float4 に変換 (w=1.0)
    output.position = float4(input.position, 1.0f);

    // UV座標をそのままピクセルシェーダーへ渡す
    output.uv = input.uv;

    return output;
}