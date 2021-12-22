#pragma once
#include "JudgmentBase.h"

// BattleNodeに遷移できるか判定
class BattleJudgment : public JudgmentBase
{
public:
	// コンストラクタ
	BattleJudgment(Enemy* enemy) :JudgmentBase(enemy) {};
	// 判定
	bool Judgment();
};

// AttackNodeに遷移できるか判定
class AttackJudgment : public JudgmentBase
{
public:
	// コンストラクタ
	AttackJudgment(Enemy* enemy) :JudgmentBase(enemy) {};
	// 判定
	bool Judgment();
};

// WanderNodeに遷移できるか判定
class WanderJudgment : public JudgmentBase
{
public:
	// コンストラクタ
	WanderJudgment(Enemy* enemy) :JudgmentBase(enemy) {};
	// 判定
	bool Judgment();
};

// DamageNodeに遷移できるか判定
class DamageJudgment : public JudgmentBase
{
public:
	// コンストラクタ
	DamageJudgment(Enemy* enemy) :JudgmentBase(enemy) {};
	// 判定
	bool Judgment();
};

// DeathNodeに遷移できるか判定
class DeathJudgment : public JudgmentBase
{
public:
	// コンストラクタ
	DeathJudgment(Enemy* enemy) :JudgmentBase(enemy) {};
	// 判定
	bool Judgment();
};

// Escapeに遷移できるか判定
class EscapeJudgment : public JudgmentBase
{
public:
	// コンストラクタ
	EscapeJudgment(Enemy* enemy) :JudgmentBase(enemy) {};
	// 判定
	bool Judgment();
};