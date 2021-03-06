#include <imgui.h>
#include "Actor.h"
#include "ActorManager.h"
#include "Component.h"
#include "Charactor.h"
#include "Graphics.h"
#include "Model.h"
#include "CullingCollision.h"

//------------------------------
// 開始処理
//------------------------------
void Actor::Start()
{
	// コンポーネントの更新処理
	for (std::shared_ptr<Component>& component : components)
	{
		component->Start();
	}
}

//------------------------------
// 更新処理
//------------------------------
void Actor::Update(float elapsed_time)
{
	// アニメーション更新
	if (model != nullptr && animation_flag)
	{
		bool hitstop_flag = false;
		
		// キャラクターコンポーネントを取得
		std::shared_ptr<Charactor> charactor = nullptr;
		charactor = GetComponent<Charactor>();

		// キャラクターコンポーネントを取得出来たら
		if (charactor != nullptr)
		{
			// ヒットストップフラグを取得
			hitstop_flag = charactor->GetHitStopFlag();
		}
		model->UpdateAnimation(elapsed_time, hitstop_flag);
	}

	// 更新処理
	for (std::shared_ptr<Component>& component : components)
	{
		component->Update(elapsed_time);
	}
}

//------------------------------
// 行列更新処理
//------------------------------
void Actor::UpdateTransform()
{
	// ワールド行列更新処理
	DirectX::XMVECTOR Q = DirectX::XMLoadFloat4(&rotation);
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(Q);
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x , position.y, position.z);
	DirectX::XMMATRIX W = S * R * T;
	DirectX::XMStoreFloat4x4(&transform, W);

	// モデル行列更新処理
	if (model != nullptr)
	{
		model->UpdateTransform(transform);
	}
}

//------------------------------
// IMGUI表示
//------------------------------
void Actor::OnGUI()
{
	// 名前
	{
		char buffer[256];
		::strncpy_s(buffer, sizeof(buffer), GetName(), sizeof(buffer));
		if (ImGui::InputText("Name", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			SetName(buffer);
		}
	}

	// トランスフォーム
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::InputFloat3("Position", &position.x);
		ImGui::InputFloat3("Rotation", &rotation.x);
		ImGui::InputFloat3("Angle", &angle.x);
		ImGui::InputFloat3("Scale", &scale.x);
	}

	// コンポーネント
	for (std::shared_ptr<Component>& component : components)
	{
		ImGui::Spacing();
		ImGui::Separator();

		if (ImGui::CollapsingHeader(component->GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			component->OnGUI();
		}
	}
}

//------------------------------
// モデルのセットアップ
//------------------------------
void Actor::SetUpModel(const char* filename, const char* ignore_root_motion_node_name)
{
	model = std::make_unique<Model>(filename, ignore_root_motion_node_name);
}

//------------------------------
// 角度の設定
//------------------------------
void Actor::SetAngle(const DirectX::XMFLOAT3& angle)
{
	this->angle = angle;

	DirectX::XMVECTOR Q = DirectX::XMQuaternionRotationRollPitchYaw(this->angle.x, this->angle.y, this->angle.z);
	DirectX::XMStoreFloat4(&rotation, Q);
}

//------------------------------
// 角度の加算
//------------------------------
void Actor::AddAngle(const DirectX::XMFLOAT3& add_angle)
{
	this->angle.x += add_angle.x;
	this->angle.y += add_angle.y;
	this->angle.z += add_angle.z;
	DirectX::XMVECTOR Q = DirectX::XMQuaternionRotationRollPitchYaw(this->angle.x, this->angle.y, this->angle.z);
	DirectX::XMStoreFloat4(&rotation, Q);
}

//------------------------------
// 角度の減算
//------------------------------
void Actor::SubtractAngle(const DirectX::XMFLOAT3& sub_angle)
{
	this->angle.x -= sub_angle.x;
	this->angle.y -= sub_angle.y;
	this->angle.z -= sub_angle.z;
	DirectX::XMVECTOR Q = DirectX::XMQuaternionRotationRollPitchYaw(this->angle.x, this->angle.y, this->angle.z);
	DirectX::XMStoreFloat4(&rotation, Q);
}
