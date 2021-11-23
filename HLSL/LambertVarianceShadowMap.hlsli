//------------------------
// テクスチャ
//------------------------
Texture2D DiffuseTexture : register(t0);
SamplerState DecalSampler : register(s0);
Texture2D ShadowTexture : register(t3);
SamplerState ShadowSampler : register(s3);

//------------------------
// データフォーマット
//------------------------
struct VSInput
{
	float3 Position		: POSITION;
	float3 Normal		: NORMAL;
	float2 Texcoord		: TEXCOORD;
	float4 Color		: COLOR;
	float4 BoneWeights	: WEIGHTS;
	uint4  BoneIndices	: BONES;
};

struct PSInput
{
	float4 Position		: SV_POSITION;
	float4 Color		: COLOR0;
	float3 wNormal		: TEXCOORD1;
	float3 wPosition	: TEXCOORD2; // ワールド法線
	float2 Texcoord		: TEXCOORD3;  // ワールド座標
	float3 vShadow		: TEXCOORD4;
};

//------------------------
// 定数バッファ
//------------------------
cbuffer CbScene : register(b0)
{
	row_major float4x4	ViewProjection;
	row_major float4x4  LightViewProjection;
	float4				LightDirection;
	float4				LightColor;		//ライトの色
	float4				AmbientColor;	//環境光
	float4				EyePos;			//カメラ座標
	float4				ShadowColor;	// 影の色
};

#define MAX_BONES 128
cbuffer CbMesh : register(b1)
{
	row_major float4x4	BoneTransforms[MAX_BONES];
};

cbuffer CbSubset : register(b2)
{
	float4				MaterialColor;
};
#include "Func.hlsli"
