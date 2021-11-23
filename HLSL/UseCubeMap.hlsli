//--------------------------------------------
//	テクスチャ
//--------------------------------------------

Texture2D diffuse_texture : register(t0);
TextureCube cube_texture:register(t1);

SamplerState decal_sampler : register(s0);



//--------------------------------------------
//	グローバル変数
//--------------------------------------------
cbuffer CbScene : register(b0)
{
	row_major float4x4	view_projection;
	float4				light_color;		//ライトの色
	float4				light_direction;	//ライトの方向
	float4				ambient_color;		//環境光
	float4				eye_pos;			//カメラ座標
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
	float3 position	: POSITION;
	float3 normal	: NORMAL;
	float2 texcoord : TEXCOORD;
	float4 color	: COLOR;	//頂点カラー無し
	float4 bone_weights		: WEIGHTS;
	uint4  bone_indices		: BONES;
};



//バーテックスシェーダー出力構造体
struct PSInput
{
	float4 position	: SV_POSITION;
	float4 color	: COLOR0;
	float3 wnormal	: TEXCOORD1;	//ワールド法線
	float3 wposition: TEXCOORD2;	//ワールド座標
	float2 texcoord	: TEXCOORD3;
};
