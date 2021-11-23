#include "JudgmentOwner.h"
#include "ActorManager.h"
#include "Enemy.h"
#include "Charactor.h"
#include "Telegram.h"
#include "MetaAI.h"
//-----------------------------------------
// ����
//-----------------------------------------
bool BattleJudgment::Judgment()
{
	// �v���C���[�������邩
	if (owner->SearchPlayer())
	{
		return true;
	}
	return false;
}

//-----------------------------------------
// ����
//-----------------------------------------
bool AttackJudgment::Judgment()
{
	// �ΏۂƂ̋������Z�o
	DirectX::XMFLOAT3 position = owner->GetActor()->GetPosition();
	DirectX::XMFLOAT3 targetPosition = ActorManager::Instance().GetActor("Player")->GetPosition();

	float vx = targetPosition.x - position.x;
	float vy = targetPosition.y - position.y;
	float vz = targetPosition.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);

	// �v���C���[�Ƃ̋������U���͈͂��Z��������
	if (dist < owner->GetAttackRange())
	{
		// ���^AI�֍U������v��
		Message message;
		message.message = MessageType::Message_Ask_Attack_Right;
		MetaAI::Instance().SendMessaging(owner->GetCharactor()->GetID() + owner->GetIdentity(), static_cast<int>(MetaAI::Identity::Meta), message);
		if (owner->GetAttackFlag())
		{
			// AttackNode�֑J�ڂł���
			return true;
		}
	}
	return false;
}

//-----------------------------------------
// ����
//-----------------------------------------
bool WanderJudgment::Judgment()
{
	std::shared_ptr<Actor> actor = owner->GetActor();
	// �ړI�n�_�܂ł�XZ���ʂł̋�������
	DirectX::XMFLOAT3 position = actor->GetPosition();
	DirectX::XMFLOAT3 targetPosition = owner->GetTargetPosition();
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float distSq = vx * vx + vz * vz;

	// �ړI�n���痣��Ă���ꍇ
	std::shared_ptr<CollisionCylinder> collision = CollisionManager::Instance().GetCollisionCylinderFromName(actor->GetName());
	float radius = collision->GetRadius();
	if (distSq > radius * radius)
	{
		return true;
	}

	return false;
}

//-----------------------------------------
// ����
//-----------------------------------------
bool DamageJudgment::Judgment()
{
	return owner->GetCharactor()->GetDamageFlag();
}

//-----------------------------------------
// ����
//-----------------------------------------
bool DeathJudgment::Judgment()
{
	return owner->GetCharactor()->GetDeathFlag();
}

//-----------------------------------------
// ����
//-----------------------------------------
bool EscapeJudgment::Judgment()
{
	if (owner->GetCharactor()->GetHealth() < (owner->GetCharactor()->GetMaxHealth() * 0.5))
	{
		return true;
	}
	return false;
}

