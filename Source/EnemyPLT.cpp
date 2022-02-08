
#include "Model.h"

#include "Graphics.h"

#include "NodeBase.h"
#include "BehaviorData.h"
#include "BehaviorTree.h"
#include "JudgmentOwner.h"
#include "PLTJudgmentOwner.h"
#include "ActionOwner.h"
#include "PLTActionOwner.h"

#include "DebugRenderer.h"

#include "Charactor.h"
#include "EnemyPLT.h"

#include "SceneManager.h"
#include "ActorManager.h"
#include "EnemyManager.h"
#include "EnemyTerritoryManager.h"
#include "CollisionManager.h"
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

	// コリジョン削除
	// 球コリジョン削除
	//CollisionManager::Instance().UnregisterSphere(CollisionManager::Instance().GetCollisionSphereFromName(right_hand_collision_name.c_str()));
	//
	//// 円柱コリジョン削除
	//CollisionManager::Instance().UnregisterCylinder(CollisionManager::Instance().GetCollisionCylinderFromName(actor->GetName()));
	//
	//// 立方体コリジョン削除
	//CollisionManager::Instance().UnregisterBox(CollisionManager::Instance().GetCollisionBoxFromName(actor->GetName()));

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
	bool attack_flag = GetAttackFlag();
	ImGui::Checkbox("AttackFlag", &attack_flag);

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
		//ダメージフラグをオンに
		OnDamaged();
		// 衝突した位置を設定
		SetHitPosition(message.message_box.hit_position);
		break;
	case MessageType::Message_Hit_Boddy:
		break;
	case MessageType::Message_Give_Attack_Right:
		SetAttackFlag(true);
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

	// キャラクターコンポーネント取得
	std::shared_ptr<Charactor> charactor = actor->GetComponent<Charactor>();

	// 名前設定
	SetName("PLT");

	// ムーブメントコンポーネントの設定
	SetMovement(actor->GetComponent<Movement>());

	// キャラクターコンポーネントの設定
	SetCharactor(charactor);

	// マネージャーに登録
	EnemyManager::Instance().EnemyRegister(actor->GetComponent<EnemyPLT>());

	// 索敵範囲の設定
	SetSearchRange(30.0f);

	// 攻撃範囲の設定
	SetAttackRange(15.0f);


	// 最初はターゲット座標を自身の座標に設定
	SetTargetPosition(actor->GetPosition());

	// コリジョン設定
	//{
	//	// モデル取得
	//	Model* model = GetActor()->GetModel();

	//	CollisionParameter parameter;
	//	std::vector<CollisionParameter> sphere_parameter;
	//	std::vector<CollisionParameter> cylinder_parameter;
	//	// カリング用のコリジョン
	//	parameter.name = "PLTAABB";
	//	parameter.actor_name = actor->GetName();
	//	parameter.node_name = "PLT:Chest_M_BK";
	//	parameter.position = {};
	//	parameter.xmfloat_radius = DirectX::XMFLOAT3(5.5f, 13.0f, 5.5f);
	//	parameter.height = 9.0f;
	//	parameter.collision_flg = true;
	//	parameter.actor_id = charactor->GetID() + GetIdentity();
	//	parameter.actor_type = CollisionActorType::Enemy;
	//	parameter.update_type = CollisionUpdateType::Update_Node_Position;

	//	// 体のコリジョン設定
	//	parameter.name = actor->GetName();
	//	parameter.node_name = "";
	//	parameter.radius = 5.0f;
	//	parameter.height = 15.0f;
	//	parameter.weight = 10.0f;
	//	parameter.collision_flg = true;
	//	parameter.actor_type = CollisionActorType::Enemy;
	//	parameter.update_type = CollisionUpdateType::Update_Actor;
	//	cylinder_parameter.emplace_back(parameter);

	//	// 右手のコリジョン設定
	//	right_hand_collision_name = actor->GetName();
	//	right_hand_collision_name += "RightHand";
	//	parameter.name = right_hand_collision_name.c_str();
	//	parameter.node_name = "PLT:MiddleFinger2_R_BK";
	//	parameter.radius = 6.0f;
	//	parameter.weight = 1.0f;
	//	parameter.collision_flg = false;
	//	parameter.actor_type = CollisionActorType::Enemy;
	//	parameter.update_type = CollisionUpdateType::Update_Node_Position;
	//	sphere_parameter.emplace_back(parameter);

	//	actor->AddComponent<EnemyCollision>(sphere_parameter, cylinder_parameter);
	//}
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
	// 現在のシーン名取得
	std::string scene_name = SceneManager::Instance().GetCurrentScene()->GetName();

	// シーンがワールドマップ時のノード設定
	if (scene_name == "SceneWorldMap")
	{
		ai_tree->AddNode("", "Root", 0, BehaviorTree::SelectRule::Priority, NULL, NULL);
		ai_tree->AddNode("Root", "Battle", 1, BehaviorTree::SelectRule::Priority, new BattleJudgment(this), NULL);
		ai_tree->AddNode("Root", "Scount", 2, BehaviorTree::SelectRule::Priority, NULL, NULL);
		ai_tree->AddNode("Battle", "Pursuit", 1, BehaviorTree::SelectRule::Non, NULL, new PursuitAction(this));
		ai_tree->AddNode("Scount", "Wander", 1, BehaviorTree::SelectRule::Non, new WanderJudgment(this), new WanderAction(this));
		ai_tree->AddNode("Scount", "Idle", 2, BehaviorTree::SelectRule::Non, NULL, new IdleAction(this));

	} // シーンがバトルシーンの時のノード設定
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
		ai_tree->AddNode("Attack", "BlowAttack", 0, BehaviorTree::SelectRule::Non, new BlowJudgment(this), new BlowAttackAction(this));
		ai_tree->AddNode("Scount", "Wander", 1, BehaviorTree::SelectRule::Non, new WanderJudgment(this), new WanderAction(this));
		ai_tree->AddNode("Scount", "Idle", 2, BehaviorTree::SelectRule::Non, NULL, new IdleAction(this));

	}
}

//-----------------------------------------
// 更新処理
//-----------------------------------------
void EnemyPLT::Update(float elapsed_time)
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
void EnemyPLT::DrawDebugPrimitive()
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