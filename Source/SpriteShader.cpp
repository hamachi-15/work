#include "SpriteShader.h"
//#include "Misc.h"

//----------------------------------
// �R���X�g���N�^
//----------------------------------
SpriteShader::SpriteShader(ID3D11Device* device)
{
	Create(device, "Shader\\Sprite_vs.cso", "Shader\\Sprite_ps.cso", true);
}

//----------------------------------
// �`��J�n����
//----------------------------------
void SpriteShader::Begin(ID3D11DeviceContext* context)
{
	Activate(context);
}

//----------------------------------
// �`��I������
//----------------------------------
void SpriteShader::End(ID3D11DeviceContext* context)
{
	InActivate(context);
}
