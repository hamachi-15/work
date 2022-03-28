#pragma once
#include "ActionBase.h"
//*****************************
// 
// 特定位置へ移動
// 
//*****************************
class MoveToTargetAction : public ActionBase
{
public:
	MoveToTargetAction(Enemy* enemy, DirectX::XMFLOAT3& target_position) : ActionBase(enemy), target_position(target_position) {}

	// 実行前処理
	void Start() override;

	// 実行処理
	ActionBase::State Run(float elapsed_time) override;
private:
	DirectX::XMFLOAT3 target_position = {};
};
//*****************************
// 
// 徘徊行動
// 
//*****************************
class WanderAction : public ActionBase
{
public:
	// コンストラクタ
	WanderAction(Enemy* enemy) :ActionBase(enemy) {}

	// 実行前処理
	void Start() override;

	// 実行処理
	ActionBase::State Run(float elapsed_time) override;
};

//*****************************
// 
// 追跡行動
// 
//*****************************
class PursuitAction : public ActionBase
{
public:
	// コンストラクタ
	PursuitAction(Enemy* enemy) :ActionBase(enemy) {}

	// 実行前処理
	void Start() override;

	// 実行処理
	ActionBase::State Run(float elapsed_time) override;
};

//*****************************
// 
// 待機行動
// 
//*****************************
class IdleAction : public ActionBase
{
public:
	// コンストラクタ
	IdleAction(Enemy* enemy) :ActionBase(enemy) {}

	// 実行前処理
	void Start() override;

	// 実行処理
	ActionBase::State Run(float elapsed_time) override;
};

//*****************************
// 
// ダメージ行動
// 
//*****************************
class DamageAction : public ActionBase
{
public:
	// コンストラクタ
	DamageAction(Enemy* enemy) :ActionBase(enemy) {}

	// 実行前処理
	void Start() override;

	// 実行処理
	ActionBase::State Run(float elapsed_time) override;
};

//*****************************
// 
// 死亡行動
// 
//*****************************
class DeathAction : public ActionBase
{
public:
	// コンストラクタ
	DeathAction(Enemy* enemy) :ActionBase(enemy) {}

	// 実行前処理
	void Start() override;

	// 実行処理
	ActionBase::State Run(float elapsed_time) override;
};

//*****************************
// 
// 逃走行動
// 
//*****************************
class LeaveAction : public ActionBase
{
public:
	// コンストラクタ
	LeaveAction(Enemy* enemy) :ActionBase(enemy) {}

	// 実行前処理
	void Start() override;

	// 実行処理
	ActionBase::State Run(float elapsed_time) override;
};

//*****************************
// 
// 回復行動
// 
//*****************************
class RecoverAction : public ActionBase
{
public:
	// コンストラクタ
	RecoverAction(Enemy* enemy) :ActionBase(enemy) {}

	// 実行前処理
	void Start() override;

	// 実行処理
	ActionBase::State Run(float elapsed_time) override;
};

//*****************************
// 
// 睡眠行動
// 
//*****************************
class SleepAction : public ActionBase
{
public:
	// コンストラクタ
	SleepAction(Enemy* enemy) : ActionBase(enemy) {}

	// 実行前処理
	void Start() override;

	// 実行処理
	ActionBase::State Run(float elapsed_time) override;
};
