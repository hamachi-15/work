// 描画
#include "Graphics.h"
#include "DebugRenderer.h"

// ゲームデータ
#include "GameDatabase.h"
#include "AnimationData.h"

// 算術関数
#include "Mathf.h"

// コンポーネント
#include "Charactor.h"
#include "Enemy.h"

// マネージャー
#include "ActorManager.h"
#include "EnemyTerritoryManager.h"

// AI
#include "BehaviorTree.h"
#include "BehaviorData.h"
#include "NodeBase.h"

// ImGui
#include "ImGuiRenderer.h"

//-----------------------------------------
// コンストラクタ
//-----------------------------------------
Enemy::Enemy()
{
}

//-----------------------------------------
// デストラクタ
//-----------------------------------------
Enemy::~Enemy()
{
	delete ai_tree;
	delete behavior_data;
}

//-----------------------------------------
// ビヘイビア更新処理
//-----------------------------------------
void Enemy::BehaviorUpdate(float elapsed_time)
{
	// ビヘイビアツリー更新処理
	if (active_node == nullptr)
	{
		active_node = ai_tree->ActiveNodeInference(this, behavior_data);
	}
	if (active_node != nullptr && active_node != old_active_node)
	{
		ai_tree->Start(active_node);
		old_active_node = active_node;
	}
	if (active_node != nullptr)
	{
		active_node = ai_tree->Run(this, active_node, behavior_data, elapsed_time);
	}
}

//-----------------------------------------
// 当たり範囲デバッグプリミティブ描画
//-----------------------------------------
void Enemy::DrawDebugPrimitive()
{
	// デバックレンダラ取得
	DebugRenderer* renderer = Graphics::Instance().GetDebugRenderer();
	// アクター取得
	std::shared_ptr<Actor> actor = GetActor();
	// 縄張りのタグ取得
	EnemyTerritoryTag teritory_tag = GetBelongingToTerritory();
	// 取得したタグからテリトリーデータ取得
	std::shared_ptr<EnemyTerritory> enemy_territory = EnemyTerritoryManager::Instance().GetTerritory(teritory_tag);
	// アクターの座標取得
	DirectX::XMFLOAT3 position = actor->GetPosition();

	// 縄張り範囲をデバッグ円柱描画
	renderer->DrawCylinder(enemy_territory->GetTerritoryOrigin(), enemy_territory->GetTerritoryRange(), 1.0f, primitive_color_green);

	// 索敵範囲をデバッグ円柱描画
	renderer->DrawCylinder(position, search_range, 1.0f, primitive_color_blue);

	// 攻撃範囲をデバッグ円柱描画
	renderer->DrawCylinder(position, GetAttackRange(), 1.0f, primitive_color_purple);

	// ターゲット座標の球描画
	renderer->DrawSphere(target_position, 0.5f, primitive_color_red);
}

//-----------------------------------------
// 範囲内にプレイヤーがいないか探す
//-----------------------------------------
bool Enemy::SearchPlayer()
{
	// プレイヤーとの高低差を考慮して3Dで距離判定をする
	const DirectX::XMFLOAT3& player_position = ActorManager::Instance().GetActor("Player")->GetPosition();
	const DirectX::XMFLOAT3& position = GetActor()->GetPosition();
	float vx = player_position.x - position.x;
	float vy = player_position.y - position.y;
	float vz = player_position.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);

	if (dist < search_range)
	{
		float distXZ = sqrtf(vx * vx + vz * vz);
		// 単位ベクトル化
		vx /= distXZ;
		vz /= distXZ;

		// 方向ベクトル化
		DirectX::XMFLOAT3 angle = GetActor()->GetAngle();
		float frontX = sinf(angle.y);
		float frontZ = cosf(angle.y);
		// 2つのベクトルの内積値で前後判定
		float dot = (frontX * vx) + (frontZ * vz);
		if (dot > 0.0f)
		{
			return true;
		}
	}
	return false;
}

//-----------------------------------------
// 目的地点へ移動
//-----------------------------------------
void Enemy::MoveToTarget(float speed_rate)
{
	// ターゲット方向への進行ベクトルを算出
	DirectX::XMFLOAT3 position = GetActor()->GetPosition();
	float vx = target_position.x - position.x;
	float vz = target_position.z - position.z;
	float dist = sqrtf(vx * vx + vz * vz);
	vx /= dist;
	vz /= dist;
	DirectX::XMFLOAT3 direction = { vx, 0, vz };
	// 移動処理
	movement->Move(direction, charactor->GetMoveSpeed() * speed_rate);
	movement->Turn(direction);
}

//-----------------------------------------
// 指定の方向へ移動
//-----------------------------------------
void Enemy::MoveToDirection(DirectX::XMFLOAT3& direction, float speed_rate)
{
	// 移動処理
	movement->Move(direction, charactor->GetMoveSpeed() * speed_rate);
	movement->Turn(direction);
}

//-----------------------------------------
// 目標のベクトルへ向く
//-----------------------------------------
float Enemy::TurnToTarget(DirectX::XMFLOAT3& direction)
{
	return movement->Turn(direction);
}

//-----------------------------------------
// ターゲット位置をランダム設定
//-----------------------------------------
void Enemy::SetRandomTargetPosition()
{
	// テリトリーデータ取得
	std::shared_ptr<EnemyTerritory> territory_data = EnemyTerritoryManager::Instance().GetTerritory(GetBelongingToTerritory());
	// データからテリトリーの半径取得
	float territory_range = territory_data->GetTerritoryRange();
	// データからテリトリーの原点取得
	DirectX::XMFLOAT3 territory_origin = territory_data->GetTerritoryOrigin();

	// ターゲットポジションをテリトリー範囲内のランダムな位置に設定
	float theta = Mathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
	float range = Mathf::RandomRange(0.0f, territory_range);

	target_position.x = territory_origin.x + sinf(theta) * range;
	target_position.y = territory_origin.y;
	target_position.z = territory_origin.z + cosf(theta) * range;
}

//-----------------------------------------
// アニメーション再生
//-----------------------------------------
void Enemy::PlayAnimation(const char* animation_name)
{
	// モデル取得
	Model* model = GetActor()->GetModel();
	
	// アニメーションデータ取得
	std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData(animation_name);
	if (!animation)
	{
		return;
	}

	// アニメーション再生
	model->PlayAnimation(animation->number, animation->roop_flag, animation->blend);
}

//-----------------------------------------
// ビヘイビアのGUI描画
//-----------------------------------------
void Enemy::DrawBehaviorGUI()
{
	// ビヘイビア関連情報
	if (ImGui::CollapsingHeader("BehaviorTree"))
	{
		ImGui::TextColored(ImVec4(1, 0, 1, 1), u8"-------アクティブになっているノード------");
		std::string child_str = "";
		std::string parent_str = "";
		if (active_node != nullptr)
		{
			parent_str = active_node->GetParent()->GetName();
			child_str = active_node->GetName();
		}
		ImGui::Text(u8"ActiveParentNode　%s", parent_str.c_str());
		ImGui::Text(u8"ActiveChildNode　%s", child_str.c_str());

		ImGui::TextColored(ImVec4(1, 0, 1, 1), u8"-------ノードツリー-------");
		ai_tree->DrawNodeGUI();
	}

	ImGui::Checkbox("AttackRight", &rightofattack);
}
