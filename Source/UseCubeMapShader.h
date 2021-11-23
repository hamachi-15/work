#pragma once
#include "Shader.h"

class UseCubeMap : public Shader
{
	static  const		int		MaxBones = 128;
public:
	UseCubeMap(ID3D11Device* device);
	~UseCubeMap(){}

	// 名前取得
	const char* GetShaderName() const override { return "UseCubeMap"; }

	void Begin(ID3D11DeviceContext* context, RenderContext& render_context);

	void Draw(ID3D11DeviceContext* context, const Model* model);

	void End(ID3D11DeviceContext* context);
	struct CBScene
	{
		DirectX::XMFLOAT4X4	view_projection;
		DirectX::XMFLOAT4	light_direction;
		DirectX::XMFLOAT4X4 projection;	//投影変換
		DirectX::XMFLOAT4	light_color;		//ライトの色
		DirectX::XMFLOAT4	ambient_color;
		DirectX::XMFLOAT4	eye_position;
	};
	struct CBMesh
	{
		DirectX::XMFLOAT4X4	bone_transforms[MaxBones];
	};
	struct CBSubset
	{
		DirectX::XMFLOAT4	material_color;
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer>			scene_constant_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			mesh_constant_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			subset_constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11BlendState>		blend_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depth_stencil_state;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>		sampler_state;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>		sampler_shadow;

};