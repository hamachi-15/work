#pragma once
#include "JudgmentBase.h"

//*****************************************
// ƒWƒƒƒ“ƒvUŒ‚ƒm[ƒh‚ÉˆÚs‚Å‚«‚é‚©”»’è
//*****************************************
class JumpAttackJudgment : public JudgmentBase
{
public:
	JumpAttackJudgment(Enemy* enemy) :JudgmentBase(enemy) {};
	// ”»’è
	bool Judgment();

};

//*****************************************
// ’ÜUŒ‚ƒm[ƒh‚ÉˆÚs‚Å‚«‚é‚©”»’è
//*****************************************
class ClawAttackJudgment : public JudgmentBase
{
public:
	ClawAttackJudgment(Enemy* enemy) :JudgmentBase(enemy) {};

	// ”»’è
	bool Judgment();
};

//*****************************************
// Šš‚İ‚Â‚«UŒ‚ƒm[ƒh‚ÉˆÚs‚Å‚«‚é‚©”»’è
//*****************************************
class BasicAttackJudgment : public JudgmentBase
{
public:
	BasicAttackJudgment(Enemy* enemy) :JudgmentBase(enemy) {};

	// ”»’è
	bool Judgment();
};

//*****************************************
// ŠpUŒ‚ƒm[ƒh‚ÉˆÚs‚Å‚«‚é‚©”»’è
//*****************************************
class HornAttackJudgment : public JudgmentBase
{
public:
	HornAttackJudgment(Enemy* enemy) :JudgmentBase(enemy) {};

	// ”»’è
	bool Judgment();
};