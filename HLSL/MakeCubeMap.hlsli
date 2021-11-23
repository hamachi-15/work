//-------------------------------------
// �e�N�X�`��
//-------------------------------------
Texture2D diffuse_texture : register(t0);
SamplerState decal_sampler : register(s0);

//--------------------------------------------
//	�O���[�o���ϐ�
//--------------------------------------------
cbuffer CbScene : register(b0)
{
	row_major float4x4	view_projection;
	row_major float4x4			view_mat[6];	//�J�����s��
	row_major float4x4			cubemap_projection; // �L���[�u�}�b�v�̃v���W�F�N�V����
	row_major float4x4			projection;	//���e�ϊ�
	float4				light_color;		//���C�g�̐F
	float4				light_direction;	//���C�g�̕���
	float4				ambient_color;		//����
	float4				eye_pos;			//�J�������W
	float				id;
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
	float3 position			: POSITION;
	float3 normal			: NORMAL;
	float2 texcoord			: TEXCOORD;
	float4 color			: COLOR;	//���_�J���[����
	float4 bone_weights		: WEIGHTS;
	uint4  bone_indices		: BONES;
};

//�W�I���g���V�F�[�_�[�o�͍\����
struct GSInput
{
	float4 position : SV_POSITION;
	float3 normal   : NORMAL;
	float2 texcoord : TEXCOORD;
	float4 color    : COLOR;	//���_�J���[����
};


//�o�[�e�b�N�X�V�F�[�_�[�o�͍\����
struct PSInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR0;
	float3 wnormal : TEXCOORD1;	//���[���h�@��
	float3 wposition : TEXCOORD2;	//���[���h���W
	float2 texcoord : TEXCOORD3;
	uint   RTindex:	SV_RenderTargetArrayIndex;
};

#include "Func.hlsli"