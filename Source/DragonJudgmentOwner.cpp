#include "DragonJudgmentOwner.h"
#include "ActorManager.h"
#include "Enemy.h"
#include "Charactor.h"
#include "Telegram.h"
#include "MetaAI.h"
#include "Mathf.h"
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
	
	
	return true;
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

	return false;
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
	return false;
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
	if (!owner->GetAngerFlag()) return false;

	std::shared_ptr<Actor> player = ActorManager::Instance().GetActor("Player");
	std::shared_ptr<Actor> enemy = owner->GetActor();

	// 敵とプレイヤーの座標取得
	DirectX::XMFLOAT3 player_position = player->GetPosition();
	DirectX::XMFLOAT3 enemy_position = enemy->GetPosition();

	// 敵とプレイヤーのベクトル算出
	DirectX::XMVECTOR vector = Mathf::ReturnVectorSubtract(player_position, enemy_position);

	// 敵の角度から前方向を算出
	DirectX::XMVECTOR front = Mathf::ReturnVectorFront(enemy->GetAngle());

	// 敵の前方向とベクトルから内積算出
	float dot = Mathf::ReturnFloatDot(front, vector);

	// 内積が負の数ならfalseを返す
	if (dot < 0) return false;

	return true;
}
