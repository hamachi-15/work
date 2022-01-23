#include "GameDatabase.h"
#include "EnemyCategory.h"

#include "DragonActionOwner.h"
#include "Actor.h"
#include "Enemy.h"
#include "Charactor.h"
// �}�l�[�W���[
#include "ActorManager.h"
#include "EnemyManager.h"

#include "Mathf.h"

// �ėp�֐�
#include "Universal.h"
//*****************************
// 
// �����킹�A�N�V����
// 
//*****************************
// -----------------------------
// ���s�O����
// -----------------------------
void AligningAction::Start()
{
	// �A�j���[�V�����Đ�
	owner->PlayAnimation("NightmareDragonDefend");
}

// -----------------------------
// ���s����
// -----------------------------
ActionBase::State AligningAction::Run(float elapsed_time)
{
	return ActionBase::State();
}

//***************************************
// 
// ���K�s��
// 
//***************************************
// -----------------------------
// ���s�O����
//------------------------------
void ScreamAction::Start()
{
	// �p������^�C�}�[�ݒ�
	owner->SetRunTimer(3.0f);

	// �A�j���[�V�������ݒ�
	std::string animation_name = owner->GetName();
	animation_name += "Scream";

	// �A�j���[�V�����Đ�
	owner->PlayAnimation(animation_name.c_str());
}

//------------------------------
// ���s����
//------------------------------
ActionBase::State ScreamAction::Run(float elapsed_time)
{
	// �^�C�}�[�擾
	float run_timer = owner->GetRunTimer();

	// �^�C�}�[����
	run_timer -= elapsed_time;

	// �A�j���[�V�����Đ����I�������犮����Ԃ�
	if (!owner->GetActor()->GetModel()->IsPlayAnimation() && run_timer <= 0.0f)
	{
		owner->SetRunTimer(0.0f);
		return ActionBase::State::Complete;
	}

	// �^�C�}�[�ݒ�
	owner->SetRunTimer(run_timer);

	// �p��
	return ActionBase::State::Run;
}

//*****************************
// 
// �K�[�h�A�N�V����
// 
//*****************************
// -----------------------------
// ���s�O����
// -----------------------------
void DefendAction::Start()
{
	// �A�j���[�V�����Đ�
	owner->PlayAnimation("NightmareDragonDefend");
}

// -----------------------------
// ���s����
// -----------------------------
ActionBase::State DefendAction::Run(float elapsed_time)
{
	owner->SetRunTimer(3.0f);

	float run_timer = owner->GetRunTimer();
	if (run_timer > 0.0f)
	{
		run_timer -= elapsed_time;
	}
	else
	{
		owner->SetRunTimer(0.0f);
		return State::Complete;
	}

	return State::Run;
}

//*****************************
// 
// ���݂��U��
// 
//*****************************
// -----------------------------
// ���s�O����
// -----------------------------
void BasicAttackAction::Start()
{
	// �A�j���[�V�����Đ�
	owner->PlayAnimation("NightmareDragonBasicAttack");

	// �����蔻����s�����Ԃ̃f�[�^���擾
	collision_time_data = GameDataBase::Instance().GetAttackCollitionTimeData(AttackCategory::BitingAttack, EnemyCategory::NightmareDragon);
}

// -----------------------------
// ���s����
// -----------------------------
ActionBase::State BasicAttackAction::Run(float elapsed_time)
{
	// �A�N�^�[�擾
	std::shared_ptr<Actor> actor = owner->GetActor();

	// �U���̓����蔻�菈��
	std::string collision_name = actor->GetName();
	collision_name += "mouth";
	AttackCollision(actor, collision_name.c_str(), collision_time_data);

	// �A�j���[�V�����Đ����I�������犮����Ԃ�
	if (!owner->GetActor()->GetModel()->IsPlayAnimation())
	{
		owner->SetRunTimer(0.0f);
		owner->SetAttackFlag(false);
		return ActionBase::State::Complete;
	}
	return ActionBase::State::Run;
}

//*****************************
// 
// �܍U��
// 
//*****************************
// -----------------------------
// ���s�O����
// -----------------------------
void ClawAttackAction::Start()
{
	// �A�j���[�V�����Đ�
	owner->PlayAnimation("NightmareDragonClawAttack");
	// �����蔻����s�����Ԃ̃f�[�^���擾
	collision_time_data = GameDataBase::Instance().GetAttackCollitionTimeData(AttackCategory::ClawAttack, EnemyCategory::NightmareDragon);
}

// -----------------------------
// ���s����
// -----------------------------
ActionBase::State ClawAttackAction::Run(float elapsed_time)
{
	// �A�N�^�[�擾
	std::shared_ptr<Actor> actor = owner->GetActor();
	
	// �U���̓����蔻�菈��
	std::string collision_name = actor->GetName();
	collision_name += "RightWrist";
	AttackCollision(actor, collision_name.c_str(), collision_time_data);

	// �A�j���[�V�����Đ����I�������犮����Ԃ�
	if (!owner->GetActor()->GetModel()->IsPlayAnimation())
	{
		owner->SetRunTimer(0.0f);
		owner->SetAttackFlag(false);
		return ActionBase::State::Complete;
	}
	return ActionBase::State::Run;
}

//*****************************
// 
// �p�U��
// 
//*****************************
// -----------------------------
// ���s�O����
// -----------------------------
void HornAttackAction::Start()
{
	// �A�j���[�V�����Đ�
	owner->PlayAnimation("NightmareDragonHornAttack");

	// �����蔻����s�����Ԃ̃f�[�^���擾
	collision_time_data = GameDataBase::Instance().GetAttackCollitionTimeData(AttackCategory::HornAttack, EnemyCategory::NightmareDragon);
}

// -----------------------------
// ���s����
// -----------------------------
ActionBase::State HornAttackAction::Run(float elapsed_time)
{
	// �A�N�^�[�擾
	std::shared_ptr<Actor> actor = owner->GetActor();

	// �U���̓����蔻�菈��
	std::string collision_name = actor->GetName();
	collision_name += "Head";
	AttackCollision(actor, collision_name.c_str(), collision_time_data);

	// �A�j���[�V�����Đ����I�������犮����Ԃ�
	if (!owner->GetActor()->GetModel()->IsPlayAnimation())
	{
		owner->SetRunTimer(0.0f);
		owner->SetAttackFlag(false);
		return ActionBase::State::Complete;
	}
	return ActionBase::State::Run;
}

//*****************************
// 
// �W�����v�U��
// 
//*****************************
// -----------------------------
// ���s�O����
// -----------------------------
void BodyPressAttackAction::Start()
{
	// �p������^�C�}�[�ݒ�
	owner->SetRunTimer(2.0f);

	// �A�j���[�V�����Đ�
	owner->PlayAnimation("NightmareDragonJump");

	// �����蔻����s�����Ԃ̃f�[�^���擾
	collision_time_data = GameDataBase::Instance().GetAttackCollitionTimeData(AttackCategory::BodyPressAttack, EnemyCategory::NightmareDragon);
}

// -----------------------------
// ���s����
// -----------------------------
ActionBase::State BodyPressAttackAction::Run(float elapsed_time)
{
	// �A�N�^�[�擾
	std::shared_ptr<Actor> actor = owner->GetActor();

	// �U���̓����蔻�菈��
	std::string collision_name = actor->GetName();
	AttackCollision(actor, collision_name.c_str(), collision_time_data, CollisionMeshType::Cylinder);

	// �^�C�}�[�擾
	float run_timer = owner->GetRunTimer();

	// �^�C�}�[����
	run_timer -= elapsed_time;

	// �A�j���[�V�����Đ����I�������犮����Ԃ�
	if (!owner->GetActor()->GetModel()->IsPlayAnimation() && run_timer <= 0.0f)
	{
		owner->SetRunTimer(0.0f);
		owner->SetAttackFlag(false);
		return ActionBase::State::Complete;
	}

	// �^�C�}�[�ݒ�
	owner->SetRunTimer(run_timer);

	return ActionBase::State::Run;
}

//*****************************
// 
// �ːi�U��
// 
//*****************************
// -----------------------------
// ���s�O����
// -----------------------------
void LungesAttackAction::Start()
{
	// �A�j���[�V�����Đ�
	owner->PlayAnimation("NightmareDragonRun");

	// �����蔻����s�����Ԃ̃f�[�^���擾
	//collision_time_data = GameDataBase::Instance().GetAttackCollitionTimeData(AttackCategory::BodyPressAttack, EnemyCategory::NightmareDragon);
	// ���Α��Ƀ^�[�Q�b�g��ݒ�
	// �^�[�Q�b�g���W�ɒl�������Ă��Ȃ���
	if (Mathf::VectorLength(target_position) == 0)
	{
		// �ڕW�n�_���v���C���[�ʒu�ɐݒ�
		owner->SetTargetPosition(ActorManager::Instance().GetActor("Player")->GetPosition());
	}
	else
	{
		// �ڕW�n�_��ݒ肳�ꂽ�ʒu�ɐݒ�
		owner->SetTargetPosition(target_position);
	}
}

// -----------------------------
// ���s����
// -----------------------------
ActionBase::State LungesAttackAction::Run(float elapsed_time)
{
	// �A�N�^�[�擾
	std::shared_ptr<Actor> owner_actor = owner->GetActor();

	// �ړI�n�֒��������̔��菈��
	if (JedgmentToTargetPosition(owner->GetTargetPosition(), owner_actor->GetPosition(), owner_actor->GetName()))
	{
		return ActionBase::State::Complete;
	}

	// �ړI�n�_�ֈړ�
	owner->MoveToTarget(elapsed_time, 1.5f);

	return ActionBase::State::Run;
}
