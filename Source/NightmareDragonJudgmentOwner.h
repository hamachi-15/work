#pragma once
#include "JudgmentBase.h"

//*****************************************
// ジャンプ攻撃ノードに移行できるか判定
//*****************************************
class JumpAttackJudgment : public JudgmentBase
{
public:
	JumpAttackJudgment(Enemy* enemy) :JudgmentBase(enemy) {};
	// 判定
	bool Judgment();

};

//*****************************************
// 爪攻撃ノードに移行できるか判定
//*****************************************
class ClawAttackJudgment : public JudgmentBase
{
public:
	ClawAttackJudgment(Enemy* enemy) :JudgmentBase(enemy) {};

	// 判定
	bool Judgment();
};

//*****************************************
// 噛みつき攻撃ノードに移行できるか判定
//*****************************************
class BasicAttackJudgment : public JudgmentBase
{
public:
	BasicAttackJudgment(Enemy* enemy) :JudgmentBase(enemy) {};

	// 判定
	bool Judgment();
};

//*****************************************
// 角攻撃ノードに移行できるか判定
//*****************************************
class HornAttackJudgment : public JudgmentBase
{
public:
	HornAttackJudgment(Enemy* enemy) :JudgmentBase(enemy) {};

	// 判定
	bool Judgment();
};