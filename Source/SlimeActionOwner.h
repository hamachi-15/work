#pragma once
#include "ActionBase.h"
//***********************************
// 
// ジャンプ攻撃行動
// 
//***********************************
class HeavyBodyAttackAction : public ActionBase
{
public:
	// コンストラクタ
	HeavyBodyAttackAction(Enemy* enemy) :ActionBase(enemy) {}
	void Start(std::string action_name);
	ActionBase::State Run(float elapsed_time);
};

//***********************************
// 
// 体当たり攻撃行動
// 
//***********************************
class BodyAttackAction : public ActionBase
{
public:
	// コンストラクタ
	BodyAttackAction(Enemy* enemy) :ActionBase(enemy) {}
	void Start(std::string action_name);
	ActionBase::State Run(float elapsed_time);
};