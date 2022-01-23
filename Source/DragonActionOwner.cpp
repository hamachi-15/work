#include "GameDatabase.h"
#include "EnemyCategory.h"

#include "DragonActionOwner.h"
#include "Actor.h"
#include "Enemy.h"
#include "Charactor.h"
// マネージャー
#include "ActorManager.h"
#include "EnemyManager.h"

#include "Mathf.h"

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

//***************************************
// 
// 咆哮行動
// 
//***************************************
// -----------------------------
// 実行前処理
//------------------------------
void ScreamAction::Start()
{
	// 継続するタイマー設定
	owner->SetRunTimer(3.0f);

	// アニメーション名設定
	std::string animation_name = owner->GetName();
	animation_name += "Scream";

	// アニメーション再生
	owner->PlayAnimation(animation_name.c_str());
}

//------------------------------
// 実行処理
//------------------------------
ActionBase::State ScreamAction::Run(float elapsed_time)
{
	// タイマー取得
	float run_timer = owner->GetRunTimer();

	// タイマー減少
	run_timer -= elapsed_time;

	// アニメーション再生が終了したら完了を返す
	if (!owner->GetActor()->GetModel()->IsPlayAnimation() && run_timer <= 0.0f)
	{
		owner->SetRunTimer(0.0f);
		return ActionBase::State::Complete;
	}

	// タイマー設定
	owner->SetRunTimer(run_timer);

	// 継続
	return ActionBase::State::Run;
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
	std::string collision_name = actor->GetName();
	collision_name += "mouth";
	AttackCollision(actor, collision_name.c_str(), collision_time_data);

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
	std::string collision_name = actor->GetName();
	collision_name += "RightWrist";
	AttackCollision(actor, collision_name.c_str(), collision_time_data);

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
	std::string collision_name = actor->GetName();
	collision_name += "Head";
	AttackCollision(actor, collision_name.c_str(), collision_time_data);

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
	// 継続するタイマー設定
	owner->SetRunTimer(2.0f);

	// アニメーション再生
	owner->PlayAnimation("NightmareDragonJump");

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
	std::string collision_name = actor->GetName();
	AttackCollision(actor, collision_name.c_str(), collision_time_data, CollisionMeshType::Cylinder);

	// タイマー取得
	float run_timer = owner->GetRunTimer();

	// タイマー減少
	run_timer -= elapsed_time;

	// アニメーション再生が終了したら完了を返す
	if (!owner->GetActor()->GetModel()->IsPlayAnimation() && run_timer <= 0.0f)
	{
		owner->SetRunTimer(0.0f);
		owner->SetAttackFlag(false);
		return ActionBase::State::Complete;
	}

	// タイマー設定
	owner->SetRunTimer(run_timer);

	return ActionBase::State::Run;
}

//*****************************
// 
// 突進攻撃
// 
//*****************************
// -----------------------------
// 実行前処理
// -----------------------------
void LungesAttackAction::Start()
{
	// アニメーション再生
	owner->PlayAnimation("NightmareDragonRun");

	// 当たり判定を行う時間のデータを取得
	//collision_time_data = GameDataBase::Instance().GetAttackCollitionTimeData(AttackCategory::BodyPressAttack, EnemyCategory::NightmareDragon);
	// 反対側にターゲットを設定
	// ターゲット座標に値が入っていない時
	if (Mathf::VectorLength(target_position) == 0)
	{
		// 目標地点をプレイヤー位置に設定
		owner->SetTargetPosition(ActorManager::Instance().GetActor("Player")->GetPosition());
	}
	else
	{
		// 目標地点を設定された位置に設定
		owner->SetTargetPosition(target_position);
	}
}

// -----------------------------
// 実行処理
// -----------------------------
ActionBase::State LungesAttackAction::Run(float elapsed_time)
{
	// アクター取得
	std::shared_ptr<Actor> owner_actor = owner->GetActor();

	// 目的地へ着いたかの判定処理
	if (JedgmentToTargetPosition(owner->GetTargetPosition(), owner_actor->GetPosition(), owner_actor->GetName()))
	{
		return ActionBase::State::Complete;
	}

	// 目的地点へ移動
	owner->MoveToTarget(elapsed_time, 1.5f);

	return ActionBase::State::Run;
}
