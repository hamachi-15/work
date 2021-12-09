//　テクスチャ
Texture2D diffuse_texture : register(t0);
SamplerState decal_sampler : register(s0);

cbuffer CbScene : register(b0)
{
	row_major float4x4	viewProjection;
	float4				lightDirection;
	float4				light_color;
};

#define MAX_BONES 128
cbuffer CbMesh : register(b1)
{
	row_major float4x4	boneTransforms[MAX_BONES];
};

cbuffer CbSubset : register(b2)
{
	float4				materialColor;
};
cbuffer CBPerFrame : register(b2)
{
	matrix	view;
	matrix	proj;
	matrix	prevView;
	matrix	prevProj;	//1F前の行列
	float	value;		//露出時間
	float	dummy1;
	float	dummy2;
	float	dummy3;
};
