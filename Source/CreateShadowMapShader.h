#pragma once
#include "Shader.h"

class CreateShadowMap : public Shader
{
public:
	// �R���X�g���N�^
	CreateShadowMap(ID3D11Device* device);
	// �f�X�g���N�^
	~CreateShadowMap() override{}

	// ���O�擾
	const char* GetShaderName() const override { return "CreateShadowMap"; }

	// �`��J�n����
	void Begin(ID3D11DeviceContext* context, RenderContext* render_context)override;

	// �`��I������
	void End(ID3D11DeviceContext* context)override;

private:
	struct CBScene
	{
		DirectX::XMFLOAT4X4	view_projection;
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer>			scene_constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>		sampler_state;

};