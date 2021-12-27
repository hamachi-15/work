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

	// 名前取得
	const char* GetShaderName() const override { return "VarianceShadowMap"; }

	// 描画開始処理
	void Begin(ID3D11DeviceContext* context, RenderContext& render_context);

	// 描画終了処理
	void End(ID3D11DeviceContext* context);

	Texture* GetShadowTexture() const { return shadow_texture.get(); }
	Texture* GetDepthTexture() const { return shadow_depth_texture.get(); }
private:
	struct CBScene
	{
		DirectX::XMFLOAT4X4	view_projection;
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer>			scene_constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>		sampler_state;

	std::unique_ptr<Texture> shadow_texture;
	std::unique_ptr<Texture> shadow_depth_texture;
};