#pragma once
#include <DirectXMath.h>

//------------------------------
// レンダーコンテキスト
//------------------------------
struct RenderContext
{
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;
	DirectX::XMFLOAT4X4 light_view_projection[3];
	DirectX::XMFLOAT4X4 single_light_view_projection;
	DirectX::XMFLOAT4X4 view_projection;
	DirectX::XMFLOAT4	ambient_color;
	DirectX::XMFLOAT4	light_color;
	DirectX::XMFLOAT4	light_direction;
	DirectX::XMFLOAT4	ShadowParameter;	//	xyz カラー、w バイアス
};

struct PrimitiveContext
{
	float	timer;
	int		number;
};

struct CubeMapRenderContext
{

	DirectX::XMFLOAT4X4 view_matrix[6];
	DirectX::XMFLOAT4X4 cubemap_projection;
	DirectX::XMFLOAT3	camera_position;
};

struct BlurRenderContext
{
	int karnel_size;
	int sigma;
	DirectX::XMFLOAT2 texcel;
};