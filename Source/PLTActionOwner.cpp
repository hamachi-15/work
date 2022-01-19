#include "PLTActionOwner.h"
#include "GameDatabase.h"
#include "Enemy.h"
#include "Charactor.h"
#include "Model.h"

#include "Universal.h"
//---------------------------
// �p���`�U���A�j���[�V�����Đ�
//---------------------------
void BlowAttackAction::Start()
{
	// �A�j���[�V�����Đ�
	owner->PlayAnimation("PLTAttack");

	// �����蔻����s�����Ԃ̃f�[�^���擾
	collision_time_data = GameDataBase::Instance().GetAttackCollitionTimeData(AttackCategory::PunchAttack, EnemyCategory::PLT);
}

//---------------------------
// �p���`�U���s��
//---------------------------
ActionBase::State BlowAttackAction::Run(float elapsed_time)
{
	std::shared_ptr<Actor> actor = owner->GetActor();

	// �U�����̓����蔻��
	std::string collision_name = actor->GetName();
	collision_name += "RightHand";
	AttackCollision(actor, collision_name.c_str(), collision_time_data);

	// �A�j���[�V�����Đ����I�������犮��
	if (!owner->GetActor()->GetModel()->IsPlayAnimation())
	{
		owner->SetRunTimer(0.0f);
		owner->SetAttackFlag(false);
		return ActionBase::State::Complete;
	}
	return ActionBase::State::Run;
}
