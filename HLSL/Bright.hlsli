// テクスチャ
Texture2D diffuse_texture : register(t0);
SamplerState decal_sampler : register(s0);


// データフォーマット
struct VSInput
{
	float3 position : POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
};

struct PSInput
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
	float4 color : COLOR;
};