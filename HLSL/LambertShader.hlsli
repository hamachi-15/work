// テクスチャ・サンプラー
Texture2D diffusetexture : register(t0);
SamplerState decalsampler : register(s0);

// データ
//struct VSInput
//{
//	float4 position		: POSITION;
//	float3 normal		: NORMAL;
//	float3 tangent		: TANGENT;
//	float2 texcoord		: TEXCOORD;
//	float4 color		: COLOR;
//	float4 bone_weights	: WEIGHTS;
//	uint4  bone_indices : BONES;
//};
//
//struct PSInput
//{
//	float4 position : SV_POSITION;
//	float2 texcoord : TEXCOORD;
//	float4 color	: COLOR;
//};
//
//// 定数バッファ
//cbuffer CBScene : register(b0)
//{
//	row_major float4x4	view_projection;
//	float4				light_direction;
//};
//
//#define MAX_BONES 128
//cbuffer CBMesh : register(b1)
//{
//	row_major float4x4 bone_transforms[MAX_BONES];
//};
//
//cbuffer CBSubset : register(b2)
//{
//	float4 material_color;
//};

struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
	float4 color    : COLOR;
};

cbuffer CbScene : register(b0)
{
	row_major float4x4	viewProjection;
	float4				lightDirection;
};

#define MAX_BONES 128
cbuffer CbMesh : register(b1)
{
	row_major float4x4	boneTransforms[MAX_BONES];
};

cbuffer CbSubset : register(b2)
{
	float4				materialColor;
};