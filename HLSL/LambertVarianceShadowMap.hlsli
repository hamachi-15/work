//------------------------
// �e�N�X�`��
//------------------------
Texture2D DiffuseTexture : register(t0);
SamplerState DecalSampler : register(s0);
Texture2D ShadowTexture : register(t3);
SamplerState ShadowSampler : register(s3);

//------------------------
// �f�[�^�t�H�[�}�b�g
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
	float3 wPosition	: TEXCOORD2; // ���[���h�@��
	float2 Texcoord		: TEXCOORD3;  // ���[���h���W
	float3 vShadow		: TEXCOORD4;
};

//------------------------
// �萔�o�b�t�@
//------------------------
cbuffer CbScene : register(b0)
{
	row_major float4x4	ViewProjection;
	row_major float4x4  LightViewProjection;
	float4				LightDirection;
	float4				LightColor;		//���C�g�̐F
	float4				AmbientColor;	//����
	float4				EyePos;			//�J�������W
	float4				ShadowColor;	// �e�̐F
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
