#include "SlimeActionOwner.h"
#include "Enemy.h"
#include "GameDatabase.h"
#include "Charactor.h"
#include "Model.h"

#include "Universal.h"

//-------------------------------------
// ジャンプ攻撃行動前処理
//-------------------------------------
void HeavyBodyAttackAction::Start()
{
	// アニメーション再生
	owner->PlayAnimation("SlimeAttack2");

	// 当たり判定を行う時間のデータを取得
	collision_time_data = GameDataBase::Instance().GetAttackCollitionTimeData(AttackCategory::JumpAttack, EnemyCategory::Slime);
}

//-------------------------------------
// ジャンプ攻撃行動
//-------------------------------------
HeavyBodyAttackAction::State HeavyBodyAttackAction::Run(float elapsed_time)
{
	std::shared_ptr<Actor> actor = owner->GetActor();
	float run_timer = owner->GetRunTimer();
	if (run_timer <= 0.0f)
	{
		// スキル
		owner->SetRunTimer(0.7f);
		run_timer = owner->GetRunTimer();
	}
	else
	{
		run_timer -= elapsed_time;
	}
	// 攻撃の当たり判定処理
	AttackCollision(actor, owner->GetCharactor()->GetID() + owner->GetIdentity(), collision_time_data);

	// タイマー設定
	owner->SetRunTimer(run_timer);

	if (run_timer <= 0.0f)
	{
		owner->SetRunTimer(0.0f);
		owner->SetAttackFlag(false);
		return ActionBase::State::Complete;
	}
	// アニメーション再生が終了したら完了を返す
	if (!owner->GetActor()->GetModel()->IsPlayAnimation())
	{
		owner->SetRunTimer(0.0f);
		owner->SetAttackFlag(false);
		return ActionBase::State::Complete;
	}
	return ActionBase::State::Run;
}

//-------------------------------------
// 体当たり攻撃行動前処理
//-------------------------------------
void BodyAttackAction::Start()
{
	// アニメーション再生
	owner->PlayAnimation("SlimeAttack1");

	// 当たり判定を行う時間のデータを取得
	collision_time_data = GameDataBase::Instance().GetAttackCollitionTimeData(AttackCategory::BodyAttack, EnemyCategory::Slime);
}

//-------------------------------------
// 体当たり攻撃行動
//-------------------------------------
ActionBase::State BodyAttackAction::Run(float elapsed_time)
{
	CollisionManager& collision_manager = CollisionManager::Instance();
	std::shared_ptr<Actor> actor = owner->GetActor();
	// 現在のコリジョンフラグと1フレーム前のコリジョンフラグを取得
	bool old_collision_time_flag = owner->GetCharactor()->GetOldCollisionTimeFlag();
	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	bool collision_time_flag =owner->GetCharactor()->SearchAnimationTime(actor, 0.2f, 0.4f);
	// 当たり判定を行う時間かの判定
	//bool collision_time_flag = Universal::JudgementCollisionTime(actor);

	// 1フレーム前からコリジョンフラグが変化していたら
	if (old_collision_time_flag != collision_time_flag)
	{
		// コリジョンのあたり判定のオンオフを切り替える
		std::string name = actor->GetName();
		name += "Head";
		collision_manager.GetCollisionSphereFromName(name.c_str())->SetCollisionFlag(collision_time_flag);
	}
	// 前フレームのコリジョンフラグを代入。コリジョンフラグが変化したタイミングを調べる用
	owner->GetCharactor()->SetOldCollisionTimeFlag(collision_time_flag);

	// アニメーション再生が終了したら完了を返す
	if (!actor->GetModel()->IsPlayAnimation())
	{
		owner->SetRunTimer(0.0f);
		owner->SetAttackFlag(false);
		return ActionBase::State::Complete;
	}
	return ActionBase::State::Run;
}

