#pragma once
#include "UI.h"

class UINodeBase;

//*************************************
// 
// �G���GHP�N���X
// 
//*************************************
class NormalEnemyHealthUI : public UI, public Component
{
public:
	// �R���X�g���N�^
	NormalEnemyHealthUI() {}

	// �f�X�g���N�^
	~NormalEnemyHealthUI() override {}

	// ���O�擾
	std::string GetName() const override { return "NormalEnemyHealthUI"; }

	// �J�n����
	void Start() override;

private:
};