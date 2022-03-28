#pragma once
#include "JudgmentBase.h"

// BodyAttackNode‚É‘JˆÚ‚Å‚«‚é‚©
class BodyAttackJudgment : public JudgmentBase
{
public:
	BodyAttackJudgment(Enemy* enemy) : JudgmentBase(enemy) {}
	// ”»’è
	bool Judgment() { return false; }
};

// JumpAttackNode‚É‘JˆÚo—ˆ‚é‚©
class HeavyBodyAttackJudgment : public JudgmentBase
{
public:
	HeavyBodyAttackJudgment(Enemy* enemy) : JudgmentBase(enemy) {}
	// ”»’è
	bool Judgment();
};