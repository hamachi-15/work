#include "NightmareDragonActionOwner.h"
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
}

// -----------------------------
// ���s����
// -----------------------------
ActionBase::State DefendAction::Run(float elapsed_time)
{
	return ActionBase::State();
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
}

// -----------------------------
// ���s����
// -----------------------------
ActionBase::State BasicAttackAction::Run(float elapsed_time)
{
	return ActionBase::State();
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
}

// -----------------------------
// ���s����
// -----------------------------
ActionBase::State ClawAttackAction::Run(float elapsed_time)
{
	return ActionBase::State();
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
}

// -----------------------------
// ���s����
// -----------------------------
ActionBase::State HornAttackAction::Run(float elapsed_time)
{
	return ActionBase::State();
}

//*****************************
// 
// �W�����v�U��
// 
//*****************************
// -----------------------------
// ���s�O����
// -----------------------------
void JumpAttackAction::Start(std::string action_name)
{
}

// -----------------------------
// ���s����
// -----------------------------
ActionBase::State JumpAttackAction::Run(float elapsed_time)
{
	return ActionBase::State();
}
