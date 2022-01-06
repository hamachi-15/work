#include "ActionOwner.h"
#include "Mathf.h"
#include "ActorManager.h"
#include "Collision.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "GameDatabase.h"
#include "Charactor.h"

#include "Model.h"

//***********************************
// 
// �ҋ@�s��
// 
//***********************************
//-----------------------------------
// �ҋ@�s���O����
//-----------------------------------
void IdleAction::Start(std::string action_name)
{
	std::string owner_name = owner->GetName();
	// �A�j���[�V�����f�[�^�x�[�X����A�j���[�V�����f�[�^���擾
	std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData(owner_name +"Idle");
	// �A�j���[�V�����f�[�^���擾�o������Đ�
	if (animation != nullptr)
	{
		owner->PlayAnimation(animation);
	}
}

ActionBase::State IdleAction::Run(float elapsed_time)
{
	float run_timer = owner->GetRunTimer();
	if (run_timer <= 0.0f)
	{
		owner->SetRunTimer(Mathf::RandomRange(3.0f, 5.0f));
		run_timer = owner->GetRunTimer();
	}
	else
	{
		run_timer = owner->GetRunTimer() - elapsed_time;
	}

	owner->SetRunTimer(run_timer);

	if (run_timer <= 0.0f)
	{
		owner->SetRandomTargetPosition();
		owner->SetRunTimer(0.0f);
		return ActionBase::State::Complete;
	}
	if (owner->SearchPlayer())
	{
		owner->SetRunTimer(0.0f);
		return ActionBase::State::Complete;
	}
	return ActionBase::State::Run;
}

//***********************************
// 
// �_���[�W�s��
// 
//***********************************
void DamageAction::Start(std::string action_name)
{
	std::string animation_name = owner->GetName();
	// �A�j���[�V�����f�[�^�x�[�X����A�j���[�V�����f�[�^���擾
	std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData(animation_name + "Damage");
	// �A�j���[�V�����f�[�^���擾�o������Đ�
	if (animation != nullptr)
	{
		owner->PlayAnimation(animation);
	}
}

ActionBase::State DamageAction::Run(float elapsed_time)
{
	// �_���[�W��^���A���S���Ȃ����
	if (owner->GetCharactor()->ApplyDamage(1, 0.7f))
	{// �m�b�N�o�b�N����
		float power = 10.0f;
		DirectX::XMVECTOR position = DirectX::XMLoadFloat3(&owner->GetActor()->GetPosition());
		DirectX::XMVECTOR vector_weppon_position = DirectX::XMLoadFloat3(&owner->GetHitPosition());
		DirectX::XMVECTOR normalize_vector = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(position, vector_weppon_position));
		DirectX::XMFLOAT3 float_normalize_vector;
		DirectX::XMStoreFloat3(&float_normalize_vector, normalize_vector);
		DirectX::XMFLOAT3 impulse;
		impulse.x = float_normalize_vector.x * power;
		impulse.y = power * 0.5f;
		impulse.z = float_normalize_vector.z * power;
		owner->GetMovement()->AddImpulse(impulse);
	}
	float run_timer = owner->GetRunTimer();
	if (run_timer <= 0.0f)
	{
		owner->SetRunTimer(0.5f);
		run_timer = owner->GetRunTimer();
	}
	else
	{
		// �^�C�}�[����
		run_timer = owner->GetRunTimer() - elapsed_time;
	}

	//// �^�C�}�[�X�V
	owner->SetRunTimer(run_timer);
	

	if (run_timer <= 0.0f)
	{
		owner->SetRandomTargetPosition();
		owner->SetRunTimer(0.0f);
		owner->GetCharactor()->SetDamageFlag(false);
		return ActionBase::State::Complete;
	}

	return ActionBase::State::Run;
}

//***********************************
// 
// ���S�s��
// 
//***********************************
void DeathAction::Start(std::string action_name)
{
	// ���S�A�j���[�V�����Đ�
	std::string animation_name = owner->GetName();
	// �A�j���[�V�����f�[�^�x�[�X����A�j���[�V�����f�[�^���擾
	std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData(animation_name + "Death");
	// �A�j���[�V�����f�[�^���擾�o������Đ�
	if (animation != nullptr)
	{
		owner->PlayAnimation(animation);
	}
}

ActionBase::State DeathAction::Run(float elapsed_time)
{
	// �A�j���[�V�����Đ����I���΃A�N�^�[��j��
	if (!owner->GetActor()->GetModel()->IsPlayAnimation())
	{
		owner->Destroy();
		return ActionBase::State::Complete;
	}
	return ActionBase::State::Run;
}

//***********************************
// 
// �p�j�s��
// 
//***********************************
void WanderAction::Start(std::string action_name)
{
	std::string animation_name = owner->GetName();
	// �A�j���[�V�����f�[�^�x�[�X����A�j���[�V�����f�[�^���擾
	std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData(animation_name + "Walk");
	// �A�j���[�V�����f�[�^���擾�o������Đ�
	if (animation != nullptr)
	{
		owner->PlayAnimation(animation);
	}
}

ActionBase::State WanderAction::Run(float elapsed_time)
{
	std::shared_ptr<Actor> owner_actor = owner->GetActor();
	// �ړI�n�_�܂ł�XZ���ʂł̋�������
	DirectX::XMFLOAT3 position = owner_actor->GetPosition();
	DirectX::XMFLOAT3 target_position = owner->GetTargetPosition();
	float vx = target_position.x - position.x;
	float vz = target_position.z - position.z;
	float distSq = vx * vx + vz * vz;

	// �ړI�n�֒�����
	std::shared_ptr<CollisionCylinder> collision = CollisionManager::Instance().GetCollisionCylinderFromName(owner_actor->GetName());
	float radius = collision->GetRadius();
	if (distSq < radius * radius)
	{
		owner->SetRunTimer(0.0f);
		return ActionBase::State::Complete;
	}

	// �ړI�n�_�ֈړ�
	owner->MoveToTarget(elapsed_time, 0.5f);

	// �v���C���[���G����������
	if (owner->SearchPlayer())
	{
		owner->SetRunTimer(0.0f);
		return ActionBase::State::Complete;
	}

	// ���s��
	return ActionBase::State::Run;
}

//***********************************
// 
// �����s��
// 
//***********************************
void LeaveAction::Start(std::string action_name)
{
	std::string animation_name = owner->GetName();
	// �A�j���[�V�����f�[�^�x�[�X����A�j���[�V�����f�[�^���擾
	std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData(animation_name + "Walk");
	// �A�j���[�V�����f�[�^���擾�o������Đ�
	if (animation != nullptr)
	{
		owner->PlayAnimation(animation);
	}
}

ActionBase::State LeaveAction::Run(float elapsed_time)
{
	DirectX::XMFLOAT3 target_position;
	std::shared_ptr<Actor> owner_actor = owner->GetActor();
	static int i = 0;
	switch (i)
	{
	case 0:

		// �ڕW�n�_���v���C���[�Ɛ����΂̃x�N�g���~5�̈ʒu�Ɏw��
		DirectX::XMVECTOR start_position = DirectX::XMLoadFloat3(&ActorManager::Instance().GetActor("Player")->GetPosition());
		DirectX::XMVECTOR end_position = DirectX::XMLoadFloat3(&owner_actor->GetPosition());

		DirectX::XMVECTOR vec_target_position = DirectX::XMVectorSubtract(end_position, start_position);
		vec_target_position = DirectX::XMVector3Normalize(vec_target_position);
		vec_target_position = DirectX::XMVectorScale(vec_target_position, 5.0f);


		DirectX::XMStoreFloat3(&target_position, vec_target_position);
		DirectX::XMFLOAT3 owner_position = owner_actor->GetPosition();
		target_position.x += owner_position.x;
		target_position.y += owner_position.y;
		target_position.z += owner_position.z;
		owner->SetTargetPosition(target_position);
		i++;
		break;

	case 1:

		break;
	}

	// �ړI�n�_�ֈړ�
	owner->MoveToTarget(elapsed_time, 1.0);

	DirectX::XMFLOAT3 position = owner_actor->GetPosition();
	target_position = owner->GetTargetPosition();

	float vx = target_position.x - position.x;
	float vz = target_position.z - position.z;
	float distSq = vx * vx + vz * vz;

	// �ړI�n�֒�����
	std::shared_ptr<CollisionCylinder> collision = CollisionManager::Instance().GetCollisionCylinderFromName(owner_actor->GetName());
	float radius = collision->GetRadius();
	if (distSq < radius * radius)
	{
		i = 0;
		return ActionBase::State::Complete;
	}

	return ActionBase::State::Run;
}

void RecoverAction::Start(std::string action_name)
{
	std::string animation_name = owner->GetName();;
	// �A�j���[�V�����f�[�^�x�[�X����A�j���[�V�����f�[�^���擾
	std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData(animation_name + "Idle");
	// �A�j���[�V�����f�[�^���擾�o������Đ�
	if (animation != nullptr)
	{
		owner->PlayAnimation(animation);
	}
}

ActionBase::State RecoverAction::Run(float elapsed_time)
{
	float runTimer = owner->GetRunTimer();
	if (runTimer <= 0.0f)
	{
		owner->SetRunTimer(3.0f);
		runTimer = owner->GetRunTimer();
	}
	else
	{
		// �^�C�}�[����
		runTimer = owner->GetRunTimer() - elapsed_time;
	}

	// �^�C�}�[�X�V
	owner->SetRunTimer(runTimer);

	if (runTimer <= 0.0f) {
		owner->GetCharactor()->SetHealth(owner->GetCharactor()->GetMaxHealth());
		owner->SetRunTimer(0.0f);
		return ActionBase::State::Complete;
	}

	return ActionBase::State::Run;
}

//***********************************
// 
// �ǐՍs��
// 
//***********************************
void PursuitAction::Start(std::string action_name)
{
	std::string animation_name = owner->GetName();;
	// �A�j���[�V�����f�[�^�x�[�X����A�j���[�V�����f�[�^���擾
	std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData(animation_name + "Walk");
	// �A�j���[�V�����f�[�^���擾�o������Đ�
	if (animation != nullptr)
	{
		owner->PlayAnimation(animation);
	}
}

ActionBase::State PursuitAction::Run(float elapsed_time)
{
	float run_timer = owner->GetRunTimer();
	if (run_timer <= 0.0f)
	{
		owner->SetRunTimer(Mathf::RandomRange(3.0f, 5.0f));
		run_timer = owner->GetRunTimer();
	}
	else
	{
		// �^�C�}�[����
		run_timer = owner->GetRunTimer() - elapsed_time;
	}

	// �^�C�}�[�X�V
	owner->SetRunTimer(run_timer);

	// �ڕW�n�_���v���C���[�ʒu�ɐݒ�
	owner->SetTargetPosition(ActorManager::Instance().GetActor("Player")->GetPosition());

	// �ړI�n�_�ֈړ�
	owner->MoveToTarget(elapsed_time, 1.0f);

	DirectX::XMFLOAT3 position = owner->GetActor()->GetPosition();
	DirectX::XMFLOAT3 target_position = owner->GetTargetPosition();

	float vx = target_position.x - position.x;
	float vy = target_position.y - position.y;
	float vz = target_position.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);

	if (dist < owner->GetAttackRange())
	{
		owner->SetRunTimer(0.0f);
		return ActionBase::State::Complete;
	}
	if (run_timer <= 0.0f)
	{
		owner->SetRunTimer(0.0f);
		return ActionBase::State::Failed;
	}

	return ActionBase::State::Run;
}

//********************************
// 
// �����s��
// 
// *******************************
// -----------------------------
// ���s�O����
//------------------------------
void SleepAction::Start(std::string action_name)
{
	std::string owner_name = owner->GetName();
	// �A�j���[�V�����f�[�^�x�[�X����A�j���[�V�����f�[�^���擾
	std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData(owner_name + "Sleep");
	// �A�j���[�V�����f�[�^���擾�o������Đ�
	if (animation != nullptr)
	{
		owner->PlayAnimation(animation);
	}
}

//------------------------------
// ���s����
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

//***************************************
// 
// ���K�s��
// 
//***************************************
// -----------------------------
// ���s�O����
//------------------------------

//------------------------------
// ���s����
//------------------------------
