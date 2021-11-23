#pragma once
#include "ActionBase.h"

// ジャンプ攻撃行動
//class JumpAttackAction : public ActionBase
//{
//public:
//	JumpAttackAction(Enemy* enemy) : ActionBase(enemy) {}
//	void Start(std::string action_name);
//	ActionBase::State Run(float elapsed_time);
//};
//
// パンチ攻撃行動
class PunchAttackAction : public ActionBase
{
public:
	PunchAttackAction(Enemy* enemy) : ActionBase(enemy) {}
	void Start(std::string action_name);
	ActionBase::State Run(float elapsed_time);
};

// 強打攻撃行動
class SwipingAttackAction : public ActionBase
{
public:
	SwipingAttackAction(Enemy* enemy) : ActionBase(enemy) {}
	void Start(std::string action_name);
	ActionBase::State Run(float elapsed_time);

};