#include "SlimeActionOwner.h"
#include "Enemy.h"
#include "GameDatabase.h"
#include "Charactor.h"


//-------------------------------------
// �W�����v�U���s���O����
//-------------------------------------
void HeavyBodyAttackAction::Start(std::string action_name)
{
	std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("SlimeAttack2");
	owner->PlayAnimation(animation);
}

//-------------------------------------
// �W�����v�U���s��
//-------------------------------------
HeavyBodyAttackAction::State HeavyBodyAttackAction::Run(float elapsed_time)
{
	CollisionManager& collision_manager = CollisionManager::Instance();
	std::shared_ptr<Actor> actor = owner->GetActor();
	// ���݂̃R���W�����t���O��1�t���[���O�̃R���W�����t���O���擾
	bool old_collision_time_flag = owner->GetCharactor()->GetOldCollisionTimeFlag();
	float run_timer = owner->GetRunTimer();
	if (run_timer <= 0.0f)
	{
		// �X�L��
		owner->SetRunTimer(0.7f);
		run_timer = owner->GetRunTimer();
	}
	else
	{
		run_timer -= elapsed_time;
	}
	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	bool collision_time_flag = owner->GetCharactor()->SearchAnimationTime(actor, 0.2f, 0.6f);
	
	// 1�t���[���O����R���W�����t���O���ω����Ă�����
	if (old_collision_time_flag != collision_time_flag)
	{
		// �R���W�����̂����蔻��̃I���I�t��؂�ւ���
		std::vector<std::shared_ptr<CollisionSphere>> list = collision_manager.GetCollisionSphereFromID(owner->GetCharactor()->GetID() + owner->GetIdentity());
		for (std::shared_ptr<CollisionSphere> sphere : list)
		{
			sphere->SetCollisionFlag(collision_time_flag);
		}
	}
	// �O�t���[���̃R���W�����t���O�����B�R���W�����t���O���ω������^�C�~���O�𒲂ׂ�p
	owner->GetCharactor()->SetOldCollisionTimeFlag(collision_time_flag);

	owner->SetRunTimer(run_timer);

	if (run_timer <= 0.0f)
	{
		owner->SetRunTimer(0.0f);
		owner->SetAttackFlag(false);
		return ActionBase::State::Complete;
	}
	// �A�j���[�V�����Đ����I�������犮����Ԃ�
	if (!owner->GetActor()->GetModel()->IsPlayAnimation())
	{
		owner->SetRunTimer(0.0f);
		owner->SetAttackFlag(false);
		return ActionBase::State::Complete;
	}
	return ActionBase::State::Run;
}

//-------------------------------------
// �̓�����U���s���O����
//-------------------------------------
void BodyAttackAction::Start(std::string action_name)
{
	std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("SlimeAttack1");
	owner->PlayAnimation(animation);
}

//-------------------------------------
// �̓�����U���s��
//-------------------------------------
ActionBase::State BodyAttackAction::Run(float elapsed_time)
{
	CollisionManager& collision_manager = CollisionManager::Instance();
	std::shared_ptr<Actor> actor = owner->GetActor();
	// ���݂̃R���W�����t���O��1�t���[���O�̃R���W�����t���O���擾
	bool old_collision_time_flag = owner->GetCharactor()->GetOldCollisionTimeFlag();
	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	bool collision_time_flag =owner->GetCharactor()->SearchAnimationTime(actor, 0.2f, 0.4f);
	
	// 1�t���[���O����R���W�����t���O���ω����Ă�����
	if (old_collision_time_flag != collision_time_flag)
	{
		// �R���W�����̂����蔻��̃I���I�t��؂�ւ���
		std::string name = actor->GetName();
		name += "Head";
		collision_manager.GetCollisionSphereFromName(name.c_str())->SetCollisionFlag(collision_time_flag);
	}
	// �O�t���[���̃R���W�����t���O�����B�R���W�����t���O���ω������^�C�~���O�𒲂ׂ�p
	owner->GetCharactor()->SetOldCollisionTimeFlag(collision_time_flag);

	// �A�j���[�V�����Đ����I�������犮����Ԃ�
	if (!actor->GetModel()->IsPlayAnimation())
	{
		owner->SetRunTimer(0.0f);
		owner->SetAttackFlag(false);
		return ActionBase::State::Complete;
	}
	return ActionBase::State::Run;
}

