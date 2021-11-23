Texture2D diffuse_texture : register(t0);
SamplerState decal_sampler : register(s0);

struct VSInput
{
	float3 position : POSITION;
	float4 color	: COLOR;
	float2 texcoord	: TEXCOORD;
};

struct PSInput
{
	float4 position : SV_POSITION;
	float4 color	: COLOR;
	float2 texcoord : TEXCOORD;
};