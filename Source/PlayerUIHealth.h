#pragma once
#include "UI.h"

class UINodeBase;

//------------------------------
// �v���C���[HP�N���X
//------------------------------
class PlayerHealthGage : public UI
{
public:
	// �R���X�g���N�^
	PlayerHealthGage(){}

	// �f�X�g���N�^
	~PlayerHealthGage();
	
	// �J�n����
	void Start() override;

private:
};