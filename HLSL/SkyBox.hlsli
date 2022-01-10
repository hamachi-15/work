#include "Func.hlsli"

//---------------------------
// テクスチャ
//---------------------------
Texture2D diffuse_texture : register(t0);
SamplerState decal_sampler : register(s0);

//---------------------------
// グローバル変数
//---------------------------
cbuffer CBPerFrame : register(b0)
{
	matrix inverse_view;
	matrix inverse_projection;
	float4 eye_pos;
};

//---------------------------
//	データフォーム
//---------------------------
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
	float3 world_pos: POSITION;
	float2 texcoord : TEXCOORD;
	float4 color	: COLOR;
};

//--------------------------------------------
//	視線ベクトルを正距円筒図法に則ったUV座標へ変換する
//--------------------------------------------
float2 EquirectangularProjection(float3 v)
{
	static const float PI = 3.141592654f;
	float2 tex;
	tex.x = 1.0f - ((1.0f / (2 * PI)) * atan2(v.z, v.x) + 0.5f);
	tex.y = -(1.0f / PI) * atan2(v.y, length(v.xz)) + 0.5f;

	return tex;
}