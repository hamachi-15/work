#pragma once
#include "ActionBase.h"

//*****************************
// 睡眠アクション
//*****************************
class SleepAction : public ActionBase
{
public:
	// コンストラクタ
	SleepAction(Enemy* enemy) : ActionBase(enemy){}

	// 実行前処理
	void Start(std::string action_name)override;

	// 実行処理
	ActionBase::State Run(float elapsed_time)override;

};

//*****************************
// ガードアクション
//*****************************
class DefendAction : public ActionBase
{
public:
	DefendAction(Enemy* enemy) : ActionBase(enemy) {}

	// 実行前処理
	void Start(std::string action_name)override;

	// 実行処理
	ActionBase::State Run(float elapsed_time)override;
};

//*****************************
// ベーシックアタック
//*****************************
class BasicAttackAction : public ActionBase
{
public:
	BasicAttackAction(Enemy* enemy) : ActionBase(enemy) {}

	// 実行前処理
	void Start(std::string action_name)override;

	// 実行処理
	ActionBase::State Run(float elapsed_time)override;
};

//*****************************
// 爪攻撃
//*****************************
class ClawAttackAction : public ActionBase
{
public:
	ClawAttackAction(Enemy* enemy) : ActionBase(enemy){}

	// 実行前処理
	void Start(std::string action_name)override;

	// 実行処理
	ActionBase::State Run(float elapsed_time)override;
};

//*****************************
// 角攻撃
//*****************************
class HornAttackAction : public ActionBase
{
public:
	HornAttackAction(Enemy* enemy) : ActionBase(enemy) {}

	// 実行前処理
	void Start(std::string action_name)override;

	// 実行処理
	ActionBase::State Run(float elapsed_time)override;
};

//*****************************
// ジャンプ攻撃
//*****************************
class JumpAttackAction : public ActionBase
{
public:
	JumpAttackAction(Enemy* enemy) : ActionBase(enemy) {}

};