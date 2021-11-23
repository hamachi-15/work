#pragma once
#include "MenuBase.h"
#include "Sprite.h"
#include "SpriteShader.h"
#include "Texture.h"

//--------------------------------
// ����������
//--------------------------------
class OperateDictionaryMenu : public MenuBase
{
public:
	// �R���X�g���N�^�E�f�X�g���N�^
	OperateDictionaryMenu();
	~OperateDictionaryMenu()override {}

	// �X�V����
	void Update(float elapsed_time)override;

	// �`�揈��
	void Render(ID3D11DeviceContext* context) override;

private:
	std::unique_ptr<SpriteShader>	shader;
	std::unique_ptr<Sprite>			sprite;
	std::unique_ptr<Texture>		texture;
};