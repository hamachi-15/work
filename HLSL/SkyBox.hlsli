//---------------------------
// �e�N�X�`��
//---------------------------
Texture2D diffuse_texture : register(t0);
SamplerState decal_sampler : register(s0);

//---------------------------
// �O���[�o���ϐ�
//---------------------------
cbuffer CBPerFrame : register(b0)
{
	matrix inverse_view;
	matrix inverse_projection;
	float4 eye_pos;
};

//---------------------------
//	�f�[�^�t�H�[��
//---------------------------
struct VSInput
{
	float3 position : POSITION;
	float4 color	: COLOR;
	float2 texcoord	: TEXCOORD;
	float3 normal	: NORMAL;
};

struct PSInput
{
	float4 position : SV_POSITION;
	float3 world_pos: POSITION;
	float2 texcoord : TEXCOORD;
	float4 color	: COLOR;
};

#include "Func.hlsli"