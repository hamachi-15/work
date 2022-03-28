#include "Model.h"
#include "Graphics.h"

// コンポーネント
#include "Charactor.h"
#include "EnemyLAT.h"

// マネージャー
#include "SceneManager.h"
#include "ActorManager.h"
#include "EnemyManager.h"
#include "EnemyTerritoryManager.h"
#include "CollisionManager.h"

// AI
#include "BehaviorTree.h"
#include "BehaviorData.h"
#include "NodeBase.h"
#include "JudgmentOwner.h"
#include "ActionOwner.h"

// コリジョン
#include "EnemyCollision.h"
//********************************
// 
// ラットクラス
// 
//********************************
//-----------------------------------------
// コンストラクタ
//-----------------------------------------
EnemyLAT::EnemyLAT()
{
}

//-----------------------------------------
// デストラクタ
//-----------------------------------------
EnemyLAT::~EnemyLAT()
{
}

//-----------------------------------------
// 敵の破棄処理
//-----------------------------------------
void EnemyLAT::Destroy()
{
	// アクターの取得
	std::shared_ptr<Actor> actor = GetActor();

	// 敵マネージャーから削除
	EnemyManager::Instance().EnemyRemove(GetActor()->GetComponent<EnemyLAT>());

	// アクターマネージャーから削除
	ActorManager::Instance().Remove(GetActor());

}

//-----------------------------------------
// GUI描画
//-----------------------------------------
void EnemyLAT::OnGUI()
{
	// ビヘイビア関連情報
	DrawBehaviorGUI();
}

//-----------------------------------------
// メッセージを受信したときの処理
//-----------------------------------------
bool EnemyLAT::OnMessages(const Telegram& message)
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
	}
	return false;
}

//-----------------------------------------
// 開始処理
//-----------------------------------------
void EnemyLAT::Start()
{
	// アクターの取得
	std::shared_ptr<Actor> actor = GetActor();

	// キャラクター取得
	std::shared_ptr<Charactor> charactor = actor->GetComponent<Charactor>();

	// 名前設定
	SetName("LAT");

	// ムーブメントコンポーネントの設定
	SetMovement(actor->GetComponent<Movement>());

	// キャラクターコンポーネントの設定
	SetCharactor(charactor);

	// マネージャーに登録
	EnemyManager::Instance().EnemyRegister(actor->GetComponent<EnemyLAT>());

	// 最初はターゲット座標を自身の座標に設定
	SetTargetPosition(actor->GetPosition());

	// ビヘイビアツリーの設定
	behavior_data = new BehaviorData();
	ai_tree = new BehaviorTree();

	// ノード設定
	SetBehaviorNode();
}

//-----------------------------------------
// ビヘイビアのノード設定処理
//-----------------------------------------
void EnemyLAT::SetBehaviorNode()
{
	ai_tree->AddNode("", "Root", 0, BehaviorTree::SelectRule::Priority, NULL, NULL);
	ai_tree->AddNode("Root", "Scount", 5, BehaviorTree::SelectRule::Priority, NULL, NULL);
	ai_tree->AddNode("Scount", "Wander", 1, BehaviorTree::SelectRule::Non, new WanderJudgment(this), new WanderAction(this));
	ai_tree->AddNode("Scount", "Idle", 2, BehaviorTree::SelectRule::Non, NULL, new IdleAction(this));
}

//-----------------------------------------
// 更新処理
//-----------------------------------------
void EnemyLAT::Update(float elapsed_time)
{
	// ビヘイビア更新処理
	BehaviorUpdate(elapsed_time);
}
