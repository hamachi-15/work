#include "DragonJudgmentOwner.h"
// 判定の汎用関数をまとめたやつ
#include "JudgmentUniversal.h"
// アクター関係
#include "Charactor.h"
#include "ActorManager.h"
#include "Enemy.h"
#include "Telegram.h"
// AI系
#include "MetaAI.h"
// 算出系
//#include "Mathf.h"
//*****************************************
// 
// 爪攻撃ノードに移行できるか判定
// 
//*****************************************
// -----------------------------
// 判定
// -----------------------------
bool ClawAttackJudgment::Judgment()
{
	// 怒り状態ならfalseを返す
	if (owner->GetAngerFlag()) return false;
	
	// プレイヤーとドラゴンのアクターを取得
	std::shared_ptr<Actor> player = ActorManager::Instance().GetActor("Player");
	std::shared_ptr<Actor> enemy = owner->GetActor();

	// 敵とプレイヤーの座標取得
	DirectX::XMFLOAT3 player_position = player->GetPosition();
	DirectX::XMFLOAT3 enemy_position = enemy->GetPosition();
	DirectX::XMFLOAT3 enemy_angle = enemy->GetAngle();

	// プレイヤーがドラゴンの前方にいるか判定
	return JudgmentUniversal::JudgementTargetInFront(enemy_position, enemy_angle, player_position);
}

//*****************************************
// 
// 噛みつき攻撃ノードに移行できるか判定
// 
//*****************************************
// -----------------------------
// 判定
// -----------------------------
bool BasicAttackJudgment::Judgment()
{
	// 怒り状態ならfalseを返す
	if (owner->GetAngerFlag()) return false;

	// プレイヤーとドラゴンのアクターを取得
	std::shared_ptr<Actor> player = ActorManager::Instance().GetActor("Player");
	std::shared_ptr<Actor> enemy = owner->GetActor();

	// 敵とプレイヤーの座標取得
	DirectX::XMFLOAT3 player_position = player->GetPosition();
	DirectX::XMFLOAT3 enemy_position = enemy->GetPosition();
	DirectX::XMFLOAT3 enemy_angle = enemy->GetAngle();

	// プレイヤーがドラゴンの前方にいるか判定
	return JudgmentUniversal::JudgementTargetInFront(enemy_position, enemy_angle, player_position);
}

//*****************************************
// 
// 角攻撃ノードに移行できるか判定
// 
//*****************************************
// -----------------------------
// 判定
// -----------------------------
bool HornAttackJudgment::Judgment()
{
	// 怒り状態じゃなければfalseを返す
	if (!owner->GetAngerFlag()) return false;

	// プレイヤーとドラゴンのアクターを取得
	std::shared_ptr<Actor> player = ActorManager::Instance().GetActor("Player");
	std::shared_ptr<Actor> enemy = owner->GetActor();

	// 敵とプレイヤーの座標取得
	DirectX::XMFLOAT3 player_position = player->GetPosition();
	DirectX::XMFLOAT3 enemy_position = enemy->GetPosition();
	DirectX::XMFLOAT3 enemy_angle = enemy->GetAngle();

	// プレイヤーがドラゴンの前方にいるか判定
	return JudgmentUniversal::JudgementTargetInFront(enemy->GetPosition(), enemy->GetAngle(), player->GetPosition());
}

//*****************************************
// 
// ボディプレス攻撃ノードに移行できるか判定
// 
//*****************************************
// -----------------------------
// 判定
// -----------------------------
bool BodyPressAttackJudgment::Judgment()
{
	// 怒り状態じゃなければfalseを返す
	//if (!owner->GetAngerFlag()) return false;

	// プレイヤーとドラゴンのアクターを取得
	std::shared_ptr<Actor> player = ActorManager::Instance().GetActor("Player");
	std::shared_ptr<Actor> enemy = owner->GetActor();

	// 敵とプレイヤーの座標取得
	DirectX::XMFLOAT3 player_position = player->GetPosition();
	DirectX::XMFLOAT3 enemy_position = enemy->GetPosition();
	DirectX::XMFLOAT3 enemy_angle = enemy->GetAngle();
	
	// プレイヤーがドラゴンの前方にいるか判定
	return JudgmentUniversal::JudgementTargetInFront(enemy_position, enemy_angle, player_position);
}

//*****************************************
// 
// 軸合わせノードに移行できるか判定
// 
//*****************************************
//-----------------------------------------
//判定
//-----------------------------------------
bool AligningJudgment::Judgment()
{
	// プレイヤーとドラゴンのアクターを取得
	std::shared_ptr<Actor> player = ActorManager::Instance().GetActor("Player");
	std::shared_ptr<Actor> enemy = owner->GetActor();

	// プレイヤーがドラゴンの前方にいるか判定
	if (!JudgmentUniversal::JudgementTargetInFront(enemy->GetPosition(), enemy->GetAngle(), player->GetPosition()))
	{
		// 前方にいなければtrue
		return true;
	}

	// 前方にいればfalse
	return false;
}
