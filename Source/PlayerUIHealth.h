#pragma once
#include "UI.h"

class UINodeBase;

//*************************************
// 
// �v���C���[HP�N���X
// 
//*************************************
class PlayerHealthUI : public UI, public Component
{
public:
	// �R���X�g���N�^
	PlayerHealthUI(){}

	// �f�X�g���N�^
	~PlayerHealthUI() override {}
	
	// ���O�擾
	std::string GetName() const override { return "PlayerHealthUI"; }
	
	// �J�n����
	void Start() override;
};