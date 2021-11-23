// テクスチャ・サンプラー
Texture2D diffuse_texture : register(t0);
SamplerState decal_sampler : register(s0);

// 定数バッファ
cbuffer CbScene : register(b0)
{
	row_major float4x4	light_view_projection;
}
#define MAX_BONES 128
cbuffer CbMesh : register(b1)
{
	row_major float4x4	boneTransforms[MAX_BONES];
};

cbuffer CbSubset : register(b2)
{
	float4				materialColor;
};
// データフォーマット
struct VSInput
{
	float4 position     : POSITION;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float2 texcoord		: TEXCOORD;
	float4 color		: COLOR;
	float4 bone_weights	: WEIGHTS;
	uint4  bone_indices	: BONES;
};

struct PSInputShadow
{
	float4 position : SV_POSITION;
	float4 depth	: TEXCOORD1;
};