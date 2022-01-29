#include "JudgmentOwner.h"
#include "Enemy.h"
#include "Charactor.h"
#include "Telegram.h"
#include "MetaAI.h"
#include "Mathf.h"

#include "ActorManager.h"
#include "CollisionManager.h"

//***********************************
// 
// �m�[�h�ɑJ�ڂ����邩�����_���ɔ���
// 
//***********************************
//-----------------------------------------
// ����
//-----------------------------------------
bool RamdamNextStepJudgment::Judgment()
{
	return (Mathf::RandomRange(1, 100) <= percent);
}

//*****************************
// 
// BattleNode�ɑJ�ڂł��邩����
// 
//*****************************
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

//*****************************
// 
// AttackNode�ɑJ�ڂł��邩����
// 
//*****************************
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

//*****************************
// 
// WanderNode�ɑJ�ڂł��邩����
// 
//*****************************
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

//*****************************
// 
// DamageNode�ɑJ�ڂł��邩����
// 
//*****************************
//-----------------------------------------
// ����
//-----------------------------------------
bool DamageJudgment::Judgment()
{
	std::shared_ptr<Charactor> charactor = owner->GetCharactor();
	// �_���[�W�t���O�������Ă��銎���G���Ԃ��ݒ肳��Ă��Ȃ����true
	return (charactor->GetDamageFlag() && charactor->GetInvincibleTimer() <= 0.0f);
}

//*****************************
// 
// DeathNode�ɑJ�ڂł��邩����
// 
//*****************************
//-----------------------------------------
// ����
//-----------------------------------------
bool DeathJudgment::Judgment()
{
	return owner->GetCharactor()->GetDeathFlag();
}

//*****************************
// 
// Escape�ɑJ�ڂł��邩����
// 
//*****************************
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
