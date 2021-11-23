Texture2D    texture0 : register(t0);
SamplerState sampler0 : register(s0);

struct VSInput
{
	float4 position : POSITION;
	float2 texcoord : TEXCOORD;
	float4 color : COLOR;
};

struct PSInput
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
	float4 color    : COLOR;
};

cbuffer ConstantBuffer : register(b0)
{
	row_major float4x4	world_view_projection;
};
