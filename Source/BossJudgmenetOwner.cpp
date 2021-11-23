#include "BossJudgmenetOwner.h"
#include "ActorManager.h"
#include "Enemy.h"

//************************************
// 
//		JumpAttackNode�ɑJ�ڂł��邩����
// 
//************************************
//--------------------------------
// ����
//--------------------------------
bool JumpAttackJudgmenet::Judgment()
{

	// �ΏۂƂ̋������Z�o
	DirectX::XMFLOAT3 position = owner->GetActor()->GetPosition();
	DirectX::XMFLOAT3 target_position = ActorManager::Instance().GetActor("Player")->GetPosition();

	float vx = target_position.x - position.x;
	float vy = target_position.y - position.y;
	float vz = target_position.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);

	if (dist < owner->GetAttackRange())
	{
		// AttackNode�֑J�ڂł���
		return true;
	}
	return false;
}

//************************************
// 
//		PunchAttackNode�ɑJ�ڂł��邩����
// 
//************************************
//--------------------------------
// ����
//--------------------------------
bool PunchAttackJudgment::Judgment()
{
	return false;
}

//************************************
// 
//		PunchAttackNode�ɑJ�ڂł��邩����
// 
//************************************
//--------------------------------
// ����
//--------------------------------
bool SwipingAttackJudgmenet::Judgment()
{
	return false;
}
