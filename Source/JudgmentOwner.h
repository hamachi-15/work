#pragma once
#include "JudgmentBase.h"

// BattleNodeに遷移できるか判定
class BattleJudgment : public JudgmentBase
{
public:
	BattleJudgment(Enemy* enemy) :JudgmentBase(enemy) {};
	// 判定
	bool Judgment();
};

// AttackNodeに遷移できるか判定
class AttackJudgment : public JudgmentBase
{
public:
	AttackJudgment(Enemy* enemy) :JudgmentBase(enemy) {};
	// 判定
	bool Judgment();
};

// WanderNodeに遷移できるか判定
class WanderJudgment : public JudgmentBase
{
public:
	WanderJudgment(Enemy* enemy) :JudgmentBase(enemy) {};
	// 判定
	bool Judgment();
};

// DamageNodeに遷移できるか判定
class DamageJudgment : public JudgmentBase
{
public:
	DamageJudgment(Enemy* enemy) :JudgmentBase(enemy) {};
	// 判定
	bool Judgment();
};

// DeathNodeに遷移できるか判定
class DeathJudgment : public JudgmentBase
{
public:
	DeathJudgment(Enemy* enemy) :JudgmentBase(enemy) {};
	// 判定
	bool Judgment();
};

// Escapeに遷移できるか判定
class EscapeJudgment : public JudgmentBase
{
public:
	EscapeJudgment(Enemy* enemy) :JudgmentBase(enemy) {};
	// 判定
	bool Judgment();
};