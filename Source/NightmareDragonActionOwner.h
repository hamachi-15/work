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
	// コンストラクタ
	DefendAction(Enemy* enemy) : ActionBase(enemy) {}

	// 実行前処理
	void Start(std::string action_name)override;

	// 実行処理
	ActionBase::State Run(float elapsed_time)override;
};

//*****************************
// 噛みつき攻撃
//*****************************
class BasicAttackAction : public ActionBase
{
public:
	// コンストラクタ
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
	// コンストラクタ
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
	// コンストラクタ
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
	// コンストラクタ
	JumpAttackAction(Enemy* enemy) : ActionBase(enemy) {}

	// 実行前処理
	void Start(std::string action_name)override;

	// 実行処理
	ActionBase::State Run(float elapsed_time)override;
};