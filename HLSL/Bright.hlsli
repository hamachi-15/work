// �e�N�X�`��
Texture2D diffuse_texture : register(t0);
SamplerState decal_sampler : register(s0);

// �O���[�o���ϐ�
cbuffer CBPerBloom : register(b3)
{
	float threshold;
	float dummy1;
	float dummy2;
	float dummy3;
};

// �f�[�^�t�H�[�}�b�g
struct VSInput
{
	float3 position : POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
};

struct PSInput
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
	float4 color : COLOR;
};