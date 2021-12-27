#include "PLTActionOwner.h"
#include "GameDatabase.h"
#include "Enemy.h"
#include "Charactor.h"
#include "Model.h"
//---------------------------
// �p���`�U���A�j���[�V�����Đ�
//---------------------------
void BlowAttackAction::Start(std::string action_name)
{
	// �U���A�j���[�V�����Đ�
	std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("PLTAttack");
	owner->PlayAnimation(animation);
}

//---------------------------
// �p���`�U���s��
//---------------------------
ActionBase::State BlowAttackAction::Run(float elapsed_time)
{
	CollisionManager& collision_manager = CollisionManager::Instance();
	std::shared_ptr<Actor> actor = owner->GetActor();
	// ���݂̃R���W�����t���O��1�t���[���O�̃R���W�����t���O���擾
	bool old_collision_time_flag = owner->GetCharactor()->GetOldCollisionTimeFlag();
	
	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	bool collision_time_flag = owner->GetCharactor()->SearchAnimationTime(actor, 1.4f, 2.3f);
	
	// 1�t���[���O����R���W�����t���O���ω����Ă�����
	if (old_collision_time_flag != collision_time_flag)
	{
		// �R���W�����̂����蔻��̃I���I�t��؂�ւ���
		std::string name = actor->GetName();
		name += "RightHand";
		collision_manager.GetCollisionSphereFromName(name.c_str())->SetCollisionFlag(collision_time_flag);
	}
	// �O�t���[���̃R���W�����t���O�����B�R���W�����t���O���ω������^�C�~���O�𒲂ׂ�p
	owner->GetCharactor()->SetOldCollisionTimeFlag(collision_time_flag);

	// �A�j���[�V�����Đ����I�������犮��
	if (!owner->GetActor()->GetModel()->IsPlayAnimation())
	{
		owner->SetRunTimer(0.0f);
		owner->SetAttackFlag(false);
		return ActionBase::State::Complete;
	}
	return ActionBase::State::Run;
}
