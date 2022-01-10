#include "DragonActionOwner.h"
#include "Mathf.h"
#include "ActorManager.h"
#include "Collision.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "GameDatabase.h"
#include "Charactor.h"


//*****************************
// 
// �K�[�h�A�N�V����
// 
//*****************************
// -----------------------------
// ���s�O����
// -----------------------------
void DefendAction::Start(std::string action_name)
{
	// �K�[�h�A�N�V�����̃f�[�^�擾
	std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("NightmareDragonDefend");
	// �A�j���[�V�����Đ�
	owner->PlayAnimation(animation);
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
void BasicAttackAction::Start(std::string action_name)
{
	// �K�[�h�A�N�V�����̃f�[�^�擾
	std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("NightmareDragonBasicAttack");
	// �A�j���[�V�����Đ�
	owner->PlayAnimation(animation);
}

// -----------------------------
// ���s����
// -----------------------------
ActionBase::State BasicAttackAction::Run(float elapsed_time)
{
	CollisionManager& collision_manager = CollisionManager::Instance();
	std::shared_ptr<Actor> actor = owner->GetActor();
	// ���݂̃R���W�����t���O��1�t���[���O�̃R���W�����t���O���擾
	bool old_collision_time_flag = owner->GetCharactor()->GetOldCollisionTimeFlag();

	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	bool collision_time_flag = owner->GetCharactor()->SearchAnimationTime(actor, 0.2f, 0.6f);

	// 1�t���[���O����R���W�����t���O���ω����Ă�����
	if (old_collision_time_flag != collision_time_flag)
	{
		// �R���W�����̂����蔻��̃I���I�t��؂�ւ���
		std::shared_ptr<CollisionSphere> collision = collision_manager.GetCollisionSphereFromName("NightmareDragonJaw02");

		// �����蔻����s���t���O��ω�������
		collision->SetCollisionFlag(collision_time_flag);
	}
	// �O�t���[���̃R���W�����t���O�����B�R���W�����t���O���ω������^�C�~���O�𒲂ׂ�p
	owner->GetCharactor()->SetOldCollisionTimeFlag(collision_time_flag);

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
void ClawAttackAction::Start(std::string action_name)
{
	// �K�[�h�A�N�V�����̃f�[�^�擾
	std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("NightmareDragonClawAttack");
	// �A�j���[�V�����Đ�
	owner->PlayAnimation(animation);
}

// -----------------------------
// ���s����
// -----------------------------
ActionBase::State ClawAttackAction::Run(float elapsed_time)
{
	CollisionManager& collision_manager = CollisionManager::Instance();
	std::shared_ptr<Actor> actor = owner->GetActor();
	// ���݂̃R���W�����t���O��1�t���[���O�̃R���W�����t���O���擾
	bool old_collision_time_flag = owner->GetCharactor()->GetOldCollisionTimeFlag();

	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	bool collision_time_flag = owner->GetCharactor()->SearchAnimationTime(actor, 0.2f, 0.6f);

	// 1�t���[���O����R���W�����t���O���ω����Ă�����
	if (old_collision_time_flag != collision_time_flag)
	{
		// �R���W�����̂����蔻��̃I���I�t��؂�ւ���
		std::shared_ptr<CollisionSphere> collision = collision_manager.GetCollisionSphereFromName("NightmareDragonRightWrist");

		// �����蔻����s���t���O��ω�������
		collision->SetCollisionFlag(collision_time_flag);
	}
	// �O�t���[���̃R���W�����t���O�����B�R���W�����t���O���ω������^�C�~���O�𒲂ׂ�p
	owner->GetCharactor()->SetOldCollisionTimeFlag(collision_time_flag);

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
void HornAttackAction::Start(std::string action_name)
{
	// �K�[�h�A�N�V�����̃f�[�^�擾
	std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("NightmareDragonHornAttack");
	// �A�j���[�V�����Đ�
	owner->PlayAnimation(animation);
}

// -----------------------------
// ���s����
// -----------------------------
ActionBase::State HornAttackAction::Run(float elapsed_time)
{
	CollisionManager& collision_manager = CollisionManager::Instance();
	std::shared_ptr<Actor> actor = owner->GetActor();
	// ���݂̃R���W�����t���O��1�t���[���O�̃R���W�����t���O���擾
	bool old_collision_time_flag = owner->GetCharactor()->GetOldCollisionTimeFlag();

	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	bool collision_time_flag = owner->GetCharactor()->SearchAnimationTime(actor, 0.2f, 0.6f);

	// 1�t���[���O����R���W�����t���O���ω����Ă�����
	if (old_collision_time_flag != collision_time_flag)
	{
		// �R���W�����̂����蔻��̃I���I�t��؂�ւ���
		std::shared_ptr<CollisionSphere> collision = collision_manager.GetCollisionSphereFromName("NightmareDragonHead");

		// �����蔻����s���t���O��ω�������
		collision->SetCollisionFlag(collision_time_flag);
	}
	// �O�t���[���̃R���W�����t���O�����B�R���W�����t���O���ω������^�C�~���O�𒲂ׂ�p
	owner->GetCharactor()->SetOldCollisionTimeFlag(collision_time_flag);

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
void BodyPressAttackAction::Start(std::string action_name)
{
	// �K�[�h�A�N�V�����̃f�[�^�擾
	std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("NightmareDragonDefend");
	// �A�j���[�V�����Đ�
	owner->PlayAnimation(animation);
}

// -----------------------------
// ���s����
// -----------------------------
ActionBase::State BodyPressAttackAction::Run(float elapsed_time)
{
	CollisionManager& collision_manager = CollisionManager::Instance();
	std::shared_ptr<Actor> actor = owner->GetActor();
	// ���݂̃R���W�����t���O��1�t���[���O�̃R���W�����t���O���擾
	bool old_collision_time_flag = owner->GetCharactor()->GetOldCollisionTimeFlag();

	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	bool collision_time_flag = owner->GetCharactor()->SearchAnimationTime(actor, 0.2f, 0.6f);

	// 1�t���[���O����R���W�����t���O���ω����Ă�����
	if (old_collision_time_flag != collision_time_flag)
	{
		// �R���W�����̂����蔻��̃I���I�t��؂�ւ���
		std::shared_ptr<CollisionSphere> collision = collision_manager.GetCollisionSphereFromName("NightmareDragonBody");

		// �����蔻����s���t���O��ω�������
		collision->SetCollisionFlag(collision_time_flag);
	}
	// �O�t���[���̃R���W�����t���O�����B�R���W�����t���O���ω������^�C�~���O�𒲂ׂ�p
	owner->GetCharactor()->SetOldCollisionTimeFlag(collision_time_flag);

	// �A�j���[�V�����Đ����I�������犮����Ԃ�
	if (!owner->GetActor()->GetModel()->IsPlayAnimation())
	{
		owner->SetRunTimer(0.0f);
		owner->SetAttackFlag(false);
		return ActionBase::State::Complete;
	}
	return ActionBase::State::Run;
}
