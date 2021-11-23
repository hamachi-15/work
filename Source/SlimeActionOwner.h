#pragma once
#include "ActionBase.h"

// ジャンプ攻撃行動
class JumpAttackAction : public ActionBase
{
public:
	JumpAttackAction(Enemy* enemy) :ActionBase(enemy) {}
	void Start(std::string action_name);
	ActionBase::State Run(float elapsed_time);
};

// 体当たり攻撃行動
class BodyAttackAction : public ActionBase
{
public:
	BodyAttackAction(Enemy* enemy) :ActionBase(enemy) {}
	void Start(std::string action_name);
	ActionBase::State Run(float elapsed_time);
};