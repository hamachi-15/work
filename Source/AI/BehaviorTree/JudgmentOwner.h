#pragma once
#include "JudgmentBase.h"

//***********************************
// 
// �m�[�h�ɑJ�ڂ����邩�����_���ɔ���
// 
//***********************************
class RamdamNextStepJudgment :public JudgmentBase
{
public:
	// �R���X�g���N�^
	RamdamNextStepJudgment(Enemy* enemy, int percent) : JudgmentBase(enemy), percent(percent) {};

	// ����
	bool Judgment() override;
private:
	int percent = 0; // true��Ԃ��m��
};
//*****************************
// 
// BattleNode�ɑJ�ڂł��邩����
// 
//*****************************
class BattleJudgment : public JudgmentBase
{
public:
	// �R���X�g���N�^
	BattleJudgment(Enemy* enemy) :JudgmentBase(enemy) {};
	
	// ����
	bool Judgment() override;
};

//*****************************
// 
// AttackNode�ɑJ�ڂł��邩����
// 
//*****************************
class AttackJudgment : public JudgmentBase
{
public:
	// �R���X�g���N�^
	AttackJudgment(Enemy* enemy) :JudgmentBase(enemy) {};
	
	// ����
	bool Judgment() override;
};

//*****************************
// 
// WanderNode�ɑJ�ڂł��邩����
// 
//*****************************
class WanderJudgment : public JudgmentBase
{
public:
	// �R���X�g���N�^
	WanderJudgment(Enemy* enemy) :JudgmentBase(enemy) {};
	
	// ����
	bool Judgment() override;
};

//*****************************
// 
// DamageNode�ɑJ�ڂł��邩����
// 
//*****************************
class DamageJudgment : public JudgmentBase
{
public:
	// �R���X�g���N�^
	DamageJudgment(Enemy* enemy) :JudgmentBase(enemy) {};
	
	// ����
	bool Judgment() override;
};

//*****************************
// 
// DeathNode�ɑJ�ڂł��邩����
// 
//*****************************
class DeathJudgment : public JudgmentBase
{
public:
	// �R���X�g���N�^
	DeathJudgment(Enemy* enemy) :JudgmentBase(enemy) {};
	
	// ����
	bool Judgment() override;
};

//*****************************
// 
// Escape�ɑJ�ڂł��邩����
// 
//*****************************
class EscapeJudgment : public JudgmentBase
{
public:
	// �R���X�g���N�^
	EscapeJudgment(Enemy* enemy) :JudgmentBase(enemy) {};

	// ����
	bool Judgment() override;
};