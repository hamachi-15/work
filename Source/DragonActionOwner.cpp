#include "GameDatabase.h"
#include "EnemyCategory.h"

#include "DragonActionOwner.h"
#include "Actor.h"
#include "Enemy.h"
#include "Charactor.h"
// マネージャー
#include "ActorManager.h"
#include "EnemyManager.h"
#include "CollisionManager.h"
#include "EnemyTerritoryManager.h"

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
void TurnToTargetAction::Start()
{
	// アニメーション再生
	owner->PlayAnimation("NightmareDragonWalkRight");

	larp_timer = 0.0f;
	// プレイヤーへの方向ベクトル算出
	// プレイヤー座標取得
	DirectX::XMFLOAT3 player_position = ActorManager::Instance().GetActor("Player")->GetPosition();
	turn_direction = Mathf::ReturnNormalizeFloatSubtract(player_position, owner->GetActor()->GetPosition());
	// Y軸は考慮しない
	turn_direction.y = 0.0f;
}

// -----------------------------
// 実行処理
// -----------------------------
ActionBase::State TurnToTargetAction::Run(float elapsed_time)
{

	float run_timer = owner->GetRunTimer();
	if (run_timer <= 0.0f)
	{
		owner->SetRunTimer(1.0f);
		run_timer = owner->GetRunTimer();
	}
	else
	{
		// タイマー処理
		larp_timer += elapsed_time;
		run_timer = owner->GetRunTimer() - elapsed_time;
	}

	// 振り向き処理
	DirectX::XMFLOAT3 angle = owner->GetActor()->GetAngle();
	front_direction.x = sinf(angle.y);
	front_direction.z = cosf(angle.y);
	front_direction.x= Mathf::Lerp(front_direction.x, turn_direction.x, larp_timer);
	front_direction.z= Mathf::Lerp(front_direction.z, turn_direction.z, larp_timer);
	float rot = owner->TurnToTarget(front_direction);
	// タイマー更新
	owner->SetRunTimer(run_timer);

	// ランタイマーが0以下になれば
	if (run_timer <= 0)
	{
		// ランタイマーがリセットして完了する
		owner->SetRunTimer(0.0f);
		return ActionBase::State::Complete;
	}
	return ActionBase::State::Run;
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
		owner->SetRightOfAttack(false);
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
		owner->SetRightOfAttack(false);
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
		owner->SetRightOfAttack(false);
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
	//collision_name += "Body";
	AttackCollision(actor, collision_name.c_str(), collision_time_data, CollisionMeshType::Cylinder);

	// タイマー取得
	float run_timer = owner->GetRunTimer();

	// タイマー減少
	run_timer -= elapsed_time;

	// アニメーション再生が終了したら完了を返す
	if (!owner->GetActor()->GetModel()->IsPlayAnimation() && run_timer <= 0.0f)
	{
		owner->SetRunTimer(0.0f);
		owner->SetRightOfAttack(false);
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
	std::shared_ptr<Actor> owner_actor = owner->GetActor();
	// アニメーション再生
	owner->PlayAnimation("NightmareDragonRun");

	// 攻撃の当たり判定処理
	std::string collision_name = owner_actor->GetName();

	// コリジョンマネージャー取得
	CollisionManager& collision_manager = CollisionManager::Instance();
	std::shared_ptr<CollisionCylinder> collision = collision_manager.GetCollisionCylinderFromName(collision_name);
	collision->SetAttackFlag(true);

	// プレイヤーへの方向ベクトル算出
	// プレイヤー座標取得
	DirectX::XMFLOAT3 player_position = ActorManager::Instance().GetActor("Player")->GetPosition();
	target_direction = Mathf::ReturnNormalizeFloatSubtract(player_position, owner->GetActor()->GetPosition());
	// Y軸は考慮しない
	target_direction.y = 0.0f;
}

// -----------------------------
// 実行処理
// -----------------------------
ActionBase::State LungesAttackAction::Run(float elapsed_time)
{
	// アクター取得
	std::shared_ptr<Actor> owner_actor = owner->GetActor();

	float run_timer = owner->GetRunTimer();
	if (run_timer <= 0.0f)
	{
		owner->SetRunTimer(Mathf::RandomRange(2.0f, 3.0f));
		run_timer = owner->GetRunTimer();
	}
	else
	{
		// タイマー処理
		run_timer = owner->GetRunTimer() - elapsed_time;
	}

	// タイマー更新
	owner->SetRunTimer(run_timer);

	// 目的地へ着いたかの判定処理
	if (JedgmentToTargetPosition(owner->GetTargetPosition(), owner_actor->GetPosition(), owner_actor->GetName()))
	{
		// 攻撃の当たり判定処理
		std::string collision_name = owner_actor->GetName();
		// コリジョンマネージャー取得
		CollisionManager& collision_manager = CollisionManager::Instance();
		std::shared_ptr<CollisionCylinder> collision = collision_manager.GetCollisionCylinderFromName(collision_name);
		collision->SetAttackFlag(false);
		owner->SetRightOfAttack(false);
		return ActionBase::State::Complete;
	}
	std::shared_ptr<EnemyTerritory> territory = EnemyTerritoryManager::Instance().GetTerritory(owner->GetBelongingToTerritory());
	DirectX::XMFLOAT3 territory_origine = territory->GetTerritoryOrigin();
	float territory_range = territory->GetTerritoryRange();
	float length;
	DirectX::XMStoreFloat(&length, DirectX::XMVector3Length(Mathf::ReturnVectorSubtract(owner->GetActor()->GetPosition(), territory_origine)));
	
	// タイムが終了したら
	if (run_timer <= 0.0f || length >= territory_range)
	{
		// タイマー設定
		owner->SetRunTimer(0.0f);
		// 攻撃の当たり判定処理
		std::string collision_name = owner_actor->GetName();
		CollisionManager& collision_manager = CollisionManager::Instance();
		std::shared_ptr<CollisionCylinder> collision = collision_manager.GetCollisionCylinderFromName(collision_name);
		collision->SetAttackFlag(false);
		owner->SetRightOfAttack(false);
		return ActionBase::State::Complete;
	}

	// 目的地点へ移動
	owner->MoveToDirection(target_direction, 2.5f);

	return ActionBase::State::Run;
}
