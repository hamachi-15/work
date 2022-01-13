#include "GameDatabase.h"
#include "EnemyCategory.h"

#include "DragonActionOwner.h"
#include "Actor.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "Charactor.h"

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
	AttackCollision(actor, "NightmareDragonJaw02", collision_time_data);

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
	AttackCollision(actor, "NightmareDragonRightWrist", collision_time_data);

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
	AttackCollision(actor, "NightmareDragonHead",collision_time_data);

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
	// �A�j���[�V�����Đ�
	owner->PlayAnimation("NightmareDragonDefend");

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
	AttackCollision(actor, "NightmareDragonBody", collision_time_data, CollisionMeshType::Cylinder);

	// �A�j���[�V�����Đ����I�������犮����Ԃ�
	if (!owner->GetActor()->GetModel()->IsPlayAnimation())
	{
		owner->SetRunTimer(0.0f);
		owner->SetAttackFlag(false);
		return ActionBase::State::Complete;
	}
	return ActionBase::State::Run;
}