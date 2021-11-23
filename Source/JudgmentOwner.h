#pragma once
#include "JudgmentBase.h"

// BattleNode�ɑJ�ڂł��邩����
class BattleJudgment : public JudgmentBase
{
public:
	BattleJudgment(Enemy* enemy) :JudgmentBase(enemy) {};
	// ����
	bool Judgment();
};

// AttackNode�ɑJ�ڂł��邩����
class AttackJudgment : public JudgmentBase
{
public:
	AttackJudgment(Enemy* enemy) :JudgmentBase(enemy) {};
	// ����
	bool Judgment();
};

// WanderNode�ɑJ�ڂł��邩����
class WanderJudgment : public JudgmentBase
{
public:
	WanderJudgment(Enemy* enemy) :JudgmentBase(enemy) {};
	// ����
	bool Judgment();
};

// DamageNode�ɑJ�ڂł��邩����
class DamageJudgment : public JudgmentBase
{
public:
	DamageJudgment(Enemy* enemy) :JudgmentBase(enemy) {};
	// ����
	bool Judgment();
};

// DeathNode�ɑJ�ڂł��邩����
class DeathJudgment : public JudgmentBase
{
public:
	DeathJudgment(Enemy* enemy) :JudgmentBase(enemy) {};
	// ����
	bool Judgment();
};

// Escape�ɑJ�ڂł��邩����
class EscapeJudgment : public JudgmentBase
{
public:
	EscapeJudgment(Enemy* enemy) :JudgmentBase(enemy) {};
	// ����
	bool Judgment();
};