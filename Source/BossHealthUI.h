#pragma once
#include "UI.h"

//**********************************
// 
// �{�X�N���X�̓GHPUI�N���X
// 
//**********************************
class BossHealthUI : public UI
{
public:
	// �R���X�g���N�^
	BossHealthUI() {}
	
	// �f�X�g���N�^
	~BossHealthUI() override {}

	// ���O�擾
	std::string GetName() const  { return "BossHealthUI"; }
	
	// �J�n����
	void Start() override;
};