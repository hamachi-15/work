#pragma once
#include <memory>
#include "Component.h"
#include "Shader.h"
#include "RenderContext.h"
#include "Texture.h"
class VarianceShadowMap : public Shader
{
private:
	static	constexpr	u_int	ShadowBuffer = 1024;
	static  const		int		MaxBones = 128;
public:
	VarianceShadowMap(ID3D11Device* context);
	~VarianceShadowMap()override {};

	// –¼‘Oæ“¾
	const char* GetShaderName() const override { return "VarianceShadowMap"; }

	// •`‰æŠJnˆ—
	void Begin(ID3D11DeviceContext* context, RenderContext& render_context);

	// •`‰æˆ—
	void Draw(ID3D11DeviceContext* context, const Model* model);

	// •`‰æI—¹ˆ—
	void End(ID3D11DeviceContext* context);

	Texture* GetShadowTexture() const { return shadow_texture.get(); }
	Texture* GetDepthTexture() const { return shadow_depth_texture.get(); }
private:
	struct CBScene
	{
		DirectX::XMFLOAT4X4	view_projection;
	};
	struct CBMesh
	{
		DirectX::XMFLOAT4X4	boneTransforms[MaxBones];
	};
	struct CBSubset
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

	std::unique_ptr<Texture> shadow_texture;
	std::unique_ptr<Texture> shadow_depth_texture;
};