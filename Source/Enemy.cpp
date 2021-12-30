#include "Mathf.h"

#include "Charactor.h"
#include "ActorManager.h"
#include "Enemy.h"

#include "BehaviorTree.h"
#include "BehaviorData.h"
#include "NodeBase.h"

#include "ImGuiRenderer.h"

Enemy::Enemy()
{
}
Enemy::~Enemy()
{
	delete ai_tree;
	delete behavior_data;
}
// 範囲内にプレイヤーがいないか探す
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
// 目的地点へ移動
void Enemy::MoveToTarget(float elapsed_time, float speed_rate)
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
	movement->Turn(direction, charactor->GetTurnSpeed() * speed_rate);
}

//-----------------------------------------
// ターゲット位置をランダム設定
//-----------------------------------------
void Enemy::SetRandomTargetPosition()
{
	float theta = Mathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
	float range = Mathf::RandomRange(0.0f, territory_range);
	target_position.x = territory_origin.x + sinf(theta) * range;
	target_position.y = territory_origin.y;
	target_position.z = territory_origin.z + cosf(theta) * range;
}

//-----------------------------------------
// アニメーション再生
//-----------------------------------------
void Enemy::PlayAnimation(std::shared_ptr<AnimationData> animation)
{
	// モデル取得
	Model* model = GetActor()->GetModel();
	// アニメーション再生
	model->PlayAnimation(animation->number, animation->roop_flag, animation->blend);
}

//-----------------------------------------
// ダメージ処理
//-----------------------------------------
void Enemy::OnDamaged()
{
	GetCharactor()->SetDamageFlag(true);
}

//-----------------------------------------
// 死亡処理
//-----------------------------------------
void Enemy::OnDead()
{
	GetCharactor()->SetDeathFlag(true);
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
}
