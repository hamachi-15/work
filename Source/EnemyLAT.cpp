#include "Model.h"
#include "Graphics.h"
#include "SceneManager.h"

#include "Charactor.h"
#include "ActorManager.h"
#include "EnemyManager.h"
#include "EnemyLAT.h"
#include "EnemyTerritoryManager.h"

#include "BehaviorTree.h"
#include "BehaviorData.h"
#include "NodeBase.h"
#include "JudgmentOwner.h"
#include "ActionOwner.h"

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
	CollisionManager::Instance().UnregisterBox(CollisionManager::Instance().GetCollisionBoxFromName("LATAABB"));

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
	bool attack_flag = GetAttackFlag();
	ImGui::Checkbox("AttackFlag", &attack_flag);

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
		//ダメージフラグをオンに
		OnDamaged();
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

	// 索敵範囲の設定
	SetSearchRange(10.0f);

	// 攻撃範囲の設定
	SetAttackRange(5.0f);

	// テリトリー範囲の設定
	DirectX::XMFLOAT3 position = actor->GetPosition();
	//SetTerritoryOrigin(position);


	// 最初はターゲット座標を自身の座標に設定
	SetTargetPosition(actor->GetPosition());

	// コリジョン設定
	{
		CollisionParameter parameter;
		// モデル取得
		Model* model = actor->GetModel();

		// カリング用のコリジョン
		parameter.name = "LATAABB";
		parameter.node_name = "";
		parameter.position = {};
		parameter.float3_radius = DirectX::XMFLOAT3(1.5f, 1.5f, 1.5f);
		parameter.collision_flg = true;
		parameter.actor_id = charactor->GetID();
		parameter.actor_type = CollisionActorType::Enemy;
		parameter.element = CollisionElement::Body;
		parameter.position_mask = CollisionPositionMask::Collision_Mask_Actor_Position;
		charactor->SetCollision(actor, parameter, CollisionMeshType::AABB);

		// 体のコリジョン設定
		parameter.name = actor->GetName();
		parameter.actor_id = charactor->GetID() + GetIdentity();
		parameter.position = { 0.0f, 0.0f, 0.0f };
		parameter.radius = 3.0f;
		parameter.weight = 1.0f;
		parameter.height = 5.0f;
		parameter.collision_flg = true;
		parameter.actor_type = CollisionActorType::Enemy;
		parameter.element = CollisionElement::Body;
		parameter.position_mask = CollisionPositionMask::Collision_Mask_Actor_Position;
		charactor->SetCollision(actor, parameter, CollisionMeshType::Cylinder);

		// 尻尾のコリジョン設定
		DirectX::XMFLOAT3 tail_position;
		charactor->GetNodePosition("LAT:tai102_M_BK", tail_position, model);
		std::string name = actor->GetName();
		name += "Tail";
		parameter.name = name.c_str();
		parameter.node_name = "LAT:tai102_M_BK";
		parameter.actor_id = charactor->GetID();
		parameter.position = tail_position;
		parameter.radius = 3.0f;
		parameter.weight = 1.0f;
		parameter.height = 0.0f;
		parameter.collision_flg = false;
		parameter.actor_type = CollisionActorType::Enemy;
		parameter.element = CollisionElement::Weppon;
		parameter.position_mask = CollisionPositionMask::Collision_Mask_Member_Position;
		charactor->SetCollision(actor, parameter, CollisionMeshType::Sphere);
	}

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
	// 現在のシーン名取得
	const char* name = SceneManager::Instance().GetCurrentScene()->GetName();

	// シーンがワールドマップ時のノード設定
	if (strcmp(name, "SceneWorldMap") == 0)
	{
		ai_tree->AddNode("", "Root", 0, BehaviorTree::SelectRule::Priority, NULL, NULL);
		ai_tree->AddNode("Root", "Battle", 4, BehaviorTree::SelectRule::Priority, new BattleJudgment(this), NULL);
		ai_tree->AddNode("Root", "Scount", 5, BehaviorTree::SelectRule::Priority, NULL, NULL);
		ai_tree->AddNode("Battle", "Pursuit", 2, BehaviorTree::SelectRule::Non, NULL, new PursuitAction(this));
		ai_tree->AddNode("Scount", "Wander", 1, BehaviorTree::SelectRule::Non, new WanderJudgment(this), new WanderAction(this));
		ai_tree->AddNode("Scount", "Idle", 2, BehaviorTree::SelectRule::Non, NULL, new IdleAction(this));
	}
	else
	{
		ai_tree->AddNode("", "Root", 0, BehaviorTree::SelectRule::Priority, NULL, NULL);
		ai_tree->AddNode("Root", "Death", 1, BehaviorTree::SelectRule::Non, new DeathJudgment(this), new DeathAction(this));
		ai_tree->AddNode("Root", "Damage", 2, BehaviorTree::SelectRule::Non, new DamageJudgment(this), new DamageAction(this));
		ai_tree->AddNode("Root", "Escape", 3, BehaviorTree::SelectRule::Sequence, new EscapeJudgment(this), NULL);
		ai_tree->AddNode("Root", "Battle", 4, BehaviorTree::SelectRule::Priority, new BattleJudgment(this), NULL);
		ai_tree->AddNode("Root", "Scount", 5, BehaviorTree::SelectRule::Priority, NULL, NULL);
		ai_tree->AddNode("Escape", "Leave", 0, BehaviorTree::SelectRule::Non, NULL, new LeaveAction(this));
		ai_tree->AddNode("Escape", "Recover", 0, BehaviorTree::SelectRule::Non, NULL, new RecoverAction(this));
		ai_tree->AddNode("Battle", "Attack", 1, BehaviorTree::SelectRule::Random, new AttackJudgment(this), NULL);
		ai_tree->AddNode("Battle", "Pursuit", 2, BehaviorTree::SelectRule::Non, NULL, new PursuitAction(this));
		//ai_tree->AddNode("Attack", "TailAttack", 0, BehaviorTree::SelectRule::Non, new SkillShotJudgment(this), new SkillShotAction(this));
		ai_tree->AddNode("Scount", "Wander", 1, BehaviorTree::SelectRule::Non, new WanderJudgment(this), new WanderAction(this));
		ai_tree->AddNode("Scount", "Idle", 2, BehaviorTree::SelectRule::Non, NULL, new IdleAction(this));
	}
}

//-----------------------------------------
// 更新処理
//-----------------------------------------
void EnemyLAT::Update(float elapsed_time)
{
	// ビヘイビア更新処理
	BehaviorUpdate(elapsed_time);

	// 速力更新処理
	GetMovement()->UpdateVelocity(elapsed_time);

	// 無敵時間更新処理
	GetCharactor()->UpdateInvincibleTimer(elapsed_time);
}

//-----------------------------------------
// 当たり範囲デバッグプリミティブ描画
//-----------------------------------------
void EnemyLAT::DrawDebugPrimitive()
{
	DebugRenderer* renderer = Graphics::Instance().GetDebugRenderer();
	std::shared_ptr<Actor> actor = GetActor();
	EnemyTerritoryTag teritory_tag = GetBelongingToTerritory();
	std::shared_ptr<EnemyTerritory> enemy_territory = EnemyTerritoryManager::Instance().GetTerritory(teritory_tag);
	DirectX::XMFLOAT3 position = actor->GetPosition();
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
