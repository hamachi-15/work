#include "DragonActionOwner.h"
#include "Mathf.h"
#include "ActorManager.h"
#include "Collision.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "GameDatabase.h"
#include "Charactor.h"


//*****************************
// 
// ガードアクション
// 
//*****************************
// -----------------------------
// 実行前処理
// -----------------------------
void DefendAction::Start(std::string action_name)
{
	// ガードアクションのデータ取得
	std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("NightmareDragonDefend");
	// アニメーション再生
	owner->PlayAnimation(animation);
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
void BasicAttackAction::Start(std::string action_name)
{
	// ガードアクションのデータ取得
	std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("NightmareDragonBasicAttack");
	// アニメーション再生
	owner->PlayAnimation(animation);
}

// -----------------------------
// 実行処理
// -----------------------------
ActionBase::State BasicAttackAction::Run(float elapsed_time)
{
	CollisionManager& collision_manager = CollisionManager::Instance();
	std::shared_ptr<Actor> actor = owner->GetActor();
	// 現在のコリジョンフラグと1フレーム前のコリジョンフラグを取得
	bool old_collision_time_flag = owner->GetCharactor()->GetOldCollisionTimeFlag();

	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	bool collision_time_flag = owner->GetCharactor()->SearchAnimationTime(actor, 0.2f, 0.6f);

	// 1フレーム前からコリジョンフラグが変化していたら
	if (old_collision_time_flag != collision_time_flag)
	{
		// コリジョンのあたり判定のオンオフを切り替える
		std::shared_ptr<CollisionSphere> collision = collision_manager.GetCollisionSphereFromName("NightmareDragonJaw02");

		// 当たり判定を行うフラグを変化させる
		collision->SetCollisionFlag(collision_time_flag);
	}
	// 前フレームのコリジョンフラグを代入。コリジョンフラグが変化したタイミングを調べる用
	owner->GetCharactor()->SetOldCollisionTimeFlag(collision_time_flag);

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
void ClawAttackAction::Start(std::string action_name)
{
	// ガードアクションのデータ取得
	std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("NightmareDragonClawAttack");
	// アニメーション再生
	owner->PlayAnimation(animation);
}

// -----------------------------
// 実行処理
// -----------------------------
ActionBase::State ClawAttackAction::Run(float elapsed_time)
{
	CollisionManager& collision_manager = CollisionManager::Instance();
	std::shared_ptr<Actor> actor = owner->GetActor();
	// 現在のコリジョンフラグと1フレーム前のコリジョンフラグを取得
	bool old_collision_time_flag = owner->GetCharactor()->GetOldCollisionTimeFlag();

	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	bool collision_time_flag = owner->GetCharactor()->SearchAnimationTime(actor, 0.2f, 0.6f);

	// 1フレーム前からコリジョンフラグが変化していたら
	if (old_collision_time_flag != collision_time_flag)
	{
		// コリジョンのあたり判定のオンオフを切り替える
		std::shared_ptr<CollisionSphere> collision = collision_manager.GetCollisionSphereFromName("NightmareDragonRightWrist");

		// 当たり判定を行うフラグを変化させる
		collision->SetCollisionFlag(collision_time_flag);
	}
	// 前フレームのコリジョンフラグを代入。コリジョンフラグが変化したタイミングを調べる用
	owner->GetCharactor()->SetOldCollisionTimeFlag(collision_time_flag);

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
void HornAttackAction::Start(std::string action_name)
{
	// ガードアクションのデータ取得
	std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("NightmareDragonHornAttack");
	// アニメーション再生
	owner->PlayAnimation(animation);
}

// -----------------------------
// 実行処理
// -----------------------------
ActionBase::State HornAttackAction::Run(float elapsed_time)
{
	CollisionManager& collision_manager = CollisionManager::Instance();
	std::shared_ptr<Actor> actor = owner->GetActor();
	// 現在のコリジョンフラグと1フレーム前のコリジョンフラグを取得
	bool old_collision_time_flag = owner->GetCharactor()->GetOldCollisionTimeFlag();

	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	bool collision_time_flag = owner->GetCharactor()->SearchAnimationTime(actor, 0.2f, 0.6f);

	// 1フレーム前からコリジョンフラグが変化していたら
	if (old_collision_time_flag != collision_time_flag)
	{
		// コリジョンのあたり判定のオンオフを切り替える
		std::shared_ptr<CollisionSphere> collision = collision_manager.GetCollisionSphereFromName("NightmareDragonHead");

		// 当たり判定を行うフラグを変化させる
		collision->SetCollisionFlag(collision_time_flag);
	}
	// 前フレームのコリジョンフラグを代入。コリジョンフラグが変化したタイミングを調べる用
	owner->GetCharactor()->SetOldCollisionTimeFlag(collision_time_flag);

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
void BodyPressAttackAction::Start(std::string action_name)
{
	// ガードアクションのデータ取得
	std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("NightmareDragonDefend");
	// アニメーション再生
	owner->PlayAnimation(animation);
}

// -----------------------------
// 実行処理
// -----------------------------
ActionBase::State BodyPressAttackAction::Run(float elapsed_time)
{
	CollisionManager& collision_manager = CollisionManager::Instance();
	std::shared_ptr<Actor> actor = owner->GetActor();
	// 現在のコリジョンフラグと1フレーム前のコリジョンフラグを取得
	bool old_collision_time_flag = owner->GetCharactor()->GetOldCollisionTimeFlag();

	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	bool collision_time_flag = owner->GetCharactor()->SearchAnimationTime(actor, 0.2f, 0.6f);

	// 1フレーム前からコリジョンフラグが変化していたら
	if (old_collision_time_flag != collision_time_flag)
	{
		// コリジョンのあたり判定のオンオフを切り替える
		std::shared_ptr<CollisionSphere> collision = collision_manager.GetCollisionSphereFromName("NightmareDragonBody");

		// 当たり判定を行うフラグを変化させる
		collision->SetCollisionFlag(collision_time_flag);
	}
	// 前フレームのコリジョンフラグを代入。コリジョンフラグが変化したタイミングを調べる用
	owner->GetCharactor()->SetOldCollisionTimeFlag(collision_time_flag);

	// アニメーション再生が終了したら完了を返す
	if (!owner->GetActor()->GetModel()->IsPlayAnimation())
	{
		owner->SetRunTimer(0.0f);
		owner->SetAttackFlag(false);
		return ActionBase::State::Complete;
	}
	return ActionBase::State::Run;
}
