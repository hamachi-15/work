//--------------------------------------------
//	テクスチャ
//--------------------------------------------

Texture2D DiffuseTexture : register(t0);
SamplerState DecalSampler : register(s0);
Texture2D MaskTexture : register(t1);

//--------------------------------------------
//	グローバル変数
//--------------------------------------------
cbuffer CBuffer : register(b0)
{
	float dissolve_threshold;
}

//--------------------------------------------
//	データーフォーマット
//--------------------------------------------

struct VSInput
{
	float3 position : POSITION;
	float3 normal   : NORMAL;
	float2 texcoord      : TEXCOORD;
	float4 color    : COLOR;
};

struct PSInput
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
	float4 color : COLOR;
};
