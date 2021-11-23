#include "JudgmentOwner.h"
#include "ActorManager.h"
#include "Enemy.h"
#include "Charactor.h"
#include "Telegram.h"
#include "MetaAI.h"
//-----------------------------------------
// 判定
//-----------------------------------------
bool BattleJudgment::Judgment()
{
	// プレイヤーが見つかるか
	if (owner->SearchPlayer())
	{
		return true;
	}
	return false;
}

//-----------------------------------------
// 判定
//-----------------------------------------
bool AttackJudgment::Judgment()
{
	// 対象との距離を算出
	DirectX::XMFLOAT3 position = owner->GetActor()->GetPosition();
	DirectX::XMFLOAT3 targetPosition = ActorManager::Instance().GetActor("Player")->GetPosition();

	float vx = targetPosition.x - position.x;
	float vy = targetPosition.y - position.y;
	float vz = targetPosition.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);

	// プレイヤーとの距離が攻撃範囲より短かったら
	if (dist < owner->GetAttackRange())
	{
		// メタAIへ攻撃権を要求
		Message message;
		message.message = MessageType::Message_Ask_Attack_Right;
		MetaAI::Instance().SendMessaging(owner->GetCharactor()->GetID() + owner->GetIdentity(), static_cast<int>(MetaAI::Identity::Meta), message);
		if (owner->GetAttackFlag())
		{
			// AttackNodeへ遷移できる
			return true;
		}
	}
	return false;
}

//-----------------------------------------
// 判定
//-----------------------------------------
bool WanderJudgment::Judgment()
{
	std::shared_ptr<Actor> actor = owner->GetActor();
	// 目的地点までのXZ平面での距離判定
	DirectX::XMFLOAT3 position = actor->GetPosition();
	DirectX::XMFLOAT3 targetPosition = owner->GetTargetPosition();
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float distSq = vx * vx + vz * vz;

	// 目的地から離れている場合
	std::shared_ptr<CollisionCylinder> collision = CollisionManager::Instance().GetCollisionCylinderFromName(actor->GetName());
	float radius = collision->GetRadius();
	if (distSq > radius * radius)
	{
		return true;
	}

	return false;
}

//-----------------------------------------
// 判定
//-----------------------------------------
bool DamageJudgment::Judgment()
{
	return owner->GetCharactor()->GetDamageFlag();
}

//-----------------------------------------
// 判定
//-----------------------------------------
bool DeathJudgment::Judgment()
{
	return owner->GetCharactor()->GetDeathFlag();
}

//-----------------------------------------
// 判定
//-----------------------------------------
bool EscapeJudgment::Judgment()
{
	if (owner->GetCharactor()->GetHealth() < (owner->GetCharactor()->GetMaxHealth() * 0.5))
	{
		return true;
	}
	return false;
}

