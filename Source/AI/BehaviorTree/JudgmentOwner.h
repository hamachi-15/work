#pragma once
#include "JudgmentBase.h"

//***********************************
// 
// ノードに遷移させるかランダムに判定
// 
//***********************************
class RamdamNextStepJudgment :public JudgmentBase
{
public:
	// コンストラクタ
	RamdamNextStepJudgment(Enemy* enemy, int percent) : JudgmentBase(enemy), percent(percent) {};

	// 判定
	bool Judgment() override;
private:
	int percent = 0; // trueを返す確率
};
//*****************************
// 
// BattleNodeに遷移できるか判定
// 
//*****************************
class BattleJudgment : public JudgmentBase
{
public:
	// コンストラクタ
	BattleJudgment(Enemy* enemy) :JudgmentBase(enemy) {};
	
	// 判定
	bool Judgment() override;
};

//*****************************
// 
// AttackNodeに遷移できるか判定
// 
//*****************************
class AttackJudgment : public JudgmentBase
{
public:
	// コンストラクタ
	AttackJudgment(Enemy* enemy) :JudgmentBase(enemy) {};
	
	// 判定
	bool Judgment() override;
};

//*****************************
// 
// WanderNodeに遷移できるか判定
// 
//*****************************
class WanderJudgment : public JudgmentBase
{
public:
	// コンストラクタ
	WanderJudgment(Enemy* enemy) :JudgmentBase(enemy) {};
	
	// 判定
	bool Judgment() override;
};

//*****************************
// 
// DamageNodeに遷移できるか判定
// 
//*****************************
class DamageJudgment : public JudgmentBase
{
public:
	// コンストラクタ
	DamageJudgment(Enemy* enemy) :JudgmentBase(enemy) {};
	
	// 判定
	bool Judgment() override;
};

//*****************************
// 
// DeathNodeに遷移できるか判定
// 
//*****************************
class DeathJudgment : public JudgmentBase
{
public:
	// コンストラクタ
	DeathJudgment(Enemy* enemy) :JudgmentBase(enemy) {};
	
	// 判定
	bool Judgment() override;
};

//*****************************
// 
// Escapeに遷移できるか判定
// 
//*****************************
class EscapeJudgment : public JudgmentBase
{
public:
	// コンストラクタ
	EscapeJudgment(Enemy* enemy) :JudgmentBase(enemy) {};

	// 判定
	bool Judgment() override;
};