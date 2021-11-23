#pragma once
#include "JudgmentBase.h"

// JumpAttackNode‚É‘JˆÚ‚Å‚«‚é‚©”»’è
class JumpAttackJudgmenet : public JudgmentBase
{
public:
	JumpAttackJudgmenet(Enemy* enemy) : JudgmentBase(enemy) {}
	// ”»’è
	bool Judgment();
};

class PunchAttackJudgment : public JudgmentBase
{
public :
	PunchAttackJudgment(Enemy* enemy) : JudgmentBase(enemy) {}
	// ”»’è
	bool Judgment();
};

class SwipingAttackJudgmenet : public JudgmentBase
{
public:
	SwipingAttackJudgmenet(Enemy* enemy) : JudgmentBase(enemy) {}
	// ”»’è
	bool Judgment();
};