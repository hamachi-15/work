#pragma once
#include "Shader.h"

class Phong : public Shader
{
public:
	Phong(ID3D11Device* device);
	~Phong(){}

	// –¼‘OŽæ“¾
	const char* GetShaderName() const override { return "Phong"; }
	
	void Begin(ID3D11DeviceContext* context, RenderContext& render_context, DirectX::XMFLOAT4 light_color);

	void Draw(ID3D11DeviceContext* context, Model* model);

	void End(ID3D11DeviceContext* context)override;

private:
	static const int MaxBones = 128;
	struct CbScene
	{
		DirectX::XMFLOAT4X4	viewProjection;
		DirectX::XMFLOAT4	lightDirection;
		DirectX::XMFLOAT4	light_color;
		DirectX::XMFLOAT4	eye_position;
	};

	struct CbMesh
	{
		DirectX::XMFLOAT4X4	boneTransforms[MaxBones];
	};

	struct CbSubset
	{
		DirectX::XMFLOAT4	materialColor;
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer>			scene_constant_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			mesh_constant_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			subset_constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11BlendState>		blend_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depth_stencil_state;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>		sampler_state;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>		vertex_shader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader>	geometry_shader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		pixel_shader = nullptr;

	Microsoft::WRL::ComPtr<ID3D11InputLayout>		input_layout = nullptr;

};