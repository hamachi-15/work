#include "Model.h"
// 描画
#include "Graphics.h"
// メッセージ
#include "Telegram.h"
// コンポーネント
#include "EnemySlime.h"
#include "Charactor.h"

// マネージャー
#include "SceneManager.h"
#include "ActorManager.h"
#include "EnemyManager.h"
#include "EnemyTerritoryManager.h"
#include "CollisionManager.h"

// AI
#include "NodeBase.h"
#include "BehaviorTree.h"
#include "BehaviorData.h"
#include "JudgmentOwner.h"
#include "SlimeJudgmentOwner.h"
#include "ActionOwner.h"
#include "SlimeActionOwner.h"

// コリジョン
#include "EnemyCollision.h"

//-----------------------------------------
// コンストラクタ
//-----------------------------------------
EnemySlime::EnemySlime()
{
}

//-----------------------------------------
// デストラクタ
//-----------------------------------------
EnemySlime::~EnemySlime()
{
}

//-----------------------------------------
// GUI描画
//-----------------------------------------
void EnemySlime::OnGUI()
{
	// ビヘイビア関連情報
	DrawBehaviorGUI();
}

//-----------------------------------------
// 開始処理
//-----------------------------------------
void EnemySlime::Start()
{
	// アクターの取得
	std::shared_ptr<Actor> actor = GetActor();

	// キャラクターコンポーネント取得
	std::shared_ptr<Charactor> charactor = actor->GetComponent<Charactor>();

	// 名前設定
	SetName("Slime");

	// ムーブメントコンポーネントの設定
	SetMovement(actor->GetComponent<Movement>());

	// キャラクターコンポーネントの設定
	SetCharactor(charactor);

	// マネージャーに登録
	EnemyManager::Instance().EnemyRegister(actor->GetComponent<EnemySlime>());
	
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
void EnemySlime::SetBehaviorNode()
{
	// ノード設定
	ai_tree->AddNode("",	   "Root",	  0, BehaviorTree::SelectRule::Priority, NULL,					   NULL);
	ai_tree->AddNode("Root",   "Scount",  5, BehaviorTree::SelectRule::Priority, NULL,					   NULL);
	ai_tree->AddNode("Scount", "Wander",  1, BehaviorTree::SelectRule::Non,		 new WanderJudgment(this), new WanderAction(this));
	ai_tree->AddNode("Scount", "Idle",	  2, BehaviorTree::SelectRule::Non,		 NULL,					   new IdleAction(this));
}

//-----------------------------------------
// 更新処理
//-----------------------------------------
void EnemySlime::Update(float elapsed_time)
{
	// ビヘイビア更新処理
	BehaviorUpdate(elapsed_time);
}

//-----------------------------------------
// 敵の破棄
//-----------------------------------------
void EnemySlime::Destroy()
{
	// アクターの取得
	std::shared_ptr<Actor> actor = GetActor();
		
	// 敵マネージャーから削除
	EnemyManager::Instance().EnemyRemove(actor->GetComponent<EnemySlime>());
	
	// アクターマネージャーから削除
	ActorManager::Instance().Remove(actor);
}

//-----------------------------------------
// メッセージを受信したときの処理
//-----------------------------------------
bool EnemySlime::OnMessages(const Telegram& message)
{
	switch (message.message_box.message)
	{
	case MessageType::Message_Hit_Attack:
		break;
	case MessageType::Message_GetHit_Attack:
		// 衝突した位置を設定
		SetHitPosition(message.message_box.hit_position);
		break;
	case MessageType::Message_Give_Attack_Right:
	{
		std::shared_ptr<Charactor> charactor = GetActor()->GetComponent<Charactor>();
		// 攻撃ヒットフラグを立てる
		charactor->SetHitAttackFlag(true);
		break;
	}
	case MessageType::Message_Hit_Boddy:
		break;
	}
	return false;
}