#include "Func.hlsli"
// テクスチャ・サンプラー
Texture2D diffuse_texture : register(t0);
SamplerState decal_sampler : register(s0);

Texture2D shadow_texture1 : register(t3);
Texture2D shadow_texture2 : register(t4);
Texture2D shadow_texture3 : register(t5);

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
	row_major float4x4	light_view_projection[3];
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

//---------------------------------------------
// カスケードシャドウ算出関数
//---------------------------------------------
float3 GetCascadeShadow(float3 world_position)
{
	Texture2D shadow_texture[3] = { shadow_texture1, shadow_texture2, shadow_texture3 };

	for (int i = 0; i < 2; ++i)
	{
		float3 vshadow = GetShadowTex(light_view_projection[i], world_position);
		float3	shadow_texcoord = vshadow;
		if (shadow_texcoord.z >= 0 && shadow_texcoord.z <= 1 &&
			shadow_texcoord.x >= 0 && shadow_texcoord.x <= 1 &&
			shadow_texcoord.y >= 0 && shadow_texcoord.y <= 1)
		{
			return GetShadow(shadow_texture[i], shadow_sampler,
				vshadow, shadow_parameter);
		}
	}
	return 1;
}
//float3 GetCascadeShadow(float3 P)
//{
//	// シャドウマップ適用
//	Texture2D shadow_texture[] = { shadow_texture1, shadow_texture2, shadow_texture3 };
//
//	for (int j = 0; j < 3; ++j)
//	{
//		float3 vshadow = GetShadowTex(light_view_projection[i], world_position);
//		float3	shadow_texcoord = vshadow;
//		if (shadow_texcoord.z >= 0 && shadow_texcoord.z <= 1 &&
//			shadow_texcoord.x >= 0 && shadow_texcoord.x <= 1 &&
//			shadow_texcoord.y >= 0 && shadow_texcoord.y <= 1)
//		{
//			return GetShadow(shadow_texture[i], shadow_sampler,
//				vshadow, shadow_parameter);
//		}
//	}
//	return 1;
//}
