
#include "EnemyDragonSoulEater.h"
// コンポーネント
#include "Charactor.h"
// マネージャー
#include "SceneManager.h"
#include "ActorManager.h"
#include "EnemyManager.h"
#include "CollisionManager.h"
#include "EnemyTerritoryManager.h"
#include "AudioManager.h"
// AI
#include "BehaviorTree.h"
#include "BehaviorData.h"
#include "ActionOwner.h"
#include "DragonActionOwner.h"
#include "JudgmentOwner.h"
#include "DragonJudgmentOwner.h"
// 描画
#include "Graphics.h"
// テリトリー関連
#include "EnemyTerritory.h"
//*********************************
// 
// ソウルイータードラゴンクラス
// 
//*********************************
//------------------------------------
// コンストラクタ
//------------------------------------
EnemyDragonSoulEater::EnemyDragonSoulEater()
{
}

//------------------------------------
// デストラクタ
//------------------------------------
EnemyDragonSoulEater::~EnemyDragonSoulEater()
{
}

//------------------------------------
// GUI描画
//------------------------------------
void EnemyDragonSoulEater::OnGUI()
{
	// ビヘイビア関連情報
	DrawBehaviorGUI();
}

//------------------------------------
// メッセージを受信したときの処理
//------------------------------------
bool EnemyDragonSoulEater::OnMessages(const Telegram& message)
{
	switch (message.message_box.message)
	{
	case MessageType::Message_Hit_Attack:
	{
		std::shared_ptr<Charactor> charactor = GetActor()->GetComponent<Charactor>();
		// 自身の攻撃ヒットフラグを立てる
		charactor->SetHitAttackFlag(true);
		// ヒットSE再生
		AudioManager::Instance().PlaySoundEffect(SEType::PlayerGetHit);
	}
	break;
	case MessageType::Message_GetHit_Attack:
		// 衝突した位置を設定
		SetHitPosition(message.message_box.hit_position);
		break;
	case MessageType::Message_Give_Attack_Right:
		// 攻撃ヒットフラグを立てる
		SetRightOfAttack(true);
	break;
	case MessageType::Message_Hit_Boddy:
		break;
	}
	return false;
}

//------------------------------------
// 開始処理
//------------------------------------
void EnemyDragonSoulEater::Start()
{
	// アクターの取得
	std::shared_ptr<Actor> actor = GetActor();

	// キャラクターコンポーネント取得
	std::shared_ptr<Charactor> charactor = actor->GetComponent<Charactor>();

	// 名前設定
	SetName("SoulEaterDragon");

	// 索敵範囲の設定
	SetSearchRange(50.0f);

	// 攻撃範囲の設定
	SetAttackRange(30.0f);

	// ムーブメントコンポーネントの設定
	SetMovement(actor->GetComponent<Movement>());

	// キャラクターコンポーネントの設定
	SetCharactor(charactor);

	// マネージャーに登録
	EnemyManager::Instance().EnemyRegister(actor->GetComponent<EnemyDragonSoulEater>());

	// ビヘイビアツリー初期化
	behavior_data = new BehaviorData();
	ai_tree = new BehaviorTree();

	// ビヘイビアツリーのノード設定
	SetBehaviorNode();
}

//------------------------------------
// ビヘイビアのノード設定処理
//------------------------------------
void EnemyDragonSoulEater::SetBehaviorNode()
{
	// 現在のシーン名取得
	std::string scene_name = SceneManager::Instance().GetCurrentScene()->GetName();

	// シーンがワールドマップ時のノード設定
	if (scene_name == "SceneBattle")
	{	// シーンがバトルシーンの時のノード設定
		ai_tree->AddNode("", "Root", 0, BehaviorTree::SelectRule::Priority, NULL, NULL);
		ai_tree->AddNode("Root", "Death", 1, BehaviorTree::SelectRule::Non, new DeathJudgment(this), new DeathAction(this));
		ai_tree->AddNode("Root", "Battle", 3, BehaviorTree::SelectRule::Priority, NULL/*new BattleJudgment(this)*/, NULL);
		ai_tree->AddNode("Root", "Scount", 4, BehaviorTree::SelectRule::Priority, NULL, NULL);
		ai_tree->AddNode("Battle", "Attack", 1, BehaviorTree::SelectRule::On_Off_Ramdom, new AttackJudgment(this), NULL);
		ai_tree->AddNode("Battle", "OutRange", 2, BehaviorTree::SelectRule::On_Off_Ramdom, NULL, NULL);
		ai_tree->AddNode("Scount", "Idle", 1, BehaviorTree::SelectRule::Non, NULL, new IdleAction(this));
		ai_tree->AddNode("OutRange", "Pursuit", 0, BehaviorTree::SelectRule::Non, NULL, new PursuitAction(this));
		ai_tree->AddNode("OutRange", "FireBallShoot", 0, BehaviorTree::SelectRule::Sequence,NULL, NULL);
		ai_tree->AddNode("Attack", "BasicAttack", 0, BehaviorTree::SelectRule::Sequence, new BasicAttackJudgment(this), NULL);
		ai_tree->AddNode("Attack", "TailAttack", 0, BehaviorTree::SelectRule::Non, NULL, new TailAttackAction(this));
		ai_tree->AddNode("Attack", "FireBallShoot", 0, BehaviorTree::SelectRule::Sequence, NULL, NULL);

		ai_tree->AddNode("FireBallShoot", "TurnSequence", 0, BehaviorTree::SelectRule::Non, NULL, new TurnToTargetAction(this));
		ai_tree->AddNode("FireBallShoot", "FireBallShootSequence", 0, BehaviorTree::SelectRule::Non, NULL, new FireBollAttackAction(this));
		
		ai_tree->AddNode("BasicAttack", "TurnSequence", 0, BehaviorTree::SelectRule::Non, NULL, new TurnToTargetAction(this));
		ai_tree->AddNode("BasicAttack", "BasicAttackSequence", 0, BehaviorTree::SelectRule::Non, NULL, new BasicAttackAction(this));
	}
	else
	{
		ai_tree->AddNode("", "Root", 0, BehaviorTree::SelectRule::Priority, NULL, NULL);
		ai_tree->AddNode("Root", "Sleep", 1, BehaviorTree::SelectRule::Non, NULL, new SleepAction(this));
	}
}

//------------------------------------
// 敵の破棄処理
//------------------------------------
void EnemyDragonSoulEater::Destroy()
{
	// アクターの取得
	std::shared_ptr<Actor> actor = GetActor();

	// 敵マネージャーから削除
	EnemyManager::Instance().EnemyRemove(GetActor()->GetComponent<EnemyDragonSoulEater>());

	// アクターマネージャーから削除
	ActorManager::Instance().Remove(GetActor());
}


//------------------------------------
// 更新処理
//------------------------------------
void EnemyDragonSoulEater::Update(float elapsed_time)
{
	// ビヘイビア更新処理
	BehaviorUpdate(elapsed_time);	
}