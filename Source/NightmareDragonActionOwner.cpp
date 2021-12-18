#include "NightmareDragonActionOwner.h"
#include "Mathf.h"
#include "ActorManager.h"
#include "Collision.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "GameDatabase.h"
#include "Charactor.h"

//********************************
// 
// 睡眠アクション
// 
// *******************************
// -----------------------------
// 実行前処理
//------------------------------
void SleepAction::Start(std::string action_name)
{
	std::string owner_name = owner->GetName();
	// アニメーションデータベースからアニメーションデータを取得
	std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData(owner_name + "Sleep");
	// アニメーションデータを取得出来たら再生
	if (animation != nullptr)
	{
		owner->PlayAnimation(animation);
	}
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