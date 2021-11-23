// テクスチャ・サンプラー
Texture2D diffuse_texture : register(t0);
SamplerState decal_sampler : register(s0);
Texture2D shadow_texture : register(t3);
SamplerState shadow_sampler : register(s3);

static const float Bius = 0.0001f;

// データフォーマット
struct VSInput
{
	float4 position		: POSITION;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float2 texcoord		: TEXCOORD;
	float4 color		: COLOR;
	float4 bone_weights	: WEIGHTS;
	uint4  bone_indices  : BONES;
};

struct PSInput
{
	float4 position		: SV_POSITION;
	float4 color		: COLOR0;
	float3 normal		: TEXCOORD1; //ワールド法線
	float3 wposition	: TEXCOORD2;//ワールド座標
	float2 texcoord		: TEXCOORD3;
	float3 shadow		: TEXCOORD4;
};

//定数バッファ
cbuffer CBScene : register(b0)
{
	float4x4			light_view_projection;
	row_major float4x4	view_projection;
	float4				light_direction;
	float4				eye_pos; //カメラ座標
	float4				shadow_parameter;
};

#define MAX_BONES 128
cbuffer CBMesh : register(b1)
{
	row_major float4x4	bone_transforms[MAX_BONES];
};

cbuffer CBSubset : register(b2)
{
	float4				material_color;
};

#include "Func.hlsli"