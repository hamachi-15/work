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

	// コリジョンの設定
	{
		Model* model = GetActor()->GetModel();
		CollisionParameter parameter;
		// カリング用のコリジョン
		parameter.name = "NightmareDragonAABB";
		parameter.node_name = "Spine02";
		parameter.float3_radius = DirectX::XMFLOAT3(40.5f, 25.5f, 60.5f);
		parameter.collision_flg = true;
		parameter.actor_id = charactor->GetID();
		parameter.element = CollisionElement::Body;
		parameter.position_mask = CollisionPositionMask::Collision_Mask_Member_Position;
		charactor->SetCollision(actor, parameter, CollisionMeshType::AABB);

		// 右腕コリジョン
		std::string name = actor->GetName();
		name += "RightWrist";
		parameter.name = name.c_str();
		parameter.node_name = "R_Wrist";
		parameter.actor_id = charactor->GetID() + GetIdentity();
		parameter.radius = 4.0f;
		parameter.weight = 1.0f;
		parameter.collision_flg = false;
		parameter.actor_type = CollisionActorType::Enemy;
		parameter.element = CollisionElement::Weppon;
		charactor->SetCollision(actor, parameter, CollisionMeshType::Sphere);

		// 頭コリジョン
		name.clear();
		name = actor->GetName();
 		name += "Head";
		parameter.name = name.c_str();
		parameter.node_name = "UpperHead02";
		parameter.radius = 6.5f;
		charactor->SetCollision(actor, parameter, CollisionMeshType::Sphere);
	
		// 口のコリジョン
		name.clear();
		name = actor->GetName();
		name += "mouth";
		parameter.name = name.c_str();
		parameter.node_name = "Jaw02";
		parameter.radius = 6.5f;
		charactor->SetCollision(actor, parameter, CollisionMeshType::Sphere);

		// 体のコリジョン設定
		Model::Node* node = model->FindNode("Root");
		node->translate;
		DirectX::XMMATRIX world_transform_matrix = DirectX::XMLoadFloat4x4(&node->world_transform);
		DirectX::XMVECTOR position = DirectX::XMVector3TransformCoord(
			DirectX::XMLoadFloat3(&parameter.local_position), world_transform_matrix);
		DirectX::XMStoreFloat3(&parameter.position, position);
		parameter.name = actor->GetName();
		parameter.node_name = "Root";
		parameter.radius = 20.5f;
		parameter.height = 34.5f;
		parameter.weight = 6.5f;
		parameter.local_position = DirectX::XMFLOAT3(0, 14, 0);
		parameter.collision_flg = true;
		parameter.element = CollisionElement::Body;
		parameter.position_mask = CollisionPositionMask::Collision_Mask_Castam_poition;
		charactor->SetCollision(actor, parameter, CollisionMeshType::Cylinder);

	}

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
	const char* name = SceneManager::Instance().GetCurrentScene()->GetName();

	// シーンがワールドマップ時のノード設定
	if (strcmp(name, "SceneWorldMap") == 0)
	{
		ai_tree->AddNode("",	 "Root",   0, BehaviorTree::SelectRule::Priority,	NULL, NULL);
		ai_tree->AddNode("Root", "Sleep",  1, BehaviorTree::SelectRule::Non,		NULL, new SleepAction(this));
	} // シーンがバトルシーンの時のノード設定
	else
	{
		ai_tree->AddNode("",						"Root",					 0,	BehaviorTree::SelectRule::Priority,	 NULL,								NULL);
		ai_tree->AddNode("Root",					"Death",				 1,	BehaviorTree::SelectRule::Non,		 new DeathJudgment(this),			new DeathAction(this));
		ai_tree->AddNode("Root",					"Damage",				 2,	BehaviorTree::SelectRule::Non,		 new DamageJudgment(this),			new DamageAction(this));
		ai_tree->AddNode("Root",					"Battle",				 3,	BehaviorTree::SelectRule::Priority,	 new BattleJudgment(this),			NULL);
		ai_tree->AddNode("Root",					"Scount",				 4,	BehaviorTree::SelectRule::Priority,	 NULL,								NULL);
		ai_tree->AddNode("Battle",					"Attack",				 1,	BehaviorTree::SelectRule::On_Off_Ramdom,	 new AttackJudgment(this),			NULL);
		ai_tree->AddNode("Battle",					"Pursuit",				 2,	BehaviorTree::SelectRule::Non,		 NULL,								new PursuitAction(this));
		ai_tree->AddNode("Scount",					"Idle",					 1,	BehaviorTree::SelectRule::Non,		 NULL,								new IdleAction(this));
		ai_tree->AddNode("Scount",					"Scream",				 2, BehaviorTree::SelectRule::Non,		 NULL, new ScreamAction(this));
		ai_tree->AddNode("Attack",					"BasicAttack",			 1,	BehaviorTree::SelectRule::Non,		 new BasicAttackJudgment(this),		new BasicAttackAction(this));
		ai_tree->AddNode("Attack",					"ClawAttack",			 2,	BehaviorTree::SelectRule::Non,		 new ClawAttackJudgment(this),		new ClawAttackAction(this));
		ai_tree->AddNode("Attack",					"HornAttack",			 3,	BehaviorTree::SelectRule::Non,		 new ClawAttackJudgment(this),		new HornAttackAction(this));
		ai_tree->AddNode("Attack",					"BodyPressAttack",		 4, BehaviorTree::SelectRule::Random, new BodyPressAttackJudgment(this), NULL);
		//ai_tree->AddNode("Attack",				"LungesAttack",			 5,	BehaviorTree::SelectRule::Sequence,		 NULL/*new (this)*/, new LungesAttackAction(this));
		ai_tree->AddNode("BodyPressAttack",			"OnceBodyPressAttack",	 1, BehaviorTree::SelectRule::Non, NULL, new BodyPressAttackAction(this));
		ai_tree->AddNode("BodyPressAttack",			"DoubleBodyPressAttack", 1, BehaviorTree::SelectRule::Sequence, NULL, NULL);
		ai_tree->AddNode("DoubleBodyPressAttack",	"FarstBodyPressAttack",	 0, BehaviorTree::SelectRule::Non, NULL, new BodyPressAttackAction(this));
		ai_tree->AddNode("DoubleBodyPressAttack",	"SecondBodyPressAttack", 0, BehaviorTree::SelectRule::Non,NULL, new BodyPressAttackAction(this));
		ai_tree->AddNode("DoubleBodyPressAttack",	"Scream",				 0, BehaviorTree::SelectRule::Non, NULL, new ScreamAction(this));
		ai_tree->AddNode("LungesAttack",			"MoveToTargetSequence",	 0, BehaviorTree::SelectRule::Non, new BodyPressAttackJudgment(this), new BodyPressAttackAction(this));
		ai_tree->AddNode("LungesAttack",			"LungesAttackSequence",	 0, BehaviorTree::SelectRule::Non, new BodyPressAttackJudgment(this), new LungesAttackAction(this));
	}
}

//--------------------------------------
// 敵の破棄処理
//--------------------------------------
void EnemyDragonNightmare::Destroy()
{
	// アクターの取得
	std::shared_ptr<Actor> actor = GetActor();

	// コリジョン削除
	// 球コリジョン削除
	std::vector<std::shared_ptr<CollisionSphere>> list = CollisionManager::Instance().GetCollisionSphereFromID(GetCharactor()->GetID() + GetIdentity());
	for (std::shared_ptr<CollisionSphere> sphere : list)
	{
		CollisionManager::Instance().UnregisterSphere(sphere);
	}

	// 円柱コリジョン削除
	CollisionManager::Instance().UnregisterCylinder(CollisionManager::Instance().GetCollisionCylinderFromName(actor->GetName()));

	// 立方体コリジョン削除
	CollisionManager::Instance().UnregisterBox(CollisionManager::Instance().GetCollisionBoxFromName("NightmareDragonAABB"));

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

	// 速力更新処理
	GetMovement()->UpdateVelocity(elapsed_time);

	// 無敵時間更新処理
	GetCharactor()->UpdateInvincibleTimer(elapsed_time);
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


