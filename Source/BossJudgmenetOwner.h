#pragma once
#include "JudgmentBase.h"

// JumpAttackNode�ɑJ�ڂł��邩����
class JumpAttackJudgmenet : public JudgmentBase
{
public:
	JumpAttackJudgmenet(Enemy* enemy) : JudgmentBase(enemy) {}
	// ����
	bool Judgment();
};

class PunchAttackJudgment : public JudgmentBase
{
public :
	PunchAttackJudgment(Enemy* enemy) : JudgmentBase(enemy) {}
	// ����
	bool Judgment();
};

class SwipingAttackJudgmenet : public JudgmentBase
{
public:
	SwipingAttackJudgmenet(Enemy* enemy) : JudgmentBase(enemy) {}
	// ����
	bool Judgment();
};