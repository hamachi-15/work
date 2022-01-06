#include "SceneManager.h"

#include "Charactor.h"
#include "ActorManager.h"
#include "EnemyManager.h"
#include "EnemyDragonSoulEater.h"

#include "BehaviorTree.h"
#include "BehaviorData.h"
#include "ActionOwner.h"
#include "DragonActionOwner.h"
#include "JudgmentOwner.h"
#include "DragonJudgmentOwner.h"
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
// 敵の破棄処理
//------------------------------------
void EnemyDragonSoulEater::Destroy()
{
	// アクターの取得
	std::shared_ptr<Actor> actor = GetActor();

	// コリジョン削除
	std::vector<std::shared_ptr<CollisionSphere>> list = CollisionManager::Instance().GetCollisionSphereFromID(GetCharactor()->GetID() + GetIdentity());
	for (std::shared_ptr<CollisionSphere> sphere : list)
	{
		CollisionManager::Instance().UnregisterSphere(sphere);
	}
	CollisionManager::Instance().UnregisterCylinder(CollisionManager::Instance().GetCollisionCylinderFromName(actor->GetName()));

	// 敵マネージャーから削除
	EnemyManager::Instance().EnemyRemove(GetActor()->GetComponent<EnemyDragonSoulEater>());

	// アクターマネージャーから削除
	ActorManager::Instance().Remove(GetActor());
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
		break;
	case MessageType::Message_GetHit_Attack:
		//ダメージフラグをオンに
		OnDamaged();
		// 衝突した位置を設定
		SetHitPosition(message.message_box.hit_position);
		break;
	case MessageType::Message_Give_Attack_Right:
		// 攻撃フラグをオンに
		SetAttackFlag(true);
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

	// ムーブメントコンポーネントの設定
	SetMovement(actor->GetComponent<Movement>());

	// キャラクターコンポーネントの設定
	SetCharactor(charactor);

	// マネージャーに登録
	EnemyManager::Instance().EnemyRegister(actor->GetComponent<EnemyDragonSoulEater>());
	
	// コリジョンの設定
	{
		Model* model = GetActor()->GetModel();
		CollisionParameter parameter;
		// カリング用のコリジョン
		parameter.name = "SoulEaterDragonAABB";
		parameter.node_name = "";
		parameter.position = {};
		parameter.float3_radius = DirectX::XMFLOAT3(1.5f, 1.5f, 1.5f);
		parameter.height = 9.0f;
		parameter.collision_flg = true;
		parameter.actor_id = charactor->GetID();
		parameter.element = CollisionElement::Body;
		parameter.position_mask = CollisionPositionMask::Collision_Mask_Actor_Position;
		charactor->SetCollision(actor, parameter, CollisionMeshType::AABB);
		
		// 体のコリジョン設定
		parameter.name = actor->GetName();
		parameter.node_name = "";
		parameter.actor_id = charactor->GetID() + GetIdentity();
		parameter.position = { 0, 0, 0 };
		parameter.radius = 20.5f;
		parameter.height = 26.5f;
		parameter.weight = 6.5f;
		parameter.collision_flg = true;
		parameter.actor_type = CollisionActorType::Enemy;
		parameter.element = CollisionElement::Body;
		parameter.position_mask = CollisionPositionMask::Collision_Mask_Actor_Position;
		charactor->SetCollision(actor, parameter, CollisionMeshType::Cylinder);

		// 頭コリジョン
		name.clear();
		name = actor->GetName();
		name += "Head";
		parameter.name = name.c_str();
		parameter.node_name = "UpperHead02";
		parameter.radius = 6.5f;
		parameter.height = 0.0f;
		parameter.weight = 1.0f;
		parameter.collision_flg = false;
		parameter.actor_type = CollisionActorType::Enemy;
		parameter.element = CollisionElement::Weppon;
		parameter.position_mask = CollisionPositionMask::Collision_Mask_Member_Position;
		charactor->SetCollision(actor, parameter, CollisionMeshType::Sphere);
	
		// 尻尾コリジョン
		name.clear();
		name = actor->GetName();
		name += "Tail";
		parameter.name = name.c_str();
		parameter.node_name = "TailEnd";
		parameter.radius = 3.5f;
		parameter.collision_flg = false;
		parameter.actor_type = CollisionActorType::Enemy;
		parameter.element = CollisionElement::Weppon;
		parameter.position_mask = CollisionPositionMask::Collision_Mask_Member_Position;
		charactor->SetCollision(actor, parameter, CollisionMeshType::Sphere);
	}

	// ビヘイビアツリー設定
	behavior_data = new BehaviorData();
	ai_tree = new BehaviorTree();

	// ノード設定
	SetBehaviorNode();
}

//------------------------------------
// ビヘイビアのノード設定処理
//------------------------------------
void EnemyDragonSoulEater::SetBehaviorNode()
{
	// 現在のシーン名取得
	const char* name = SceneManager::Instance().GetCurrentScene()->GetName();

	// シーンがワールドマップ時のノード設定
	if (strcmp(name, "SceneWorldMap") == 0)
	{
		ai_tree->AddNode("", "Root", 0, BehaviorTree::SelectRule::Priority, NULL, NULL);
		ai_tree->AddNode("Root", "Sleep", 1, BehaviorTree::SelectRule::Non, NULL, new SleepAction(this));

	} // シーンがバトルシーンの時のノード設定
	else //if (strcmp(name, "SceneBattle") == 0)
	{
		ai_tree->AddNode("", "Root", 0, BehaviorTree::SelectRule::Priority, NULL, NULL);
		ai_tree->AddNode("Root", "Death", 1, BehaviorTree::SelectRule::Non, new DeathJudgment(this), new DeathAction(this));
		ai_tree->AddNode("Root", "Damage", 2, BehaviorTree::SelectRule::Non, new DamageJudgment(this), new DamageAction(this));
		ai_tree->AddNode("Root", "Battle", 3, BehaviorTree::SelectRule::Priority, new BattleJudgment(this), NULL);
		ai_tree->AddNode("Root", "Scount", 4, BehaviorTree::SelectRule::Priority, NULL, NULL);
		ai_tree->AddNode("Battle", "Attack", 1, BehaviorTree::SelectRule::Priority, new AttackJudgment(this), NULL);
		ai_tree->AddNode("Battle", "Pursuit", 2, BehaviorTree::SelectRule::Non, NULL, new PursuitAction(this));
		//ai_tree->AddNode("Attack", "BasicAttack", 1, BehaviorTree::SelectRule::Non, new BasicAttackJudgment(this), new BasicAttackAction(this));
		//ai_tree->AddNode("Attack", "ClawAttack", 2, BehaviorTree::SelectRule::Non, new ClawAttackJudgment(this), new ClawAttackAction(this));
		//ai_tree->AddNode("Attack", "HornAttack", 3, BehaviorTree::SelectRule::Non, new ClawAttackJudgment(this), new HornAttackAction(this));
		//ai_tree->AddNode("Attack", "JumpAttack", 4, BehaviorTree::SelectRule::Non, new JumpAttackJudgment(this), new JumpAttackAction(this));
		ai_tree->AddNode("Scount", "Wander", 1, BehaviorTree::SelectRule::Non, new WanderJudgment(this), new WanderAction(this));
		ai_tree->AddNode("Scount", "Idle", 2, BehaviorTree::SelectRule::Non, NULL, new IdleAction(this));
	}
}

//------------------------------------
// 更新処理
//------------------------------------
void EnemyDragonSoulEater::Update(float elapsed_time)
{
	// ビヘイビアツリー更新処理
	if (active_node == nullptr)
	{
		active_node = ai_tree->ActiveNodeInference(this, behavior_data);
	}
	if (active_node != nullptr && active_node != old_active_node)
	{
		ai_tree->Start(active_node);
	}
	if (active_node != nullptr)
	{
		active_node = ai_tree->Run(this, active_node, behavior_data, elapsed_time);
	}
	old_active_node = active_node;

	// 速力更新処理
	GetMovement()->UpdateVelocity(elapsed_time);

	// 無敵時間更新処理
	GetCharactor()->UpdateInvincibleTimer(elapsed_time);
}

//------------------------------------
// 当たり範囲デバッグプリミティブ描画
//------------------------------------
void EnemyDragonSoulEater::DrawDebugPrimitive()
{
}

