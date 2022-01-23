#include "ActionOwner.h"
#include "Mathf.h"
#include "ActorManager.h"
#include "Collision.h"
#include "Enemy.h"
#include "EnemyManager.h"

#include "GameDatabase.h"
#include "Charactor.h"

#include "Model.h"

//*****************************
// 
// 特定位置へ移動
// 
//*****************************
//----------------------------
// 実行前処理
//----------------------------
void MoveToTargetAction::Start()
{
	std::string owner_name = owner->GetName();
	owner_name += "Run";

	// アニメーション再生
	owner->PlayAnimation(owner_name.c_str());

	// 目標地点を設定
	owner->SetTargetPosition(target_position);
}

//----------------------------
// 実行処理
//----------------------------
ActionBase::State MoveToTargetAction::Run(float elapsed_time)
{
	// アクター取得
	std::shared_ptr<Actor> owner_actor = owner->GetActor();

	// 目的地へ着いたかの判定処理
	if (JedgmentToTargetPosition(owner->GetTargetPosition(), owner_actor->GetPosition(), owner_actor->GetName()))
	{
		// ついたら終了
		return ActionBase::State::Complete;
	}

	// 目的地点へ移動
	owner->MoveToTarget(elapsed_time, 1.5f);

	// 実行中
	return ActionBase::State::Run;
}

//***********************************
// 
// 待機行動
// 
//***********************************
//----------------------------------
// 実行前処理
//----------------------------------
void IdleAction::Start()
{
	std::string owner_name = owner->GetName();
	owner_name +="Idle";

	// アニメーション再生
	owner->PlayAnimation(owner_name.c_str());
}

//----------------------------------
// 実行処理
//----------------------------------
ActionBase::State IdleAction::Run(float elapsed_time)
{
	float run_timer = owner->GetRunTimer();
	if (run_timer <= 0.0f)
	{
		owner->SetRunTimer(Mathf::RandomRange(3.0f, 5.0f));
		run_timer = owner->GetRunTimer();
	}
	else
	{
		run_timer = owner->GetRunTimer() - elapsed_time;
	}

	owner->SetRunTimer(run_timer);

	if (run_timer <= 0.0f)
	{
		owner->SetRandomTargetPosition();
		owner->SetRunTimer(0.0f);
		return ActionBase::State::Complete;
	}
	if (owner->SearchPlayer())
	{
		owner->SetRunTimer(0.0f);
		return ActionBase::State::Complete;
	}
	return ActionBase::State::Run;
}

//***********************************
// 
// ダメージ行動
// 
//***********************************
//----------------------------------
// 実行前処理
//----------------------------------
void DamageAction::Start()
{
	std::string animation_name = owner->GetName();
	animation_name += "Damage";

	// アニメーション再生
	owner->PlayAnimation(animation_name.c_str());
}

//----------------------------------
// 実行処理
//----------------------------------
ActionBase::State DamageAction::Run(float elapsed_time)
{
	// ダメージを与え、死亡しなければ
	if (owner->GetCharactor()->ApplyDamage(1, 0.7f))
	{// ノックバックする
		float power = 10.0f;
		DirectX::XMVECTOR position = DirectX::XMLoadFloat3(&owner->GetActor()->GetPosition());
		DirectX::XMVECTOR vector_weppon_position = DirectX::XMLoadFloat3(&owner->GetHitPosition());
		DirectX::XMVECTOR normalize_vector = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(position, vector_weppon_position));
		DirectX::XMFLOAT3 float_normalize_vector;
		DirectX::XMStoreFloat3(&float_normalize_vector, normalize_vector);
		DirectX::XMFLOAT3 impulse;
		impulse.x = float_normalize_vector.x * power;
		impulse.y = power * 0.5f;
		impulse.z = float_normalize_vector.z * power;
		owner->GetMovement()->AddImpulse(impulse);
	}
	float run_timer = owner->GetRunTimer();
	if (run_timer <= 0.0f)
	{
		owner->SetRunTimer(0.5f);
		run_timer = owner->GetRunTimer();
	}
	else
	{
		// タイマー処理
		run_timer = owner->GetRunTimer() - elapsed_time;
	}

	//// タイマー更新
	owner->SetRunTimer(run_timer);
	

	if (run_timer <= 0.0f)
	{
		owner->SetRandomTargetPosition();
		owner->SetRunTimer(0.0f);
		owner->GetCharactor()->SetDamageFlag(false);
		return ActionBase::State::Complete;
	}

	return ActionBase::State::Run;
}

//***********************************
// 
// 死亡行動
// 
//***********************************
//----------------------------------
// 実行前処理
//----------------------------------
void DeathAction::Start()
{
	// 死亡アニメーション再生
	std::string animation_name = owner->GetName();
	animation_name += "Death";

	// アニメーション再生
	owner->PlayAnimation(animation_name.c_str());
}

//----------------------------------
// 実行処理
//----------------------------------
ActionBase::State DeathAction::Run(float elapsed_time)
{
	// アニメーション再生が終わればアクターを破棄
	if (!owner->GetActor()->GetModel()->IsPlayAnimation())
	{
		owner->Destroy();
		return ActionBase::State::Complete;
	}
	return ActionBase::State::Run;
}

//***********************************
// 
// 徘徊行動
// 
//***********************************
//----------------------------
// 実行前処理
//----------------------------
void WanderAction::Start()
{
	std::string animation_name = owner->GetName();
	animation_name += "Walk";

	// アニメーション再生
	owner->PlayAnimation(animation_name.c_str());
}

//----------------------------
// 実行処理
//----------------------------
ActionBase::State WanderAction::Run(float elapsed_time)
{
	// アクター取得
	std::shared_ptr<Actor> owner_actor = owner->GetActor();

	// 目的地へ着いたかの判定処理
	if (JedgmentToTargetPosition(owner->GetTargetPosition(), owner_actor->GetPosition(), owner_actor->GetName()))
	{
		return ActionBase::State::Complete;
	}

	// 目的地点へ移動
	owner->MoveToTarget(elapsed_time, 0.5f);

	// プレイヤー索敵成功したら
	if (owner->SearchPlayer())
	{
		owner->SetRunTimer(0.0f);
		return ActionBase::State::Complete;
	}

	// 実行中
	return ActionBase::State::Run;
}

//***********************************
// 
// 逃走行動
// 
//***********************************
//----------------------------------
// 実行前処理
//----------------------------------
void LeaveAction::Start()
{
	// アクター取得
	std::shared_ptr<Actor> owner_actor = owner->GetActor();
	
	// アニメーション名を設定して
	std::string animation_name = owner->GetName();
	animation_name += "Walk";

	// アニメーション再生
	owner->PlayAnimation(animation_name.c_str());

	// 目標地点をプレイヤーと正反対のベクトル×5の位置に指定
	DirectX::XMVECTOR start_position = DirectX::XMLoadFloat3(&ActorManager::Instance().GetActor("Player")->GetPosition());
	DirectX::XMVECTOR end_position = DirectX::XMLoadFloat3(&owner_actor->GetPosition());

	DirectX::XMVECTOR vec_target_position = DirectX::XMVectorSubtract(end_position, start_position);
	vec_target_position = DirectX::XMVector3Normalize(vec_target_position);
	vec_target_position = DirectX::XMVectorScale(vec_target_position, 5.0f);

	DirectX::XMFLOAT3 target_position;
	DirectX::XMStoreFloat3(&target_position, vec_target_position);
	DirectX::XMFLOAT3 owner_position = owner_actor->GetPosition();
	target_position.x += owner_position.x;
	target_position.y += owner_position.y;
	target_position.z += owner_position.z;
	owner->SetTargetPosition(target_position);
}

//----------------------------------
// 実行処理
//----------------------------------
ActionBase::State LeaveAction::Run(float elapsed_time)
{
	std::shared_ptr<Actor> owner_actor = owner->GetActor();

	// 目的地点へ移動
	owner->MoveToTarget(elapsed_time, 1.0);

	// 目的地へ着いたら
	if (JedgmentToTargetPosition(owner->GetTargetPosition(), owner_actor->GetPosition(), owner_actor->GetName()))
	{
		// コンプリート
		return ActionBase::State::Complete;
	}

	// 継続
	return ActionBase::State::Run;
}

//***********************************
// 
// 回復行動
// 
//***********************************
//----------------------------------
// 実行前処理
//----------------------------------
void RecoverAction::Start()
{
	std::string animation_name = owner->GetName();;
	animation_name += "Idle";

	// アニメーション再生
	owner->PlayAnimation(animation_name.c_str());

}

//----------------------------------
// 実行処理
//----------------------------------
ActionBase::State RecoverAction::Run(float elapsed_time)
{
	float runTimer = owner->GetRunTimer();
	if (runTimer <= 0.0f)
	{
		owner->SetRunTimer(3.0f);
		runTimer = owner->GetRunTimer();
	}
	else
	{
		// タイマー処理
		runTimer = owner->GetRunTimer() - elapsed_time;
	}

	// タイマー更新
	owner->SetRunTimer(runTimer);

	if (runTimer <= 0.0f) {
		owner->GetCharactor()->SetHealth(owner->GetCharactor()->GetMaxHealth());
		owner->SetRunTimer(0.0f);
		return ActionBase::State::Complete;
	}

	return ActionBase::State::Run;
}

//***********************************
// 
// 追跡行動
// 
//***********************************
//----------------------------------
// 実行前処理
//----------------------------------
void PursuitAction::Start()
{
	std::string animation_name = owner->GetName();;
	animation_name += "Walk";

	// アニメーション再生
	owner->PlayAnimation(animation_name.c_str());
}

//----------------------------------
// 実行処理
//----------------------------------
ActionBase::State PursuitAction::Run(float elapsed_time)
{
	float run_timer = owner->GetRunTimer();
	if (run_timer <= 0.0f)
	{
		owner->SetRunTimer(Mathf::RandomRange(3.0f, 5.0f));
		run_timer = owner->GetRunTimer();
	}
	else
	{
		// タイマー処理
		run_timer = owner->GetRunTimer() - elapsed_time;
	}

	// タイマー更新
	owner->SetRunTimer(run_timer);

	// 目標地点をプレイヤー位置に設定
	owner->SetTargetPosition(ActorManager::Instance().GetActor("Player")->GetPosition());

	// 目的地点へ移動
	owner->MoveToTarget(elapsed_time, 1.0f);

	DirectX::XMFLOAT3 position = owner->GetActor()->GetPosition();
	DirectX::XMFLOAT3 target_position = owner->GetTargetPosition();

	float vx = target_position.x - position.x;
	float vy = target_position.y - position.y;
	float vz = target_position.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);

	if (dist < owner->GetAttackRange())
	{
		owner->SetRunTimer(0.0f);
		return ActionBase::State::Complete;
	}
	if (run_timer <= 0.0f)
	{
		owner->SetRunTimer(0.0f);
		return ActionBase::State::Failed;
	}

	return ActionBase::State::Run;
}

//********************************
// 
// 睡眠行動
// 
// *******************************
// -----------------------------
// 実行前処理
//------------------------------
void SleepAction::Start()
{
	std::string owner_name = owner->GetName();
	owner_name += "Sleep";

	// アニメーション再生
	owner->PlayAnimation(owner_name.c_str());
}

//------------------------------
// 実行処理
//------------------------------
ActionBase::State SleepAction::Run(float elapsed_time)
{
	//float run_timer = owner->GetRunTimer();
	//if (run_timer <= 0.0f)
	//{
	//	owner->SetRunTimer(Mathf::RandomRange(3.0f, 5.0f));
	//	run_timer = owner->GetRunTimer();
	//}
	//else
	//{
	//	run_timer = owner->GetRunTimer() - elapsed_time;
	//}

	//owner->SetRunTimer(run_timer);

	//if (run_timer <= 0.0f)
	//{
	//	owner->SetRandomTargetPosition();
	//	owner->SetRunTimer(0.0f);
	//	return ActionBase::State::Complete;
	//}
	//if (owner->SearchPlayer())
	//{
	//	owner->SetRunTimer(0.0f);
	//	return ActionBase::State::Complete;
	//}

	return ActionBase::State::Run;
}