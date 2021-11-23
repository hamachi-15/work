#include "BossJudgmenetOwner.h"
#include "ActorManager.h"
#include "Enemy.h"

//************************************
// 
//		JumpAttackNodeに遷移できるか判定
// 
//************************************
//--------------------------------
// 判定
//--------------------------------
bool JumpAttackJudgmenet::Judgment()
{

	// 対象との距離を算出
	DirectX::XMFLOAT3 position = owner->GetActor()->GetPosition();
	DirectX::XMFLOAT3 target_position = ActorManager::Instance().GetActor("Player")->GetPosition();

	float vx = target_position.x - position.x;
	float vy = target_position.y - position.y;
	float vz = target_position.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);

	if (dist < owner->GetAttackRange())
	{
		// AttackNodeへ遷移できる
		return true;
	}
	return false;
}

//************************************
// 
//		PunchAttackNodeに遷移できるか判定
// 
//************************************
//--------------------------------
// 判定
//--------------------------------
bool PunchAttackJudgment::Judgment()
{
	return false;
}

//************************************
// 
//		PunchAttackNodeに遷移できるか判定
// 
//************************************
//--------------------------------
// 判定
//--------------------------------
bool SwipingAttackJudgmenet::Judgment()
{
	return false;
}
