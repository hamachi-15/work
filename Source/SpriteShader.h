#pragma once
#include <memory>
#include <wrl.h>
#include "Shader.h"

class SpriteShader : public Shader
{
public:
	SpriteShader(ID3D11Device* device);
	~SpriteShader() override {}

	// ���O�擾
	const char* GetShaderName() const override { return "SpriteShader"; }

	// ���_�V�F�[�_�[�擾
	ID3D11VertexShader* GetVertexShader() const { return vertex_shader.Get(); }
	
	// �s�N�Z���V�F�[�_�[�擾
	ID3D11PixelShader* GetPixelShader() const { return pixel_shader.Get(); }
	
	// �C���v�b�g���C�A�E�g�擾
	ID3D11InputLayout* GetInputLayout() const { return vertex_layout.Get(); }

	// �`��J�n����
	void Begin(ID3D11DeviceContext* context);

	// �`��I������
	void End(ID3D11DeviceContext* context);
private:

	//Microsoft::WRL::ComPtr<ID3D11Buffer>		vertex_buffer;

};