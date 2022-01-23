#pragma once
#include "ActionBase.h"

//*****************************
// 
// 軸合わせアクション
// 
//*****************************
class AligningAction : public ActionBase
{
public:
	// コンストラクタ
	AligningAction(Enemy* enemy) : ActionBase(enemy) {}
	
	// 実行前処理
	void Start() override;

	// 実行処理
	ActionBase::State Run(float elapsed_time) override;
};

//*****************************
// 
// 咆哮行動
// 
//*****************************
class ScreamAction : public ActionBase
{
public:
	// コンストラクタ
	ScreamAction(Enemy* enemy) : ActionBase(enemy) {}

	// 実行前処理
	void Start() override;

	// 実行処理
	ActionBase::State Run(float elapsed_time) override;
};

//*****************************
// 
// ガードアクション
// 
//*****************************
class DefendAction : public ActionBase
{
public:
	// コンストラクタ
	DefendAction(Enemy* enemy) : ActionBase(enemy) {}

	// 実行前処理
	void Start() override;

	// 実行処理
	ActionBase::State Run(float elapsed_time) override;
};

//*****************************
// 
// 噛みつき攻撃
// 
//*****************************
class BasicAttackAction : public ActionBase
{
public:
	// コンストラクタ
	BasicAttackAction(Enemy* enemy) : ActionBase(enemy) {}

	// 実行前処理
	void Start() override;

	// 実行処理
	ActionBase::State Run(float elapsed_time) override;
};

//*****************************
// 
// 爪攻撃
// 
//*****************************
class ClawAttackAction : public ActionBase
{
public:
	// コンストラクタ
	ClawAttackAction(Enemy* enemy) : ActionBase(enemy) {}

	// 実行前処理
	void Start() override;

	// 実行処理
	ActionBase::State Run(float elapsed_time) override;
};

//*****************************
// 
// 角攻撃
// 
//*****************************
class HornAttackAction : public ActionBase
{
public:
	// コンストラクタ
	HornAttackAction(Enemy* enemy) : ActionBase(enemy) {}

	// 実行前処理
	void Start() override;

	// 実行処理
	ActionBase::State Run(float elapsed_time) override;
};

//*****************************
// 
// ボディプレス攻撃
// 
//*****************************
class BodyPressAttackAction : public ActionBase
{
public:
	// コンストラクタ
	BodyPressAttackAction(Enemy* enemy) : ActionBase(enemy) {}

	// 実行前処理
	void Start() override;

	// 実行処理
	ActionBase::State Run(float elapsed_time) override;
};

//*****************************
// 
// 突進攻撃
// 
//*****************************
class LungesAttackAction : public ActionBase
{
public:
	// コンストラクタ
	LungesAttackAction(Enemy* enemy, DirectX::XMFLOAT3 target_position = { 0.0f, 0.0f, 0.0f }) : ActionBase(enemy), target_position(target_position) {}

	// 実行前処理
	void Start() override;

	// 実行処理
	ActionBase::State Run(float elapsed_time) override;
private:
	DirectX::XMFLOAT3 target_position = {};
};

//*****************************
// 
// 火球攻撃
// 
//*****************************
class FireBollAttackAction : public ActionBase
{
public:
	// コンストラクタ
	FireBollAttackAction (Enemy* enemy) : ActionBase(enemy) {}

	// 実行前処理
	void Start() override;

	// 実行処理
	ActionBase::State Run(float elapsed_time) override;
};