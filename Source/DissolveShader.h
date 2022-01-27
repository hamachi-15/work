#pragma once
#include <memory>
#include "Shader.h"


//**********************************
// 
// �f�B�\�[�v�V�F�[�_�[
// 
//**********************************
class DissolveShader : public Shader
{
public:
	// �R���X�g���N�^
	DissolveShader(ID3D11Device* device);
	
	// �f�X�g���N�^
	~DissolveShader() override {}

	// ���O�擾
	const char* GetShaderName() const override { return "Dissolve"; }

	// �`��J�n����
	void Begin(ID3D11DeviceContext* context, float timer)override;

	// �`�揈��
	void Render(ID3D11DeviceContext* context, Texture* texture, Texture* mask_texture);

	// �`��I������
	void End(ID3D11DeviceContext* context)override;
private:
	struct ConstantBufferForDissolve
	{
		float threshold;
		float dummy1;
		float dummy2;
		float dummy3;
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer>	constant_buffer;

	//std::unique_ptr<Texture>				mask_texture;
};