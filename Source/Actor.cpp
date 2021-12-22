#include <imgui.h>
#include "Actor.h"
#include "ActorManager.h"
#include "Component.h"
#include "Charactor.h"
#include "Graphics.h"

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
		std::shared_ptr<Charactor> charactor = GetComponent<Charactor>();
		if (charactor != nullptr)
		{
			hitstop_flag = charactor->GetHitStopFlag();
		}
		model->UpdateAnimation(elapsed_time, animation_node_origin.c_str(), hitstop_flag);
	}
	// 更新処理
	for (std::shared_ptr<Component>& component : components)
	{
		component->Update(elapsed_time);
	}
}

//------------------------------
// 描画処理
//------------------------------
void Actor::Draw(ID3D11DeviceContext* context)
{
	shader->Draw(context, model.get());
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

	//if (ImGui::CollapsingHeader("Shader", ImGuiTreeNodeFlags_DefaultOpen))
	//{
	//	ImGui::Text(shader->GetShaderName());
	//	
	//}
	// コンポーネント
	for (std::shared_ptr<Component>& component : components)
	{
		ImGui::Spacing();
		ImGui::Separator();

		if (ImGui::CollapsingHeader(component->GetName(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			component->OnGUI();
		}
	}
}

//------------------------------
// モデルのセットアップ
//------------------------------
void Actor::SetUpModel(const char* filename)
{
	model = std::make_unique<Model>(filename);
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
