#include "ImGuiRenderer.h"
#include "Charactor.h"
#include "Model.h"
#include "Actor.h"
#include "CollisionManager.h"
#include "Telegram.h"
//-----------------------------------------
// �_���[�W��^����
//-----------------------------------------
bool Charactor::ApplyDamage(int damage, float invincible_time)
{
	// �_���[�W��0�̏ꍇ�A���N��Ԃ�ύX����
	if (damage == 0) return false;

	// ���S���Ă���ꍇ�͌��N��Ԃ�ύX���Ȃ�
	if (health <= 0) return false;

	// ���G���Ԓ��̓_���[�W��^���Ȃ�
	if (invincible_timer > 0.0f) return false;

	// ���G���Ԑݒ�
	invincible_timer = invincible_time;

	// �_���[�W����
	health -= damage;

	// ���S�ʒm
	if (health <= 0)
	{
		OnDead();
	}
	// �_���[�W�ʒm
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
// �m�[�h���W���擾
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
// �R���W�����̓o�^
//-----------------------------------------
void Charactor::SetCollision(std::shared_ptr<Actor> actor, CollisionParameter& collision_parameter, CollisionMeshType type)
{
	switch (type)
	{
	case CollisionMeshType::Cylinder:
		{
			// �R���|�[�l���g�ǉ�
			
			// �p�����[�^��ݒ�
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
			// �}�l�[�W���[�ɒǉ�
			CollisionManager::Instance().RegisterCylinder(cylinder);
		}
		break;
	case CollisionMeshType::Sphere:
		{
			// �R���|�[�l���g�ǉ�
			
			// �p�����[�^��ݒ�
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
			// �}�l�[�W���[�ɒǉ�
			CollisionManager::Instance().RegisterSphere(sphere);
		}
		break;
	case CollisionMeshType::AABB:
	{
		// �R���|�[�l���g�ǉ�

		// �p�����[�^��ݒ�
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
		// �}�l�[�W���[�ɒǉ�
		CollisionManager::Instance().RegisterBox(box);
	}
	break;
	}
}

//-----------------------------------------
// ���G���ԍX�V
//-----------------------------------------
void Charactor::UpdateInvincibleTimer(float elapsed_time)
{
	// ���G���Ԃ�����Ό��������Ă���
	if (invincible_timer > 0.0f)
	{
		invincible_timer -= elapsed_time;
	}
}

//-----------------------------------------
// �A�j���[�V�����͈̔͋�Ԕ���
//-----------------------------------------
bool Charactor::SearchAnimationTime(std::shared_ptr<Actor> actor, float start_time, float end_time)
{
	Model* model = actor->GetModel();
	// �A�j���[�V�������Ԃ����������ׂ�
	float animation_time = model->GetCurrentAnimationSeconds();
	return animation_time >= start_time && animation_time <= end_time;
}

//-----------------------------------------
// �q�b�g�X�g�b�v�^�C�}�[�̍X�V����
//-----------------------------------------
void Charactor::UpdateHitStop(float elapsed_time)
{
	// �q�b�g�X�g�b�v�t���O�������Ă��Ȃ���Ώ����͂��Ȃ�
	if (!hitstop_flag) return;
	// �q�b�g�X�g�b�v�^�C�}�[����莞�Ԉȏ�Ȃ�t���O��false�ɂ���
	if (hitstop_timer >= 0.3f)
	{
		hitstop_flag = false;
		hitstop_timer = 0.0f;
		return;
	}
	// �q�b�g�X�g�b�v�^�C�}�[����
	hitstop_timer += elapsed_time;
}
