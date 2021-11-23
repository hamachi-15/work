//--------------------------------------------
//	�e�N�X�`��
//--------------------------------------------

Texture2D DiffuseTexture : register(t0);
SamplerState DecalSampler : register(s0);

Texture2DArray CubeTexture:register(t1);

//--------------------------------------------
//	�O���[�o���ϐ�
//--------------------------------------------
cbuffer CBPerFrame : register(b2)
{
	float4  LightColor;		//���C�g�̐F
	float4	LightDir;		//���C�g�̕���
	float4  AmbientColor;	//����
	float4	EyePos;			//�J�����ʒu
	float4x4 ViewMat[6];	//�J�����s��
	float	Id;
	float	dummy1;
	float	dummy2;
	float	dummy3;
};



//--------------------------------------------
//	�f�[�^�[�t�H�[�}�b�g
//--------------------------------------------

struct VSInput
{
	float3 Position : POSITION;
	float3 Normal   : NORMAL;
	float2 Tex      : TEXCOORD;
	float4 Color    : COLOR;
};

struct PSInput
{
	float4 Position : SV_POSITION;
	float2 Tex : TEXCOORD;
	float4 Color : COLOR;
};
