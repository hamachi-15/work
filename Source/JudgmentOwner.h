#pragma once
#include "JudgmentBase.h"

// BattleNode�ɑJ�ڂł��邩����
class BattleJudgment : public JudgmentBase
{
public:
	// �R���X�g���N�^
	BattleJudgment(Enemy* enemy) :JudgmentBase(enemy) {};
	// ����
	bool Judgment();
};

// AttackNode�ɑJ�ڂł��邩����
class AttackJudgment : public JudgmentBase
{
public:
	// �R���X�g���N�^
	AttackJudgment(Enemy* enemy) :JudgmentBase(enemy) {};
	// ����
	bool Judgment();
};

// WanderNode�ɑJ�ڂł��邩����
class WanderJudgment : public JudgmentBase
{
public:
	// �R���X�g���N�^
	WanderJudgment(Enemy* enemy) :JudgmentBase(enemy) {};
	// ����
	bool Judgment();
};

// DamageNode�ɑJ�ڂł��邩����
class DamageJudgment : public JudgmentBase
{
public:
	// �R���X�g���N�^
	DamageJudgment(Enemy* enemy) :JudgmentBase(enemy) {};
	// ����
	bool Judgment();
};

// DeathNode�ɑJ�ڂł��邩����
class DeathJudgment : public JudgmentBase
{
public:
	// �R���X�g���N�^
	DeathJudgment(Enemy* enemy) :JudgmentBase(enemy) {};
	// ����
	bool Judgment();
};

// Escape�ɑJ�ڂł��邩����
class EscapeJudgment : public JudgmentBase
{
public:
	// �R���X�g���N�^
	EscapeJudgment(Enemy* enemy) :JudgmentBase(enemy) {};
	// ����
	bool Judgment();
};