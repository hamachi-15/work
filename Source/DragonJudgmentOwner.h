#pragma once
#include "JudgmentBase.h"


//*****************************************
// 
// �܍U���m�[�h�Ɉڍs�ł��邩����
// 
//*****************************************
class ClawAttackJudgment : public JudgmentBase
{
public:
	// �R���X�g���N�^
	ClawAttackJudgment(Enemy* enemy) :JudgmentBase(enemy) {}

	// ����
	bool Judgment() override;
};

//*****************************************
// 
// ���݂��U���m�[�h�Ɉڍs�ł��邩����
// 
//*****************************************
class BasicAttackJudgment : public JudgmentBase
{
public:
	// �R���X�g���N�^
	BasicAttackJudgment(Enemy* enemy) :JudgmentBase(enemy) {}

	// ����
	bool Judgment() override;
};

//*****************************************
// 
// �p�U���m�[�h�Ɉڍs�ł��邩����
// 
//*****************************************
class HornAttackJudgment : public JudgmentBase
{
public:
	// �R���X�g���N�^
	HornAttackJudgment(Enemy* enemy) :JudgmentBase(enemy) {}

	// ����
	bool Judgment() override;
};

//*****************************************
// 
// �{�f�B�v���X�U���m�[�h�Ɉڍs�ł��邩����
// 
//*****************************************
class BodyPressAttackJudgment : public JudgmentBase
{
public:
	// �R���X�g���N�^
	BodyPressAttackJudgment(Enemy* enemy) : JudgmentBase(enemy) {}

	// ����
	bool Judgment() override;
};

//*****************************************
// 
// �����킹�m�[�h�Ɉڍs�ł��邩����
// 
//*****************************************
class TurnToTargetJudgment :public JudgmentBase
{
public:
	// �R���X�g���N�^
	TurnToTargetJudgment(Enemy* enemy) : JudgmentBase(enemy) {}

	//����
	bool Judgment() override;
};