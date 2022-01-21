#include "ImGuiRenderer.h"
#include "Charactor.h"
#include "Model.h"
#include "Actor.h"
#include "CollisionManager.h"
#include "Telegram.h"
//-----------------------------------------
// ダメージを与える
//-----------------------------------------
bool Charactor::ApplyDamage(int damage, float invincible_time)
{
	// ダメージが0の場合、健康状態を変更する
	if (damage == 0) return false;

	// 死亡している場合は健康状態を変更しない
	if (health <= 0) return false;

	// 無敵時間中はダメージを与えない
	if (invincible_timer > 0.0f) return false;

	// 無敵時間設定
	invincible_timer = invincible_time;

	// ダメージ処理
	health -= damage;

	// 死亡通知
	if (health <= 0)
	{
		OnDead();
	}
	// ダメージ通知
	else
	{
		OnDamaged();
	}

	return true;
}

void Charactor::Start()
{
}

void Charactor::Update(float elapsed_time)
{
}

void Charactor::OnGUI()
{
	ImGui::InputInt("Health", &health);
	ImGui::InputInt("ID", &id);
}

//-----------------------------------------
// ノード座標を取得
//-----------------------------------------
//void Charactor::GetNodePosition(const char* node_name, DirectX::XMFLOAT3& node_position, Model* model)
//{
//	Model::Node* node = model->FindNode(node_name);
//	if (node != nullptr)
//	{
//		node_position.x = node->world_transform._41;
//		node_position.y = node->world_transform._42;
//		node_position.z = node->world_transform._43;
//	}
//}

//-----------------------------------------
// コリジョンの登録
//-----------------------------------------
void Charactor::SetCollision(std::shared_ptr<Actor> actor, CollisionParameter& collision_parameter, CollisionMeshType type)
{
	switch (type)
	{
	case CollisionMeshType::Cylinder:
		{
			// コンポーネント追加
			
			// パラメータを設定
			std::shared_ptr<CollisionCylinder> cylinder = actor->AddComponent<CollisionCylinder>();
			cylinder->SetName(collision_parameter.name);
			cylinder->SetNodeName(collision_parameter.node_name);
			cylinder->SetLocalPosition(collision_parameter.local_position);
			cylinder->SetRadius(collision_parameter.radius);
			cylinder->SetHeight(collision_parameter.height);
			cylinder->SetWeight(collision_parameter.weight);
			cylinder->SetActorID(collision_parameter.actor_id);
			cylinder->SetCollisionFlag(collision_parameter.collision_flg);
			cylinder->SetCollisionElement(collision_parameter.element);
			cylinder->SetPositionMask(collision_parameter.position_mask);
			// マネージャーに追加
			CollisionManager::Instance().RegisterCylinder(cylinder);
		}
		break;
	case CollisionMeshType::Sphere:
		{
			// コンポーネント追加
			
			// パラメータを設定
			std::shared_ptr<CollisionSphere> sphere = actor->AddComponent<CollisionSphere>();
			sphere->SetName(collision_parameter.name);
			sphere->SetNodeName(collision_parameter.node_name);
			sphere->SetPosition(collision_parameter.position);
			sphere->SetLocalPosition(collision_parameter.local_position);
			sphere->SetPositionMask(collision_parameter.position_mask);
			sphere->SetRadius(collision_parameter.radius);
			sphere->SetWeight(collision_parameter.weight);
			sphere->SetActorID(collision_parameter.actor_id);
			sphere->SetCollisionElement(collision_parameter.element);
			sphere->SetCollisionFlag(collision_parameter.collision_flg);
			// マネージャーに追加
			CollisionManager::Instance().RegisterSphere(sphere);
		}
		break;
	case CollisionMeshType::AABB:
	{
		// コンポーネント追加

		// パラメータを設定
		std::shared_ptr<CollisionBox> box = actor->AddComponent<CollisionBox>();
		box->SetName(collision_parameter.name);
		box->SetNodeName(collision_parameter.node_name);
		box->SetPosition(collision_parameter.position);
		box->SetLocalPosition(collision_parameter.local_position);
		box->SetPositionMask(collision_parameter.position_mask);
		box->SetRadius(collision_parameter.float3_radius);
		box->SetWeight(collision_parameter.weight);
		box->SetActorID(collision_parameter.actor_id);
		box->SetCollisionElement(collision_parameter.element);
		box->SetCollisionFlag(collision_parameter.collision_flg);
		// マネージャーに追加
		CollisionManager::Instance().RegisterBox(box);
	}
	break;
	}
}

//-----------------------------------------
// 無敵時間更新
//-----------------------------------------
void Charactor::UpdateInvincibleTimer(float elapsed_time)
{
	// 無敵時間があれば減少させていく
	if (invincible_timer > 0.0f)
	{
		invincible_timer -= elapsed_time;
	}
}

//-----------------------------------------
// アニメーションの範囲区間判定
//-----------------------------------------
bool Charactor::SearchAnimationTime(std::shared_ptr<Actor> actor, float start_time, float end_time)
{
	Model* model = actor->GetModel();
	// アニメーション時間が一定内か調べる
	float animation_time = model->GetCurrentAnimationSeconds();
	return animation_time >= start_time && animation_time <= end_time;
}

//-----------------------------------------
// ヒットストップタイマーの更新処理
//-----------------------------------------
void Charactor::UpdateHitStop(float elapsed_time)
{
	// ヒットストップフラグが立っていなければ処理はしない
	if (!hitstop_flag) return;
	// ヒットストップタイマーが一定時間以上ならフラグをfalseにする
	if (hitstop_timer >= 0.3f)
	{
		hitstop_flag = false;
		hitstop_timer = 0.0f;
		return;
	}
	// ヒットストップタイマー増加
	hitstop_timer += elapsed_time;
}
