#include "BossJudgmenetOwner.h"
#include "ActorManager.h"
#include "Enemy.h"

//************************************
// 
//		JumpAttackNode‚É‘JˆÚ‚Å‚«‚é‚©”»’è
// 
//************************************
//--------------------------------
// ”»’è
//--------------------------------
bool JumpAttackJudgmenet::Judgment()
{

	// ‘ÎÛ‚Æ‚Ì‹——£‚ðŽZo
	DirectX::XMFLOAT3 position = owner->GetActor()->GetPosition();
	DirectX::XMFLOAT3 target_position = ActorManager::Instance().GetActor("Player")->GetPosition();

	float vx = target_position.x - position.x;
	float vy = target_position.y - position.y;
	float vz = target_position.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);

	if (dist < owner->GetAttackRange())
	{
		// AttackNode‚Ö‘JˆÚ‚Å‚«‚é
		return true;
	}
	return false;
}

//************************************
// 
//		PunchAttackNode‚É‘JˆÚ‚Å‚«‚é‚©”»’è
// 
//************************************
//--------------------------------
// ”»’è
//--------------------------------
bool PunchAttackJudgment::Judgment()
{
	return false;
}

//************************************
// 
//		PunchAttackNode‚É‘JˆÚ‚Å‚«‚é‚©”»’è
// 
//************************************
//--------------------------------
// ”»’è
//--------------------------------
bool SwipingAttackJudgmenet::Judgment()
{
	return false;
}
