#include "JudgmentOwner.h"
#include "ActorManager.h"
#include "Enemy.h"
#include "Charactor.h"
#include "Telegram.h"
#include "MetaAI.h"
#include "Mathf.h"

//***********************************
// 
// ƒm[ƒh‚É‘JˆÚ‚³‚¹‚é‚©ƒ‰ƒ“ƒ_ƒ€‚É”»’è
// 
//***********************************
//-----------------------------------------
// ”»’è
//-----------------------------------------
bool RamdamNextStepJudgment::Judgment()
{
	return (Mathf::RandomRange(1, 100) <= percent);
}

//*****************************
// 
// BattleNode‚É‘JˆÚ‚Å‚«‚é‚©”»’è
// 
//*****************************
//-----------------------------------------
// ”»’è
//-----------------------------------------
bool BattleJudgment::Judgment()
{
	// ƒvƒŒƒCƒ„[‚ªŒ©‚Â‚©‚é‚©
	if (owner->SearchPlayer())
	{
		return true;
	}
	return false;
}

//*****************************
// 
// AttackNode‚É‘JˆÚ‚Å‚«‚é‚©”»’è
// 
//*****************************
//-----------------------------------------
// ”»’è
//-----------------------------------------
bool AttackJudgment::Judgment()
{
	// ‘ÎÛ‚Æ‚Ì‹——£‚ğZo
	DirectX::XMFLOAT3 position = owner->GetActor()->GetPosition();
	DirectX::XMFLOAT3 targetPosition = ActorManager::Instance().GetActor("Player")->GetPosition();

	float vx = targetPosition.x - position.x;
	float vy = targetPosition.y - position.y;
	float vz = targetPosition.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);

	// ƒvƒŒƒCƒ„[‚Æ‚Ì‹——£‚ªUŒ‚”ÍˆÍ‚æ‚è’Z‚©‚Á‚½‚ç
	if (dist < owner->GetAttackRange())
	{
		// ƒƒ^AI‚ÖUŒ‚Œ ‚ğ—v‹
		Message message;
		message.message = MessageType::Message_Ask_Attack_Right;
		MetaAI::Instance().SendMessaging(owner->GetCharactor()->GetID() + owner->GetIdentity(), static_cast<int>(MetaAI::Identity::Meta), message);
		if (owner->GetAttackFlag())
		{
			// AttackNode‚Ö‘JˆÚ‚Å‚«‚é
			return true;
		}
	}
	return false;
}

//*****************************
// 
// WanderNode‚É‘JˆÚ‚Å‚«‚é‚©”»’è
// 
//*****************************
//-----------------------------------------
// ”»’è
//-----------------------------------------
bool WanderJudgment::Judgment()
{
	std::shared_ptr<Actor> actor = owner->GetActor();
	// –Ú“I’n“_‚Ü‚Å‚ÌXZ•½–Ê‚Å‚Ì‹——£”»’è
	DirectX::XMFLOAT3 position = actor->GetPosition();
	DirectX::XMFLOAT3 targetPosition = owner->GetTargetPosition();
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float distSq = vx * vx + vz * vz;

	// –Ú“I’n‚©‚ç—£‚ê‚Ä‚¢‚éê‡
	std::shared_ptr<CollisionCylinder> collision = CollisionManager::Instance().GetCollisionCylinderFromName(actor->GetName());
	float radius = collision->GetRadius();
	if (distSq > radius * radius)
	{
		return true;
	}

	return false;
}

//*****************************
// 
// DamageNode‚É‘JˆÚ‚Å‚«‚é‚©”»’è
// 
//*****************************
//-----------------------------------------
// ”»’è
//-----------------------------------------
bool DamageJudgment::Judgment()
{
	return owner->GetCharactor()->GetDamageFlag();
}

//*****************************
// 
// DeathNode‚É‘JˆÚ‚Å‚«‚é‚©”»’è
// 
//*****************************
//-----------------------------------------
// ”»’è
//-----------------------------------------
bool DeathJudgment::Judgment()
{
	return owner->GetCharactor()->GetDeathFlag();
}

//*****************************
// 
// Escape‚É‘JˆÚ‚Å‚«‚é‚©”»’è
// 
//*****************************
//-----------------------------------------
// ”»’è
//-----------------------------------------
bool EscapeJudgment::Judgment()
{
	if (owner->GetCharactor()->GetHealth() < (owner->GetCharactor()->GetMaxHealth() * 0.5))
	{
		return true;
	}
	return false;
}
