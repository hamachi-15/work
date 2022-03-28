// モデル
#include "Model.h"

// 描画
#include "Graphics.h"
#include "DebugRenderer.h"

// AI
#include "NodeBase.h"
#include "BehaviorData.h"
#include "BehaviorTree.h"
#include "JudgmentOwner.h"
#include "PLTJudgmentOwner.h"
#include "ActionOwner.h"
#include "PLTActionOwner.h"

// コンポーネント
#include "Charactor.h"
#include "EnemyPLT.h"

// マネージャー
#include "SceneManager.h"
#include "ActorManager.h"
#include "EnemyManager.h"
#include "EnemyTerritoryManager.h"
#include "CollisionManager.h"

// コリジョン
#include "EnemyCollision.h"

//**********************************
// 
// プラットクラス
// 
//**********************************
//-----------------------------------------
// コンストラクタ
//-----------------------------------------
EnemyPLT::EnemyPLT()
{
}

//-----------------------------------------
// デストラクタ
//-----------------------------------------
EnemyPLT::~EnemyPLT()
{
}

//-----------------------------------------
// 破棄処理
//-----------------------------------------
void EnemyPLT::Destroy()
{
	// アクターの取得
	std::shared_ptr<Actor> actor = GetActor();

	// 敵マネージャーから削除
	EnemyManager::Instance().EnemyRemove(GetActor()->GetComponent<EnemyPLT>());

	// アクターマネージャーから削除
	ActorManager::Instance().Remove(GetActor());
}

//-----------------------------------------
// GUI描画
//-----------------------------------------
void EnemyPLT::OnGUI()
{
	// ビヘイビア関連情報
	DrawBehaviorGUI();
}

//-----------------------------------------
// メッセージを受信したときの処理
//-----------------------------------------
bool EnemyPLT::OnMessages(const Telegram& message)
{
	switch (message.message_box.message)
	{
	case MessageType::Message_Hit_Attack:

		break;
	case MessageType::Message_GetHit_Attack:
		// 衝突した位置を設定
		SetHitPosition(message.message_box.hit_position);
		break;
	case MessageType::Message_Hit_Boddy:
		break;
	case MessageType::Message_Give_Attack_Right:
	{
		std::shared_ptr<Charactor> charactor = GetActor()->GetComponent<Charactor>();
		// 攻撃ヒットフラグを立てる
		charactor->SetHitAttackFlag(true);
	}
	break;
	}
	return false;
}

//-----------------------------------------
// 開始処理
//-----------------------------------------
void EnemyPLT::Start()
{
	// アクターの取得
	std::shared_ptr<Actor> actor = GetActor();

	// 名前設定
	SetName("PLT");

	// ムーブメントコンポーネントの設定
	SetMovement(actor->GetComponent<Movement>());

	// キャラクターコンポーネントの設定
	SetCharactor(actor->GetComponent<Charactor>());

	// マネージャーに登録
	EnemyManager::Instance().EnemyRegister(actor->GetComponent<EnemyPLT>());

	// 最初はターゲット座標を自身の座標に設定
	SetTargetPosition(actor->GetPosition());

	// ビヘイビアツリー設定
	behavior_data = new BehaviorData();
	ai_tree = new BehaviorTree();

	// ノード設定
	SetBehaviorNode();
}

//-----------------------------------------
// ビヘイビアのノード設定処理
//-----------------------------------------
void EnemyPLT::SetBehaviorNode()
{
	// ノード設定
	ai_tree->AddNode("", "Root", 0, BehaviorTree::SelectRule::Priority, NULL, NULL);
	ai_tree->AddNode("Root", "Scount", 2, BehaviorTree::SelectRule::Priority, NULL, NULL);
	ai_tree->AddNode("Scount", "Wander", 1, BehaviorTree::SelectRule::Non, new WanderJudgment(this), new WanderAction(this));
	ai_tree->AddNode("Scount", "Idle", 2, BehaviorTree::SelectRule::Non, NULL, new IdleAction(this));

}

//-----------------------------------------
// 更新処理
//-----------------------------------------
void EnemyPLT::Update(float elapsed_time)
{
	// ビヘイビア更新処理
	BehaviorUpdate(elapsed_time);
}