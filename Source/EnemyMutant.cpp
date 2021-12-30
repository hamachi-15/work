#include "Model.h"

#include "EnemyMutant.h"
#include "EnemyManager.h"
#include "Graphics.h"
#include "BehaviorTree.h"
#include "BehaviorData.h"
#include "NodeBase.h"
#include "JudgmentOwner.h"
#include "BossJudgmenetOwner.h"
#include "ActionOwner.h"
#include "BossActionOwner.h"
#include "Charactor.h"
//---------------------------------------
// コンストラクタ
//---------------------------------------
EnemyMutant::EnemyMutant()
{
	// 名前設定
	SetName("Mutant");
}

//---------------------------------------
// デストラクタ
//---------------------------------------
EnemyMutant::~EnemyMutant()
{
}

//---------------------------------------
// GUI描画
//---------------------------------------
void EnemyMutant::OnGUI()
{
	// ビヘイビア関連情報
	DrawBehaviorGUI();
}

//-----------------------------------------
// 敵の破棄処理
//-----------------------------------------
void EnemyMutant::Destroy()
{
}

//---------------------------------------
// 開始処理
//---------------------------------------
void EnemyMutant::Start()
{
	// アクターの取得
	std::shared_ptr<Actor> actor = GetActor();
	std::shared_ptr<Charactor> charactor = actor->GetComponent<Charactor>();


	// ムーブメントコンポーネントの設定
	SetMovement(actor->GetComponent<Movement>());

	// キャラクターコンポーネントの設定
	SetCharactor(charactor);
	// マネージャーに登録
	EnemyManager::Instance().EnemyRegister(actor->GetComponent<EnemyMutant>());

	// 索敵範囲の設定
	SetSearchRange(40.0f);
	// 攻撃範囲の設定
	SetAttackRange(5.0f);
	// テリトリー範囲の設定
	SetTerritoryOrigin(actor->GetPosition());
	// 最初はターゲット座標を自身の座標に設定
	SetTargetPosition(actor->GetPosition());

	{
		Model* model = actor->GetModel();
		CollisionParameter parameter;
		parameter.name = GetName();
		parameter.node_name = "";
		parameter.actor_id = charactor->GetID();
		parameter.position = { 0, 0, 0 };
		parameter.radius = 4.5f;
		parameter.height = 7.0f;
		parameter.weight = 6.5f;
		parameter.collision_flg = true;
		parameter.element = CollisionElement::Body;
		parameter.position_mask = CollisionPositionMask::Collision_Mask_Actor_Position;
		charactor->SetCollision(actor, parameter, CollisionMeshType::Cylinder);


		DirectX::XMFLOAT3 position;
		charactor->GetNodePosition("Mutant:RightHandIndex1", position, model);
		parameter.name = "MutantRightHand";
		parameter.node_name = "Mutant:RightHandIndex1";
		parameter.actor_id = charactor->GetID();
		parameter.position = position;
		parameter.radius = 3.0f;
		parameter.weight = 2.0f;
		parameter.collision_flg = true;
		parameter.element = CollisionElement::Weppon;
		parameter.position_mask = CollisionPositionMask::Collision_Mask_Member_Position;
		charactor->SetCollision(actor, parameter, CollisionMeshType::Sphere);

		// 左手のコリジョン
		charactor->GetNodePosition("Mutant:LeftHand", position, model);
		parameter.name = "MutantLeftHand";
		parameter.node_name = "Mutant:LeftHand";
		parameter.position = position;
		parameter.collision_flg = true;
		charactor->SetCollision(actor, parameter, CollisionMeshType::Sphere);

	}

	// ビヘイビアツリー設定
	behavior_data = new BehaviorData();
	ai_tree = new BehaviorTree();

	ai_tree->AddNode("", "Root", 0, BehaviorTree::SelectRule::Priority, NULL, NULL);
	ai_tree->AddNode("Root", "Death", 1, BehaviorTree::SelectRule::Non, new DeathJudgment(this), new DeathAction(this));
	ai_tree->AddNode("Root", "Damage", 2, BehaviorTree::SelectRule::Non, new DamageJudgment(this), new DamageAction(this));
	ai_tree->AddNode("Root", "Escape", 3, BehaviorTree::SelectRule::Sequence, new EscapeJudgment(this), NULL);
	ai_tree->AddNode("Root", "Battle", 4, BehaviorTree::SelectRule::Priority, new BattleJudgment(this), NULL);
	ai_tree->AddNode("Root", "Scount", 5, BehaviorTree::SelectRule::Priority, NULL, NULL);
	//	ai_tree->AddNode("Escape", "Recover", 0, BehaviorTree::SelectRule::Non, NULL, new RecoverAction(this));
	ai_tree->AddNode("Battle", "Attack", 1, BehaviorTree::SelectRule::Random, new AttackJudgment(this), NULL);
	ai_tree->AddNode("Battle", "Pursuit", 2, BehaviorTree::SelectRule::Non, NULL, new PursuitAction(this));
	ai_tree->AddNode("Attack", "Punch", 0, BehaviorTree::SelectRule::Non, new PunchAttackJudgment(this), new PunchAttackAction(this));
	ai_tree->AddNode("Attack", "Swiping", 0, BehaviorTree::SelectRule::Non, new SwipingAttackJudgmenet(this), new SwipingAttackAction(this));
	//ai_tree->AddNode("Attack", "JumpAttack", 0, BehaviorTree::SelectRule::Non, new JumpAttackJudgmenet(this), new JumpAttackAction(this));
	ai_tree->AddNode("Scount", "Wander", 1, BehaviorTree::SelectRule::Non, new WanderJudgment(this), new WanderAction(this));
	ai_tree->AddNode("Scount", "Idle", 2, BehaviorTree::SelectRule::Non, NULL, new IdleAction(this));
}

//-----------------------------------------
// ビヘイビアのノード設定処理
//-----------------------------------------
void EnemyMutant::SetBehaviorNode()
{
}

//---------------------------------------
// 更新処理
//---------------------------------------
void EnemyMutant::Update(float elapsed_time)
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

void EnemyMutant::DrawDebugPrimitive()
{
	DebugRenderer* renderer = Graphics::Instance().GetDebugRenderer();
	std::shared_ptr<Actor> actor = GetActor();
	DirectX::XMFLOAT3 position = actor->GetPosition();
	float territory_range = GetTerritoryRange();
	DirectX::XMFLOAT3 territory_origin = GetTerritoryOrigin();
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