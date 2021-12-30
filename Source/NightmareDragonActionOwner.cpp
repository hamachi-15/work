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
// ガードアクション
// 
//*****************************
// -----------------------------
// 実行前処理
// -----------------------------
void DefendAction::Start(std::string action_name)
{
}

// -----------------------------
// 実行処理
// -----------------------------
ActionBase::State DefendAction::Run(float elapsed_time)
{
	return ActionBase::State();
}

//*****************************
// 
// 噛みつき攻撃
// 
//*****************************
// -----------------------------
// 実行前処理
// -----------------------------
void BasicAttackAction::Start(std::string action_name)
{
}

// -----------------------------
// 実行処理
// -----------------------------
ActionBase::State BasicAttackAction::Run(float elapsed_time)
{
	return ActionBase::State();
}

//*****************************
// 
// 爪攻撃
// 
//*****************************
// -----------------------------
// 実行前処理
// -----------------------------
void ClawAttackAction::Start(std::string action_name)
{
}

// -----------------------------
// 実行処理
// -----------------------------
ActionBase::State ClawAttackAction::Run(float elapsed_time)
{
	return ActionBase::State();
}

//*****************************
// 
// 角攻撃
// 
//*****************************
// -----------------------------
// 実行前処理
// -----------------------------
void HornAttackAction::Start(std::string action_name)
{
}

// -----------------------------
// 実行処理
// -----------------------------
ActionBase::State HornAttackAction::Run(float elapsed_time)
{
	return ActionBase::State();
}

//*****************************
// 
// ジャンプ攻撃
// 
//*****************************
// -----------------------------
// 実行前処理
// -----------------------------
void JumpAttackAction::Start(std::string action_name)
{
}

// -----------------------------
// 実行処理
// -----------------------------
ActionBase::State JumpAttackAction::Run(float elapsed_time)
{
	return ActionBase::State();
}
