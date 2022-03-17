#include "EnemyDragonNightmare.h"
#include "Graphics.h"
#include "SceneManager.h"

// メッセージ関連インクルード
#include "Telegram.h"

#include "Model.h"

// アクター関連インクルード
#include "Charactor.h"

// マネージャーインクルード
#include "ActorManager.h"
#include "EnemyManager.h"
#include "EnemyTerritoryManager.h"
#include "CollisionManager.h"
#include "AudioManager.h"

#include "EnemyCollision.h"

// AI関連インクルード
#include "NodeBase.h"
#include "BehaviorTree.h"
#include "BehaviorData.h"
#include "JudgmentOwner.h"
#include "DragonJudgmentOwner.h"
#include "ActionOwner.h"
#include "DragonActionOwner.h"

//********************************
// 
// ナイトメアドラゴンクラス
// 
//********************************
//--------------------------------------
// コンストラクタ
//--------------------------------------
EnemyDragonNightmare::EnemyDragonNightmare()
{
}

//--------------------------------------
// デストラクタ
//--------------------------------------
EnemyDragonNightmare::~EnemyDragonNightmare()
{
}

//--------------------------------------
// GUI描画
//--------------------------------------
void EnemyDragonNightmare::OnGUI()
{
	// ビヘイビア関連情報
	DrawBehaviorGUI();
}

//--------------------------------------
// メッセージを受信したときの処理
//--------------------------------------
bool EnemyDragonNightmare::OnMessages(const Telegram& message)
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
	{
		// 攻撃権を与える
		SetRightOfAttack(true);
	}
	break;
	case MessageType::Message_Hit_Boddy:
		break;
	}
	return false;
}

//--------------------------------------
// 開始処理
//--------------------------------------
void EnemyDragonNightmare::Start()
{
	// アクターの取得
	std::shared_ptr<Actor> actor = GetActor();

	// キャラクターコンポーネント取得
	std::shared_ptr<Charactor> charactor = actor->GetComponent<Charactor>();

	// 名前設定
	SetName("NightmareDragon");

	// 索敵範囲の設定
	SetSearchRange(80.0f);

	// 攻撃範囲の設定
	SetAttackRange(60.0f);

	// ムーブメントコンポーネントの設定
	SetMovement(actor->GetComponent<Movement>());

	// キャラクターコンポーネントの設定
	SetCharactor(charactor);

	// マネージャーに登録
	EnemyManager::Instance().EnemyRegister(actor->GetComponent<EnemyDragonNightmare>());


	// ビヘイビアツリー設定
	behavior_data = new BehaviorData();
	ai_tree = new BehaviorTree();

	// ノード設定
	SetBehaviorNode();
}

//--------------------------------------
// ビヘイビアのノード設定処理
//--------------------------------------
void EnemyDragonNightmare::SetBehaviorNode()
{
	// 現在のシーン名取得
	std::string scene_name = SceneManager::Instance().GetCurrentScene()->GetName();

	// シーンがワールドマップ時のノード設定
	if (scene_name == "SceneBattle") 
	{	// シーンがバトルシーンの時のノード設定
		ai_tree->AddNode("",					"Root",						0, BehaviorTree::SelectRule::Priority,	NULL,								NULL);
		ai_tree->AddNode("Root",				"Death",					1, BehaviorTree::SelectRule::Non,		new DeathJudgment(this),			new DeathAction(this));
		//ai_tree->AddNode("Root",				"Damage",					2, BehaviorTree::SelectRule::Non,		new DamageJudgment(this),			new DamageAction(this));
		ai_tree->AddNode("Root",				"Battle",					3, BehaviorTree::SelectRule::Priority,	NULL/*new BattleJudgment(this)*/,	NULL);
		ai_tree->AddNode("Root",				"Scount",					4, BehaviorTree::SelectRule::Priority,	NULL,								NULL);
		ai_tree->AddNode("Battle",				"Attack",					1, BehaviorTree::SelectRule::On_Off_Ramdom,	new AttackJudgment(this),			NULL);
		ai_tree->AddNode("Battle",				"OutRange",					2, BehaviorTree::SelectRule::Priority,		NULL,								NULL);
		ai_tree->AddNode("Scount",				"Idle",						1, BehaviorTree::SelectRule::Non,		NULL,								new IdleAction(this));
		ai_tree->AddNode("OutRange",			"Pursuit",					0, BehaviorTree::SelectRule::Non,		NULL, new PursuitAction(this));
		//ai_tree->AddNode("OutRange",			"LungesAttack",				1, BehaviorTree::SelectRule::Sequence,	NULL, NULL);
		//ai_tree->AddNode("LungesAttack",		"TurnSequence",				0, BehaviorTree::SelectRule::Non,		NULL, new TurnToTargetAction(this));
		//ai_tree->AddNode("LungesAttack",		"LungesAttackSequence",		1, BehaviorTree::SelectRule::Non,		NULL, new LungesAttackAction(this));
		ai_tree->AddNode("Attack",				"BasicAttack",				0, BehaviorTree::SelectRule::Sequence,	new BasicAttackJudgment(this),		NULL);
		ai_tree->AddNode("Attack",				"BodyPressAttack",			0, BehaviorTree::SelectRule::Random,	new BodyPressAttackJudgment(this), NULL);
		ai_tree->AddNode("Attack",				"PlayerToTurn",				0, BehaviorTree::SelectRule::Non,		new TurnToTargetJudgment(this),	new TurnToTargetAction(this));
		ai_tree->AddNode("Attack",				"ClawAttack",				0, BehaviorTree::SelectRule::Sequence,	new ClawAttackJudgment(this),		NULL);
		ai_tree->AddNode("Attack",				"HornAttack",				0, BehaviorTree::SelectRule::Non,		new ClawAttackJudgment(this),		NULL);
		ai_tree->AddNode("BasicAttack",			"TurnSequence",				0, BehaviorTree::SelectRule::Non,		NULL,								new TurnToTargetAction(this));
		ai_tree->AddNode("BasicAttack",			"BasicAttackSequence",		0, BehaviorTree::SelectRule::Non,		NULL,								new BasicAttackAction(this));
		ai_tree->AddNode("ClawAttack",			"TurnSequence",				0, BehaviorTree::SelectRule::Non,		NULL,								new TurnToTargetAction(this));
		ai_tree->AddNode("ClawAttack",			"BasicAttackSequence",		0, BehaviorTree::SelectRule::Non,		NULL,								new ClawAttackAction(this));
		ai_tree->AddNode("HornAttack",			"TurnSequence",				0, BehaviorTree::SelectRule::Non,		NULL,								new TurnToTargetAction(this));
		ai_tree->AddNode("HornAttack",			"HornAttackSequence",		0, BehaviorTree::SelectRule::Non,		NULL,								new HornAttackAction(this));
		ai_tree->AddNode("BodyPressAttack",		"OnceBodyPressAttack",		1, BehaviorTree::SelectRule::Non,		NULL,								new BodyPressAttackAction(this));
		ai_tree->AddNode("BodyPressAttack",		"DoubleBodyPressAttack",	1, BehaviorTree::SelectRule::Sequence,	NULL,								NULL);
		// 2連続ボディプレス
		ai_tree->AddNode("DoubleBodyPressAttack", "FarstBodyPressAttack",		0, BehaviorTree::SelectRule::Non, NULL,									new BodyPressAttackAction(this));
		ai_tree->AddNode("DoubleBodyPressAttack", "SecondBodyPressAttack",		0, BehaviorTree::SelectRule::Non, NULL,									new BodyPressAttackAction(this));
		ai_tree->AddNode("DoubleBodyPressAttack", "Scream",						0, BehaviorTree::SelectRule::Non, NULL,									new ScreamAction(this));
	}
	else
	{
		ai_tree->AddNode("",	 "Root",   0, BehaviorTree::SelectRule::Priority,	NULL, NULL);
		ai_tree->AddNode("Root", "Sleep",  1, BehaviorTree::SelectRule::Non,		NULL, new SleepAction(this));
	}
}

//--------------------------------------
// 敵の破棄処理
//--------------------------------------
void EnemyDragonNightmare::Destroy()
{
	// アクターの取得
	std::shared_ptr<Actor> actor = GetActor();

	// 敵マネージャーから削除
	EnemyManager::Instance().EnemyRemove(GetActor()->GetComponent<EnemyDragonNightmare>());

	// アクターマネージャーから削除
	ActorManager::Instance().Remove(GetActor());
}

//--------------------------------------
// 更新処理
//--------------------------------------
void EnemyDragonNightmare::Update(float elapsed_time)
{
	// ビヘイビア更新処理
	BehaviorUpdate(elapsed_time);
}

//--------------------------------------
// 当たり範囲デバッグプリミティブ描画
//--------------------------------------
void EnemyDragonNightmare::DrawDebugPrimitive()
{
	DebugRenderer* renderer = Graphics::Instance().GetDebugRenderer();
	std::shared_ptr<Actor> actor = GetActor();
	DirectX::XMFLOAT3 position = actor->GetPosition();
	EnemyTerritoryTag teritory_tag = GetBelongingToTerritory();
	std::shared_ptr<EnemyTerritory> enemy_territory = EnemyTerritoryManager::Instance().GetTerritory(teritory_tag);
	float territory_range = enemy_territory->GetTerritoryRange();
	DirectX::XMFLOAT3 territory_origin = enemy_territory->GetTerritoryOrigin();
	territory_origin.y = actor->GetPosition().y;
	// 縄張り範囲をデバッグ円柱描画
	renderer->DrawCylinder(territory_origin, territory_range, 1.0f, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

	// 索敵範囲をデバッグ円柱描画
	renderer->DrawCylinder(position, search_range, 1.0f, DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));

	// 攻撃範囲をデバッグ円柱描画
	renderer->DrawCylinder(position, GetAttackRange(), 1.0f, DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f));

	// ターゲット座標の球描画
	renderer->DrawSphere(target_position, 0.5f, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
}


