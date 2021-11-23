#include "BossActionOwner.h"
#include "Enemy.h"
#include "GameDatabase.h"
//*********************************
// 
//			���ōU���s��
// 
//*********************************
//-----------------------------------------------
// �J�n�O����
//-----------------------------------------------
void SwipingAttackAction::Start(std::string action_name)
{
	// �A�j���[�V�����Đ�
	std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData(owner->GetName() + action_name);
	owner->PlayAnimation(animation);

}

ActionBase::State SwipingAttackAction::Run(float elapsed_time)
{
	Model* model = owner->GetActor()->GetModel();
	float animation_time = model->GetCurrentAnimationSeconds();
	bool attack_collision_flag = animation_time >= 0.3f && animation_time <= 0.4f;
	CollisionManager& sphere = CollisionManager::Instance();

	std::shared_ptr<CollisionSphere> left_hand = sphere.GetCollisionSphereFromName("Mutant:RightHandIndex1");
	// �C�ӂ̃A�j���[�V������Ԃł݂̂����蔻�������
	if (attack_collision_flag)
	{
		// �����蔻�������悤�ύX
		sphere.GetCollisionSphereFromName("Mutant:LeftHand")->SetCollisionFlag(true);
	}

	// �A�j���[�V�����Đ��I���Ńm�[�h���甲����
	if (!model->IsPlayAnimation())
	{
		sphere.GetCollisionSphereFromName("Mutant:LeftHand")->SetCollisionFlag(false);
		return ActionBase::State::Complete;
	}
	return ActionBase::State::Run;
}

void PunchAttackAction::Start(std::string action_name)
{
}

ActionBase::State PunchAttackAction::Run(float elapsed_time)
{
	return ActionBase::State();
}
