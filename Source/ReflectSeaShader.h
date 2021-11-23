#pragma once
#include "Shader.h"

class ReflectSea : public Shader
{
	static  const		int		MaxBones = 128;
public:
	ReflectSea(ID3D11Device* device);
	~ReflectSea()override {}

	// 名前取得
	const char* GetShaderName() const override { return "ReflectSea"; }

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
		float				wave_time;
		float dummy1;
		float dummy2;
		float dummy3;
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
	
	std::unique_ptr<Texture>						sea_normal_map;
};