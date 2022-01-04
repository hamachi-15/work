#pragma once
#include <vector>
#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>

class DebugRenderer
{
public:
	DebugRenderer(ID3D11Device* device);
	~DebugRenderer(){}

public:
	// 描画処理
	void Render(ID3D11DeviceContext* context, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);
	
	// 立方体描画
	void DrawCube(const DirectX::XMFLOAT3& center, const DirectX::XMFLOAT3& radius, const DirectX::XMFLOAT4& color);
	
	// 球描画処理
	void DrawSphere(const DirectX::XMFLOAT3& center, float radius, const DirectX::XMFLOAT4& color);
	
	// 円柱描画処理
	void DrawCylinder(const DirectX::XMFLOAT3& position, float radius, float height, const DirectX::XMFLOAT4& color);

	void DrawLine(ID3D11DeviceContext* context, ID3D11Device* device, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end);
private:
	// 立方体メッシュ作成
	void CreateCubeMesh(ID3D11Device* device);

	// 球メッシュ作成
	void CreateSphereMesh(ID3D11Device* device, float radius, int slices, int stacks);
	// 円柱メッシュ作成
	void CreateCylinderMesh(ID3D11Device* device, float radius1, float radius2, float start, float height, int slices, int stacks);
private:
	struct VERTEX
	{
		DirectX::XMFLOAT3 Pos;		//位置
		DirectX::XMFLOAT3 Normal;	//法線
		DirectX::XMFLOAT2 Tex;		//UV座標
		DirectX::XMFLOAT4 Color;	//頂点色
	}; 
	struct CBMesh
	{
		DirectX::XMFLOAT4X4 world_view_projection;
		DirectX::XMFLOAT4	color;
	};
	struct Cube
	{
		DirectX::XMFLOAT3	position;
		DirectX::XMFLOAT3	radius;
		DirectX::XMFLOAT3	scale;
		DirectX::XMFLOAT3	angle;
		DirectX::XMFLOAT4	color;
	};
	struct Sphere
	{
		DirectX::XMFLOAT4	color;
		DirectX::XMFLOAT3	center;
		float				radius;
	};
	struct Cylinder
	{
		DirectX::XMFLOAT4	color;
		DirectX::XMFLOAT3	position;
		float				radius;
		float				height;
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer>			cylinder_index_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			cube_vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			sphere_vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			cylinder_vertex_buffer;

	Microsoft::WRL::ComPtr<ID3D11Buffer>			constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>		vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		input_layout;

	Microsoft::WRL::ComPtr<ID3D11BlendState>		blend_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depth_stencil_state;

	std::vector<Cube>								cubes;
	std::vector<Sphere>								spheres;
	std::vector<Cylinder>							cylinders;
	
	UINT											cube_vertex_count = 0;
	UINT											sphere_vertex_count = 0;
	UINT											cylinder_vertex_count = 0;
};

// 平面の定義
struct Plane
{
	DirectX::XMFLOAT3	normal;	// 法線
	float				direction;	// 原点からの最短距離
};
