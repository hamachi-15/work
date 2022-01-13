#include "SlimeActionOwner.h"
#include "Enemy.h"
#include "GameDatabase.h"
#include "Charactor.h"
#include "Model.h"

#include "Universal.h"

//-------------------------------------
// �W�����v�U���s���O����
//-------------------------------------
void HeavyBodyAttackAction::Start()
{
	// �A�j���[�V�����Đ�
	owner->PlayAnimation("SlimeAttack2");

	// �����蔻����s�����Ԃ̃f�[�^���擾
	collision_time_data = GameDataBase::Instance().GetAttackCollitionTimeData(AttackCategory::JumpAttack, EnemyCategory::Slime);
}

//-------------------------------------
// �W�����v�U���s��
//-------------------------------------
HeavyBodyAttackAction::State HeavyBodyAttackAction::Run(float elapsed_time)
{
	std::shared_ptr<Actor> actor = owner->GetActor();
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
	// �U���̓����蔻�菈��
	AttackCollision(actor,"SlimeHead", owner->GetCharactor()->GetID() + owner->GetIdentity(), collision_time_data);

	// �^�C�}�[�ݒ�
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
void BodyAttackAction::Start()
{
	// �A�j���[�V�����Đ�
	owner->PlayAnimation("SlimeAttack1");

	// �����蔻����s�����Ԃ̃f�[�^���擾
	collision_time_data = GameDataBase::Instance().GetAttackCollitionTimeData(AttackCategory::BodyAttack, EnemyCategory::Slime);
}

//-------------------------------------
// �̓�����U���s��
//-------------------------------------
ActionBase::State BodyAttackAction::Run(float elapsed_time)
{
	CollisionManager& collision_manager = CollisionManager::Instance();
	std::shared_ptr<Actor> actor = owner->GetActor();

	// �U���̓����蔻�菈��
	AttackCollision(actor, "SlimeHead", owner->GetCharactor()->GetID() + owner->GetIdentity(), collision_time_data);

	// �A�j���[�V�����Đ����I�������犮����Ԃ�
	if (!actor->GetModel()->IsPlayAnimation())
	{
		owner->SetRunTimer(0.0f);
		owner->SetAttackFlag(false);
		return ActionBase::State::Complete;
	}
	return ActionBase::State::Run;
}

