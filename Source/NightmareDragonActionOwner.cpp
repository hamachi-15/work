#include "NightmareDragonActionOwner.h"
#include "Mathf.h"
#include "ActorManager.h"
#include "Collision.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "GameDatabase.h"
#include "Charactor.h"

//********************************
// 
// �����A�N�V����
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