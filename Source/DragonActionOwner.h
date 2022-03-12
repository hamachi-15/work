#pragma once
#include "ActionBase.h"

//*****************************
// 
// 軸合わせアクション
// 
//*****************************
class TurnToTargetAction : public ActionBase
{
public:
	// コンストラクタ
	TurnToTargetAction(Enemy* enemy) : ActionBase(enemy) {}
	
	// 実行前処理
	void Start() override;

	// 実行処理
	ActionBase::State Run(float elapsed_time) override;
private:
	DirectX::XMFLOAT3 turn_direction; // 向く方向ベクトル
	DirectX::XMFLOAT3 front_direction;
	float			larp_timer = 0.0f;
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
	LungesAttackAction(Enemy* enemy) : ActionBase(enemy) {}

	// 実行前処理
	void Start() override;

	// 実行処理
	ActionBase::State Run(float elapsed_time) override;
private:
	DirectX::XMFLOAT3 target_direction = {}; // ターゲットへの方向ベクトル
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
private:
	DirectX::XMFLOAT3	direction;
	bool				flag = false;
};

//*****************************
// 
// 尻尾攻撃
// 
//*****************************
class TailAttackAction : public ActionBase
{
public:
	// コンストラクタ
	TailAttackAction(Enemy* enemy) : ActionBase(enemy) {}

	// 実行前処理
	void Start() override;

	// 実行処理
	ActionBase::State Run(float elapsed_time) override;

};