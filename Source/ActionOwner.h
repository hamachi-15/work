#pragma once
#include "ActionBase.h"

// 徘徊行動
class WanderAction : public ActionBase
{
public:
	WanderAction(Enemy* enemy) :ActionBase(enemy) {}
	void Start(std::string action_name);
	ActionBase::State Run(float elapsed_time);
};

// 追跡行動
class PursuitAction : public ActionBase
{
public:
	PursuitAction(Enemy* enemy) :ActionBase(enemy) {}
	void Start(std::string action_name);
	ActionBase::State Run(float elapsed_time);
};

// 待機行動
class IdleAction : public ActionBase
{
public:
	IdleAction(Enemy* enemy) :ActionBase(enemy) {}
	void Start(std::string action_name);
	ActionBase::State Run(float elapsed_time);
};
// ダメージ行動
class DamageAction : public ActionBase
{
public:
	DamageAction(Enemy* enemy) :ActionBase(enemy) {}
	void Start(std::string action_name);
	ActionBase::State Run(float elapsed_time);
};

// 死亡行動
class DeathAction : public ActionBase
{
public:
	DeathAction(Enemy* enemy) :ActionBase(enemy) {}
	void Start(std::string action_name);
	ActionBase::State Run(float elapsed_time);
};

// 逃走行動
class LeaveAction : public ActionBase
{
public:
	LeaveAction(Enemy* enemy) :ActionBase(enemy) {}
	void Start(std::string action_name);
	ActionBase::State Run(float elapsed_time);
};

// 回復行動
class RecoverAction : public ActionBase
{
public:
	RecoverAction(Enemy* enemy) :ActionBase(enemy) {}
	void Start(std::string action_name);
	ActionBase::State Run(float elapsed_time);
};