#include "ActionOwner.h"
#include "Mathf.h"
#include "ActorManager.h"
#include "Collision.h"
#include "Enemy.h"
#include "EnemyManager.h"

#include "GameDatabase.h"
#include "Charactor.h"

#include "Model.h"

//*****************************
// 
// ����ʒu�ֈړ�
// 
//*****************************
//----------------------------
// ���s�O����
//----------------------------
void MoveToTargetAction::Start()
{
	std::string owner_name = owner->GetName();
	owner_name += "Run";

	// �A�j���[�V�����Đ�
	owner->PlayAnimation(owner_name.c_str());

	// �ڕW�n�_��ݒ�
	owner->SetTargetPosition(target_position);
}

//----------------------------
// ���s����
//----------------------------
ActionBase::State MoveToTargetAction::Run(float elapsed_time)
{
	// �A�N�^�[�擾
	std::shared_ptr<Actor> owner_actor = owner->GetActor();

	// �ړI�n�֒��������̔��菈��
	if (JedgmentToTargetPosition(owner->GetTargetPosition(), owner_actor->GetPosition(), owner_actor->GetName()))
	{
		// ������I��
		return ActionBase::State::Complete;
	}

	// �ړI�n�_�ֈړ�
	owner->MoveToTarget(elapsed_time, 1.5f);

	// ���s��
	return ActionBase::State::Run;
}

//***********************************
// 
// �ҋ@�s��
// 
//***********************************
//----------------------------------
// ���s�O����
//----------------------------------
void IdleAction::Start()
{
	std::string owner_name = owner->GetName();
	owner_name +="Idle";

	// �A�j���[�V�����Đ�
	owner->PlayAnimation(owner_name.c_str());
}

//----------------------------------
// ���s����
//----------------------------------
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
//----------------------------------
// ���s�O����
//----------------------------------
void DamageAction::Start()
{
	std::string animation_name = owner->GetName();
	animation_name += "Damage";

	// �A�j���[�V�����Đ�
	owner->PlayAnimation(animation_name.c_str());
}

//----------------------------------
// ���s����
//----------------------------------
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
//----------------------------------
// ���s�O����
//----------------------------------
void DeathAction::Start()
{
	// ���S�A�j���[�V�����Đ�
	std::string animation_name = owner->GetName();
	animation_name += "Death";

	// �A�j���[�V�����Đ�
	owner->PlayAnimation(animation_name.c_str());
}

//----------------------------------
// ���s����
//----------------------------------
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
//----------------------------
// ���s�O����
//----------------------------
void WanderAction::Start()
{
	std::string animation_name = owner->GetName();
	animation_name += "Walk";

	// �A�j���[�V�����Đ�
	owner->PlayAnimation(animation_name.c_str());
}

//----------------------------
// ���s����
//----------------------------
ActionBase::State WanderAction::Run(float elapsed_time)
{
	// �A�N�^�[�擾
	std::shared_ptr<Actor> owner_actor = owner->GetActor();

	// �ړI�n�֒��������̔��菈��
	if (JedgmentToTargetPosition(owner->GetTargetPosition(), owner_actor->GetPosition(), owner_actor->GetName()))
	{
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
//----------------------------------
// ���s�O����
//----------------------------------
void LeaveAction::Start()
{
	// �A�N�^�[�擾
	std::shared_ptr<Actor> owner_actor = owner->GetActor();
	
	// �A�j���[�V��������ݒ肵��
	std::string animation_name = owner->GetName();
	animation_name += "Walk";

	// �A�j���[�V�����Đ�
	owner->PlayAnimation(animation_name.c_str());

	// �ڕW�n�_���v���C���[�Ɛ����΂̃x�N�g���~5�̈ʒu�Ɏw��
	DirectX::XMVECTOR start_position = DirectX::XMLoadFloat3(&ActorManager::Instance().GetActor("Player")->GetPosition());
	DirectX::XMVECTOR end_position = DirectX::XMLoadFloat3(&owner_actor->GetPosition());

	DirectX::XMVECTOR vec_target_position = DirectX::XMVectorSubtract(end_position, start_position);
	vec_target_position = DirectX::XMVector3Normalize(vec_target_position);
	vec_target_position = DirectX::XMVectorScale(vec_target_position, 5.0f);

	DirectX::XMFLOAT3 target_position;
	DirectX::XMStoreFloat3(&target_position, vec_target_position);
	DirectX::XMFLOAT3 owner_position = owner_actor->GetPosition();
	target_position.x += owner_position.x;
	target_position.y += owner_position.y;
	target_position.z += owner_position.z;
	owner->SetTargetPosition(target_position);
}

//----------------------------------
// ���s����
//----------------------------------
ActionBase::State LeaveAction::Run(float elapsed_time)
{
	std::shared_ptr<Actor> owner_actor = owner->GetActor();

	// �ړI�n�_�ֈړ�
	owner->MoveToTarget(elapsed_time, 1.0);

	// �ړI�n�֒�������
	if (JedgmentToTargetPosition(owner->GetTargetPosition(), owner_actor->GetPosition(), owner_actor->GetName()))
	{
		// �R���v���[�g
		return ActionBase::State::Complete;
	}

	// �p��
	return ActionBase::State::Run;
}

//***********************************
// 
// �񕜍s��
// 
//***********************************
//----------------------------------
// ���s�O����
//----------------------------------
void RecoverAction::Start()
{
	std::string animation_name = owner->GetName();;
	animation_name += "Idle";

	// �A�j���[�V�����Đ�
	owner->PlayAnimation(animation_name.c_str());

}

//----------------------------------
// ���s����
//----------------------------------
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
//----------------------------------
// ���s�O����
//----------------------------------
void PursuitAction::Start()
{
	std::string animation_name = owner->GetName();;
	animation_name += "Walk";

	// �A�j���[�V�����Đ�
	owner->PlayAnimation(animation_name.c_str());
}

//----------------------------------
// ���s����
//----------------------------------
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
void SleepAction::Start()
{
	std::string owner_name = owner->GetName();
	owner_name += "Sleep";

	// �A�j���[�V�����Đ�
	owner->PlayAnimation(owner_name.c_str());
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