#include "PLTActionOwner.h"
#include "GameDatabase.h"
#include "Enemy.h"
#include "Charactor.h"
#include "Model.h"

#include "Universal.h"
//---------------------------
// パンチ攻撃アニメーション再生
//---------------------------
void BlowAttackAction::Start()
{
	// アニメーション再生
	owner->PlayAnimation("PLTAttack");

	// 当たり判定を行う時間のデータを取得
	collision_time_data = GameDataBase::Instance().GetAttackCollitionTimeData(AttackCategory::PunchAttack, EnemyCategory::PLT);
}

//---------------------------
// パンチ攻撃行動
//---------------------------
ActionBase::State BlowAttackAction::Run(float elapsed_time)
{
	std::shared_ptr<Actor> actor = owner->GetActor();

	// 攻撃時の当たり判定
	std::string collision_name = actor->GetName();
	collision_name += "RightHand";
	AttackCollision(actor, collision_name.c_str(), collision_time_data);

	// アニメーション再生が終了したら完了
	if (!owner->GetActor()->GetModel()->IsPlayAnimation())
	{
		owner->SetRunTimer(0.0f);
		owner->SetAttackFlag(false);
		return ActionBase::State::Complete;
	}
	return ActionBase::State::Run;
}
