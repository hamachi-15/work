#pragma once
#include "Shader.h"

class FontShader : public Shader
{
public:
	// �R���X�g���N�^�E�f�X�g���N�^
	FontShader(ID3D11Device* device);
	~FontShader() override {}

	// ���O�擾
	const char* GetShaderName() const override { return "FontShader"; }

	// �`��J�n����
	void Begin(ID3D11DeviceContext* context);

	// �`��I������
	void End(ID3D11DeviceContext* context);

};