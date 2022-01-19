#pragma once
#include <memory>
#include "Shader.h"

class Texture;
class GaussianBlur;

class Bloom : public Shader
{
public:
	// コンストラクタ
	Bloom(ID3D11Device* device);
	// デストラクタ
	~Bloom() override {}

	// 名前取得
	const char* GetShaderName() const override { return "Bloom"; }

	// 描画開始処理
	void Begin(ID3D11DeviceContext* context)override;

	// 輝度抽出処理
	Texture* Render(ID3D11DeviceContext* context, RenderContext& render_context, Texture* texture = nullptr);

	// 描画終了処理
	void End(ID3D11DeviceContext* context)override;


private:
	struct ConstantBufferForBloom
	{
		float threshold;
		float dummy1;
		float dummy2;
		float dummy3;
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11BlendState>		blend_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depth_stencil_state;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>		sampler_state;
	
	std::shared_ptr<GaussianBlur> bulr;
	
	std::unique_ptr<Texture> bright_texture;
	std::unique_ptr<Texture> bloom_texture;
	std::unique_ptr<Texture> depth_texture;
};