#include <imgui.h>
#include "Actor.h"
#include "ActorManager.h"
#include "Component.h"
#include "Charactor.h"
#include "Graphics.h"

//------------------------------
// �J�n����
//------------------------------
void Actor::Start()
{
	// �R���|�[�l���g�̍X�V����
	for (std::shared_ptr<Component>& component : components)
	{
		component->Start();
	}
}

//------------------------------
// �X�V����
//------------------------------
void Actor::Update(float elapsed_time)
{
	// �A�j���[�V�����X�V
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
	// �X�V����
	for (std::shared_ptr<Component>& component : components)
	{
		component->Update(elapsed_time);
	}
}

//------------------------------
// �`�揈��
//------------------------------
void Actor::Draw(ID3D11DeviceContext* context)
{
	shader->Draw(context, model.get());
}

//------------------------------
// �s��X�V����
//------------------------------
void Actor::UpdateTransform()
{
	// ���[���h�s��X�V����
	DirectX::XMVECTOR Q = DirectX::XMLoadFloat4(&rotation);
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(Q);
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x , position.y, position.z);
	DirectX::XMMATRIX W = S * R * T;
	DirectX::XMStoreFloat4x4(&transform, W);

	// ���f���s��X�V����
	if (model != nullptr)
	{
		model->UpdateTransform(transform);
	}
}

//------------------------------
// IMGUI�\��
//------------------------------
void Actor::OnGUI()
{
	// ���O
	{
		char buffer[256];
		::strncpy_s(buffer, sizeof(buffer), GetName(), sizeof(buffer));
		if (ImGui::InputText("Name", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			SetName(buffer);
		}
	}

	// �g�����X�t�H�[��
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
	// �R���|�[�l���g
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
// ���f���̃Z�b�g�A�b�v
//------------------------------
void Actor::SetUpModel(const char* filename)
{
	model = std::make_unique<Model>(filename);
}

//------------------------------
// �p�x�̐ݒ�
//------------------------------
void Actor::SetAngle(const DirectX::XMFLOAT3& angle)
{
	this->angle = angle;

	DirectX::XMVECTOR Q = DirectX::XMQuaternionRotationRollPitchYaw(this->angle.x, this->angle.y, this->angle.z);
	DirectX::XMStoreFloat4(&rotation, Q);
}

//------------------------------
// �p�x�̉��Z
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
// �p�x�̌��Z
//------------------------------
void Actor::SubtractAngle(const DirectX::XMFLOAT3& sub_angle)
{
	this->angle.x -= sub_angle.x;
	this->angle.y -= sub_angle.y;
	this->angle.z -= sub_angle.z;
	DirectX::XMVECTOR Q = DirectX::XMQuaternionRotationRollPitchYaw(this->angle.x, this->angle.y, this->angle.z);
	DirectX::XMStoreFloat4(&rotation, Q);
}
