#pragma once
#include "JudgmentBase.h"

// BodyAttackNodeに遷移できるか
class BodyAttackJudgment : public JudgmentBase
{
public:
	BodyAttackJudgment(Enemy* enemy) : JudgmentBase(enemy) {}
	// 判定
	bool Judgment() { return false; }
};

// JumpAttackNodeに遷移出来るか
class HeavyBodyAttackJudgment : public JudgmentBase
{
public:
	HeavyBodyAttackJudgment(Enemy* enemy) : JudgmentBase(enemy) {}
	// 判定
	bool Judgment();
};