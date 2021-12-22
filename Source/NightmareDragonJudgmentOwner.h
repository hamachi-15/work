#pragma once
#include "JudgmentBase.h"

//*****************************************
// �W�����v�U���m�[�h�Ɉڍs�ł��邩����
//*****************************************
class JumpAttackJudgment : public JudgmentBase
{
public:
	JumpAttackJudgment(Enemy* enemy) :JudgmentBase(enemy) {};
	// ����
	bool Judgment();

};

//*****************************************
// �܍U���m�[�h�Ɉڍs�ł��邩����
//*****************************************
class ClawAttackJudgment : public JudgmentBase
{
public:
	ClawAttackJudgment(Enemy* enemy) :JudgmentBase(enemy) {};

	// ����
	bool Judgment();
};

//*****************************************
// ���݂��U���m�[�h�Ɉڍs�ł��邩����
//*****************************************
class BasicAttackJudgment : public JudgmentBase
{
public:
	BasicAttackJudgment(Enemy* enemy) :JudgmentBase(enemy) {};

	// ����
	bool Judgment();
};

//*****************************************
// �p�U���m�[�h�Ɉڍs�ł��邩����
//*****************************************
class HornAttackJudgment : public JudgmentBase
{
public:
	HornAttackJudgment(Enemy* enemy) :JudgmentBase(enemy) {};

	// ����
	bool Judgment();
};