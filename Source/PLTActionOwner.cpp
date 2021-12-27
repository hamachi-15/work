#include "PLTActionOwner.h"
#include "GameDatabase.h"
#include "Enemy.h"
#include "Charactor.h"
#include "Model.h"
//---------------------------
// パンチ攻撃アニメーション再生
//---------------------------
void BlowAttackAction::Start(std::string action_name)
{
	// 攻撃アニメーション再生
	std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("PLTAttack");
	owner->PlayAnimation(animation);
}

//---------------------------
// パンチ攻撃行動
//---------------------------
ActionBase::State BlowAttackAction::Run(float elapsed_time)
{
	CollisionManager& collision_manager = CollisionManager::Instance();
	std::shared_ptr<Actor> actor = owner->GetActor();
	// 現在のコリジョンフラグと1フレーム前のコリジョンフラグを取得
	bool old_collision_time_flag = owner->GetCharactor()->GetOldCollisionTimeFlag();
	
	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	bool collision_time_flag = owner->GetCharactor()->SearchAnimationTime(actor, 1.4f, 2.3f);
	
	// 1フレーム前からコリジョンフラグが変化していたら
	if (old_collision_time_flag != collision_time_flag)
	{
		// コリジョンのあたり判定のオンオフを切り替える
		std::string name = actor->GetName();
		name += "RightHand";
		collision_manager.GetCollisionSphereFromName(name.c_str())->SetCollisionFlag(collision_time_flag);
	}
	// 前フレームのコリジョンフラグを代入。コリジョンフラグが変化したタイミングを調べる用
	owner->GetCharactor()->SetOldCollisionTimeFlag(collision_time_flag);

	// アニメーション再生が終了したら完了
	if (!owner->GetActor()->GetModel()->IsPlayAnimation())
	{
		owner->SetRunTimer(0.0f);
		owner->SetAttackFlag(false);
		return ActionBase::State::Complete;
	}
	return ActionBase::State::Run;
}
