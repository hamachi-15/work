//------------------------------
// テクスチャ
//------------------------------
Texture2D diffusetexture : register(t0);
SamplerState decalsampler : register(s0);

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
	float4 color	: COLOR;
	float2 texcoord : TEXCOORD;

};
struct VS_OUT
{
	float4 position : SV_POSITION;
	float4 color    : COLOR;
	float2 texcoord : TEXCOORD;
};
