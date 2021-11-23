#pragma once
#include "JudgmentBase.h"

// BodyAttackNode�ɑJ�ڂł��邩
class BodyAttackJudgment : public JudgmentBase
{
public:
	BodyAttackJudgment(Enemy* enemy) : JudgmentBase(enemy) {}
	// ����
	bool Judgment() { return false; }
};

// JumpAttackNode�ɑJ�ڏo���邩
class JumpAttackJudgment : public JudgmentBase
{
public:
	JumpAttackJudgment(Enemy* enemy) : JudgmentBase(enemy) {}
	// ����
	bool Judgment();
};