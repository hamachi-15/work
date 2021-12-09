#include "Func.hlsli"
// �e�N�X�`���@
Texture2D diffuse_texture : register(t0);
SamplerState decal_sampler : register(s0);

// �f�[�^�t�H�[�}�b�g
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
	float3 normal		: TEXCOORD1; //���[���h�@��
	float3 wposition	: TEXCOORD2;//���[���h���W
	float2 texcoord		: TEXCOORD3;
};

//�萔�o�b�t�@
cbuffer CBScene : register(b0)
{
	row_major float4x4	view_projection;
	float4				light_direction;
	float4				light_color;
	float4				eye_pos; //�J�������W
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
