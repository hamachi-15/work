//--------------------------------------------
//	�e�N�X�`��
//--------------------------------------------

Texture2D diffuse_texture : register(t0);
TextureCube cube_texture:register(t1);

SamplerState decal_sampler : register(s0);

Texture2D normal_texture:register(t2);
SamplerState wrap_sampler : register(s2);

//--------------------------------------------
//	�O���[�o���ϐ�
//--------------------------------------------
//�g�̑����E����
static const float2 wave1 = { 0.005,0.001 };
static const float2 wave2 = { -0.005,0.016 };
//�g�̑傫��
static const float wave_size = 1.0f;
//�g�ɂ��h�炬
static const float fluctuation = 0.01f;

//--------------------------------------------
//	�O���[�o���ϐ�
//--------------------------------------------
cbuffer CbScene : register(b0)
{
	row_major float4x4	view_projection;
	float4x4			view_mat[6];	//�J�����s��
	float4x4			projection;	//���e�ϊ�
	float4				light_color;		//���C�g�̐F
	float4				light_direction;	//���C�g�̕���
	float4				ambient_color;		//����
	float4				eye_pos;			//�J�������W
	float				wave_time;
	float dummy1;
	float dummy2;
	float dummy3;
};
#define MAX_BONES 128
cbuffer CbMesh : register(b1)
{
	row_major float4x4	bone_transforms[MAX_BONES];
};

cbuffer CbSubset : register(b2)
{
	float4				material_color;
};


//--------------------------------------------
//	�f�[�^�[�t�H�[�}�b�g
//--------------------------------------------
struct VSInput
{
	float3 Position : POSITION;
	float3 Normal   : NORMAL;
	float2 Tex      : TEXCOORD;
	float4 Color    : COLOR;	//���_�J���[����
	float4 bone_weights		: WEIGHTS;
	uint4  bone_indices		: BONES;
};

//�o�[�e�b�N�X�V�F�[�_�[�o�͍\����
struct PSInput
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR0;
	float3 wNormal : TEXCOORD1;	 //���[���h�@��
	float3 wPosition : TEXCOORD2;//���[���h���W
	float2 Tex : TEXCOORD3;


	float3 vT : TEXCOORD4;
	float3 vB : TEXCOORD5;
	float3 vN : TEXCOORD6;
};

#include "Func.hlsli"