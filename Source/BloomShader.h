#pragma once
#include <memory>
#include "Shader.h"

class Texture;
class GaussianBlur;

class Bloom : public Shader
{
public:
	// �R���X�g���N�^
	Bloom(ID3D11Device* device);
	// �f�X�g���N�^
	~Bloom() override {}

	// ���O�擾
	const char* GetShaderName() const override { return "Bloom"; }

	// �`��J�n����
	void Begin(ID3D11DeviceContext* context)override;

	// �P�x���o����
	Texture* Render(ID3D11DeviceContext* context, RenderContext& render_context, Texture* texture = nullptr);

	// �`��I������
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