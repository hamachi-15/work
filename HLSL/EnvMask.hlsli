//------------------------------
// テクスチャ
//------------------------------
Texture2D diffuse_texture : register(t0);
Texture2D mask_texture : register(t1);
SamplerState decal_sampler : register(s0);

//------------------------------
// グローバル変数
//------------------------------
cbuffer CBPerFrame :register(b0)
{
	float dissolve_threshold;
	float dummy1;
	float dummy2;
	float dummy3;
};

//------------------------------
// データフォーム
//------------------------------
struct VSInput
{
	float3 position : POSITION;
	float4 color	: COLOR;
	float2 texcoord	: TEXCOORD;
	float3 normal	: NORMAL;
};

struct PSInput
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
	float4 color    : COLOR;
};
