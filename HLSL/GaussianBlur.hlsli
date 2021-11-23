// テクスチャ
Texture2D diffuse_texture : register(t0);
SamplerState decal_sampler : register(s0);


cbuffer CBPerBuffer : register(b0)
{
	float4 weight[2];
};

//--------------------------------------------
//	データーフォーマット
//--------------------------------------------
struct VSInput
{
	float3 position : POSITION;
	float4 color    : COLOR;
	float2 tex      : TEXCOORD;
	float3 normal   : NORMAL;
};

struct PSInput
{
	float4 position : SV_POSITION;
	float4 color    : COLOR;
	float4 tex0		: TEXCOORD0;
	float4 tex1		: TEXCOORD1;
	float4 tex2		: TEXCOORD2;
	float4 tex3		: TEXCOORD3;
	float4 tex4		: TEXCOORD4;
	float4 tex5		: TEXCOORD5;
	float4 tex6		: TEXCOORD6;
	float4 tex7		: TEXCOORD7;
};
