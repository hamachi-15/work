#include "GameDatabase.h"
#include "EnemyCategory.h"

#include "DragonActionOwner.h"
#include "Actor.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "Charactor.h"

// 汎用関数
#include "Universal.h"
//*****************************
// 
// 軸合わせアクション
// 
//*****************************
// -----------------------------
// 実行前処理
// -----------------------------
void AligningAction::Start()
{
	// アニメーション再生
	owner->PlayAnimation("NightmareDragonDefend");
}

// -----------------------------
// 実行処理
// -----------------------------
ActionBase::State AligningAction::Run(float elapsed_time)
{
	return ActionBase::State();
}
//*****************************
// 
// ガードアクション
// 
//*****************************
// -----------------------------
// 実行前処理
// -----------------------------
void DefendAction::Start()
{
	// アニメーション再生
	owner->PlayAnimation("NightmareDragonDefend");
}

// -----------------------------
// 実行処理
// -----------------------------
ActionBase::State DefendAction::Run(float elapsed_time)
{
	owner->SetRunTimer(3.0f);

	float run_timer = owner->GetRunTimer();
	if (run_timer > 0.0f)
	{
		run_timer -= elapsed_time;
	}
	else
	{
		owner->SetRunTimer(0.0f);
		return State::Complete;
	}

	return State::Run;
}

//*****************************
// 
// 噛みつき攻撃
// 
//*****************************
// -----------------------------
// 実行前処理
// -----------------------------
void BasicAttackAction::Start()
{
	// アニメーション再生
	owner->PlayAnimation("NightmareDragonBasicAttack");

	// 当たり判定を行う時間のデータを取得
	collision_time_data = GameDataBase::Instance().GetAttackCollitionTimeData(AttackCategory::BitingAttack, EnemyCategory::NightmareDragon);
}

// -----------------------------
// 実行処理
// -----------------------------
ActionBase::State BasicAttackAction::Run(float elapsed_time)
{
	// アクター取得
	std::shared_ptr<Actor> actor = owner->GetActor();

	// 攻撃の当たり判定処理
	AttackCollision(actor, "NightmareDragonJaw02", collision_time_data);

	// アニメーション再生が終了したら完了を返す
	if (!owner->GetActor()->GetModel()->IsPlayAnimation())
	{
		owner->SetRunTimer(0.0f);
		owner->SetAttackFlag(false);
		return ActionBase::State::Complete;
	}
	return ActionBase::State::Run;
}

//*****************************
// 
// 爪攻撃
// 
//*****************************
// -----------------------------
// 実行前処理
// -----------------------------
void ClawAttackAction::Start()
{
	// アニメーション再生
	owner->PlayAnimation("NightmareDragonClawAttack");
	// 当たり判定を行う時間のデータを取得
	collision_time_data = GameDataBase::Instance().GetAttackCollitionTimeData(AttackCategory::ClawAttack, EnemyCategory::NightmareDragon);
}

// -----------------------------
// 実行処理
// -----------------------------
ActionBase::State ClawAttackAction::Run(float elapsed_time)
{
	// アクター取得
	std::shared_ptr<Actor> actor = owner->GetActor();
	
	// 攻撃の当たり判定処理
	AttackCollision(actor, "NightmareDragonRightWrist", collision_time_data);

	// アニメーション再生が終了したら完了を返す
	if (!owner->GetActor()->GetModel()->IsPlayAnimation())
	{
		owner->SetRunTimer(0.0f);
		owner->SetAttackFlag(false);
		return ActionBase::State::Complete;
	}
	return ActionBase::State::Run;
}

//*****************************
// 
// 角攻撃
// 
//*****************************
// -----------------------------
// 実行前処理
// -----------------------------
void HornAttackAction::Start()
{
	// アニメーション再生
	owner->PlayAnimation("NightmareDragonHornAttack");

	// 当たり判定を行う時間のデータを取得
	collision_time_data = GameDataBase::Instance().GetAttackCollitionTimeData(AttackCategory::HornAttack, EnemyCategory::NightmareDragon);
}

// -----------------------------
// 実行処理
// -----------------------------
ActionBase::State HornAttackAction::Run(float elapsed_time)
{
	// アクター取得
	std::shared_ptr<Actor> actor = owner->GetActor();

	// 攻撃の当たり判定処理
	AttackCollision(actor, "NightmareDragonHead",collision_time_data);

	// アニメーション再生が終了したら完了を返す
	if (!owner->GetActor()->GetModel()->IsPlayAnimation())
	{
		owner->SetRunTimer(0.0f);
		owner->SetAttackFlag(false);
		return ActionBase::State::Complete;
	}
	return ActionBase::State::Run;
}

//*****************************
// 
// ジャンプ攻撃
// 
//*****************************
// -----------------------------
// 実行前処理
// -----------------------------
void BodyPressAttackAction::Start()
{
	// アニメーション再生
	owner->PlayAnimation("NightmareDragonDefend");

	// 当たり判定を行う時間のデータを取得
	collision_time_data = GameDataBase::Instance().GetAttackCollitionTimeData(AttackCategory::BodyPressAttack, EnemyCategory::NightmareDragon);
}

// -----------------------------
// 実行処理
// -----------------------------
ActionBase::State BodyPressAttackAction::Run(float elapsed_time)
{
	// アクター取得
	std::shared_ptr<Actor> actor = owner->GetActor();

	// 攻撃の当たり判定処理
	AttackCollision(actor, "NightmareDragonBody", collision_time_data, CollisionMeshType::Cylinder);

	// アニメーション再生が終了したら完了を返す
	if (!owner->GetActor()->GetModel()->IsPlayAnimation())
	{
		owner->SetRunTimer(0.0f);
		owner->SetAttackFlag(false);
		return ActionBase::State::Complete;
	}
	return ActionBase::State::Run;
}