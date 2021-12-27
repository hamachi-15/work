#pragma once
//#include <memory>
#include "Shader.h"

class PhongVarianceShadowMap : public Shader
{
	static  const		int		MaxBones = 128;
public:
	PhongVarianceShadowMap(ID3D11Device* device);
	~PhongVarianceShadowMap()override {}

	// 名前取得
	const char* GetShaderName() const override { return "PhongVarianceShadowMap"; }

	void Begin(ID3D11DeviceContext* context, RenderContext& render_context);

	void End(ID3D11DeviceContext* context);

private:
	struct CBScene
	{
		DirectX::XMFLOAT4X4 light_view_projection;
		DirectX::XMFLOAT4X4	view_projection;
		DirectX::XMFLOAT4	light_direction;
		DirectX::XMFLOAT4	eye_position;
		DirectX::XMFLOAT4	shadow_parameter;	//	xyz カラー、w バイアス
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer>			scene_constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11BlendState>		blend_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depth_stencil_state;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>		sampler_state;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>		sampler_shadow;

	//std::unique_ptr<VarianceShadowMap> shadow;
};
