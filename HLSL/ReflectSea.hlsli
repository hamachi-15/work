//--------------------------------------------
//	テクスチャ
//--------------------------------------------

Texture2D diffuse_texture : register(t0);
TextureCube cube_texture:register(t1);

SamplerState decal_sampler : register(s0);

Texture2D normal_texture:register(t2);
SamplerState wrap_sampler : register(s2);

//--------------------------------------------
//	グローバル変数
//--------------------------------------------
//波の速さ・向き
static const float2 wave1 = { 0.005,0.001 };
static const float2 wave2 = { -0.005,0.016 };
//波の大きさ
static const float wave_size = 1.0f;
//波による揺らぎ
static const float fluctuation = 0.01f;

//--------------------------------------------
//	グローバル変数
//--------------------------------------------
cbuffer CbScene : register(b0)
{
	row_major float4x4	view_projection;
	float4x4			view_mat[6];	//カメラ行列
	float4x4			projection;	//投影変換
	float4				light_color;		//ライトの色
	float4				light_direction;	//ライトの方向
	float4				ambient_color;		//環境光
	float4				eye_pos;			//カメラ座標
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
//	データーフォーマット
//--------------------------------------------
struct VSInput
{
	float3 Position : POSITION;
	float3 Normal   : NORMAL;
	float2 Tex      : TEXCOORD;
	float4 Color    : COLOR;	//頂点カラー無し
	float4 bone_weights		: WEIGHTS;
	uint4  bone_indices		: BONES;
};

//バーテックスシェーダー出力構造体
struct PSInput
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR0;
	float3 wNormal : TEXCOORD1;	 //ワールド法線
	float3 wPosition : TEXCOORD2;//ワールド座標
	float2 Tex : TEXCOORD3;


	float3 vT : TEXCOORD4;
	float3 vB : TEXCOORD5;
	float3 vN : TEXCOORD6;
};

#include "Func.hlsli"