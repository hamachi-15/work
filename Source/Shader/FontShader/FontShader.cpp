#include "FontShader.h"

//----------------------------------
// �R���X�g���N�^
//----------------------------------
FontShader::FontShader(ID3D11Device* device)
{
	Create(device, "Shader\\Font_vs.cso", "Shader\\Font_ps.cso");
}

//----------------------------------
// �`��J�n����
//----------------------------------
void FontShader::Begin(ID3D11DeviceContext* context)
{
	Activate(context);
}

//----------------------------------
// �`��I������
//----------------------------------
void FontShader::End(ID3D11DeviceContext* context)
{
	InActivate(context);
}
