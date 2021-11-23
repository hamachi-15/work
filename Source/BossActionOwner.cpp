#include "BossActionOwner.h"
#include "Enemy.h"
#include "GameDatabase.h"
//*********************************
// 
//			強打攻撃行動
// 
//*********************************
//-----------------------------------------------
// 開始前処理
//-----------------------------------------------
void SwipingAttackAction::Start(std::string action_name)
{
	// アニメーション再生
	std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData(owner->GetName() + action_name);
	owner->PlayAnimation(animation);

}

ActionBase::State SwipingAttackAction::Run(float elapsed_time)
{
	Model* model = owner->GetActor()->GetModel();
	float animation_time = model->GetCurrentAnimationSeconds();
	bool attack_collision_flag = animation_time >= 0.3f && animation_time <= 0.4f;
	CollisionManager& sphere = CollisionManager::Instance();

	std::shared_ptr<CollisionSphere> left_hand = sphere.GetCollisionSphereFromName("Mutant:RightHandIndex1");
	// 任意のアニメーション区間でのみあたり判定をする
	if (attack_collision_flag)
	{
		// あたり判定をするよう変更
		sphere.GetCollisionSphereFromName("Mutant:LeftHand")->SetCollisionFlag(true);
	}

	// アニメーション再生終了でノードから抜ける
	if (!model->IsPlayAnimation())
	{
		sphere.GetCollisionSphereFromName("Mutant:LeftHand")->SetCollisionFlag(false);
		return ActionBase::State::Complete;
	}
	return ActionBase::State::Run;
}

void PunchAttackAction::Start(std::string action_name)
{
}

ActionBase::State PunchAttackAction::Run(float elapsed_time)
{
	return ActionBase::State();
}
