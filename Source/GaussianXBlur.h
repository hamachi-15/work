#pragma once
#include <memory>
#include"Shader.h"
#include "RenderContext.h"
#include "Texture.h"

class GaussianXBlur : public Shader
{
public:
	// �R���X�g���N�^
	GaussianXBlur(ID3D11Device* device, u_int buffer_width, u_int buffer_height, DXGI_FORMAT format = DXGI_FORMAT_R32G32_FLOAT);
	~GaussianXBlur()override {}

	// ���O�擾
	const char* GetShaderName() const override { return "GaussianXBlur"; }

	// �`��J�n����
	void Begin(ID3D11DeviceContext* context, BlurRenderContext& bulr_render_context)override;

	// �`��I������
	void End(ID3D11DeviceContext* context) override;

	Texture* GetBlurXTexture() const { return xblur_texture.get(); }
private:
	std::unique_ptr<Texture> xblur_texture;
	std::unique_ptr<Texture> depth_texture;
};