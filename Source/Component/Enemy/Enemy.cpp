// �`��
#include "Graphics.h"
#include "DebugRenderer.h"

// �Q�[���f�[�^
#include "GameDatabase.h"
#include "AnimationData.h"

// �Z�p�֐�
#include "Mathf.h"

// �R���|�[�l���g
#include "Charactor.h"
#include "Enemy.h"

// �}�l�[�W���[
#include "ActorManager.h"
#include "EnemyTerritoryManager.h"

// AI
#include "BehaviorTree.h"
#include "BehaviorData.h"
#include "NodeBase.h"

// ImGui
#include "ImGuiRenderer.h"

//-----------------------------------------
// �R���X�g���N�^
//-----------------------------------------
Enemy::Enemy()
{
}

//-----------------------------------------
// �f�X�g���N�^
//-----------------------------------------
Enemy::~Enemy()
{
	delete ai_tree;
	delete behavior_data;
}

//-----------------------------------------
// �r�w�C�r�A�X�V����
//-----------------------------------------
void Enemy::BehaviorUpdate(float elapsed_time)
{
	// �r�w�C�r�A�c���[�X�V����
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
// ������͈̓f�o�b�O�v���~�e�B�u�`��
//-----------------------------------------
void Enemy::DrawDebugPrimitive()
{
	// �f�o�b�N�����_���擾
	DebugRenderer* renderer = Graphics::Instance().GetDebugRenderer();
	// �A�N�^�[�擾
	std::shared_ptr<Actor> actor = GetActor();
	// �꒣��̃^�O�擾
	EnemyTerritoryTag teritory_tag = GetBelongingToTerritory();
	// �擾�����^�O����e���g���[�f�[�^�擾
	std::shared_ptr<EnemyTerritory> enemy_territory = EnemyTerritoryManager::Instance().GetTerritory(teritory_tag);
	// �A�N�^�[�̍��W�擾
	DirectX::XMFLOAT3 position = actor->GetPosition();

	// �꒣��͈͂��f�o�b�O�~���`��
	renderer->DrawCylinder(enemy_territory->GetTerritoryOrigin(), enemy_territory->GetTerritoryRange(), 1.0f, primitive_color_green);

	// ���G�͈͂��f�o�b�O�~���`��
	renderer->DrawCylinder(position, search_range, 1.0f, primitive_color_blue);

	// �U���͈͂��f�o�b�O�~���`��
	renderer->DrawCylinder(position, GetAttackRange(), 1.0f, primitive_color_purple);

	// �^�[�Q�b�g���W�̋��`��
	renderer->DrawSphere(target_position, 0.5f, primitive_color_red);
}

//-----------------------------------------
// �͈͓��Ƀv���C���[�����Ȃ����T��
//-----------------------------------------
bool Enemy::SearchPlayer()
{
	// �v���C���[�Ƃ̍��፷���l������3D�ŋ������������
	const DirectX::XMFLOAT3& player_position = ActorManager::Instance().GetActor("Player")->GetPosition();
	const DirectX::XMFLOAT3& position = GetActor()->GetPosition();
	float vx = player_position.x - position.x;
	float vy = player_position.y - position.y;
	float vz = player_position.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);

	if (dist < search_range)
	{
		float distXZ = sqrtf(vx * vx + vz * vz);
		// �P�ʃx�N�g����
		vx /= distXZ;
		vz /= distXZ;

		// �����x�N�g����
		DirectX::XMFLOAT3 angle = GetActor()->GetAngle();
		float frontX = sinf(angle.y);
		float frontZ = cosf(angle.y);
		// 2�̃x�N�g���̓��ϒl�őO�㔻��
		float dot = (frontX * vx) + (frontZ * vz);
		if (dot > 0.0f)
		{
			return true;
		}
	}
	return false;
}

//-----------------------------------------
// �ړI�n�_�ֈړ�
//-----------------------------------------
void Enemy::MoveToTarget(float speed_rate)
{
	// �^�[�Q�b�g�����ւ̐i�s�x�N�g�����Z�o
	DirectX::XMFLOAT3 position = GetActor()->GetPosition();
	float vx = target_position.x - position.x;
	float vz = target_position.z - position.z;
	float dist = sqrtf(vx * vx + vz * vz);
	vx /= dist;
	vz /= dist;
	DirectX::XMFLOAT3 direction = { vx, 0, vz };
	// �ړ�����
	movement->Move(direction, charactor->GetMoveSpeed() * speed_rate);
	movement->Turn(direction);
}

//-----------------------------------------
// �w��̕����ֈړ�
//-----------------------------------------
void Enemy::MoveToDirection(DirectX::XMFLOAT3& direction, float speed_rate)
{
	// �ړ�����
	movement->Move(direction, charactor->GetMoveSpeed() * speed_rate);
	movement->Turn(direction);
}

//-----------------------------------------
// �ڕW�̃x�N�g���֌���
//-----------------------------------------
float Enemy::TurnToTarget(DirectX::XMFLOAT3& direction)
{
	return movement->Turn(direction);
}

//-----------------------------------------
// �^�[�Q�b�g�ʒu�������_���ݒ�
//-----------------------------------------
void Enemy::SetRandomTargetPosition()
{
	// �e���g���[�f�[�^�擾
	std::shared_ptr<EnemyTerritory> territory_data = EnemyTerritoryManager::Instance().GetTerritory(GetBelongingToTerritory());
	// �f�[�^����e���g���[�̔��a�擾
	float territory_range = territory_data->GetTerritoryRange();
	// �f�[�^����e���g���[�̌��_�擾
	DirectX::XMFLOAT3 territory_origin = territory_data->GetTerritoryOrigin();

	// �^�[�Q�b�g�|�W�V�������e���g���[�͈͓��̃����_���Ȉʒu�ɐݒ�
	float theta = Mathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
	float range = Mathf::RandomRange(0.0f, territory_range);

	target_position.x = territory_origin.x + sinf(theta) * range;
	target_position.y = territory_origin.y;
	target_position.z = territory_origin.z + cosf(theta) * range;
}

//-----------------------------------------
// �A�j���[�V�����Đ�
//-----------------------------------------
void Enemy::PlayAnimation(const char* animation_name)
{
	// ���f���擾
	Model* model = GetActor()->GetModel();
	
	// �A�j���[�V�����f�[�^�擾
	std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData(animation_name);
	if (!animation)
	{
		return;
	}

	// �A�j���[�V�����Đ�
	model->PlayAnimation(animation->number, animation->roop_flag, animation->blend);
}

//-----------------------------------------
// �r�w�C�r�A��GUI�`��
//-----------------------------------------
void Enemy::DrawBehaviorGUI()
{
	// �r�w�C�r�A�֘A���
	if (ImGui::CollapsingHeader("BehaviorTree"))
	{
		ImGui::TextColored(ImVec4(1, 0, 1, 1), u8"-------�A�N�e�B�u�ɂȂ��Ă���m�[�h------");
		std::string child_str = "";
		std::string parent_str = "";
		if (active_node != nullptr)
		{
			parent_str = active_node->GetParent()->GetName();
			child_str = active_node->GetName();
		}
		ImGui::Text(u8"ActiveParentNode�@%s", parent_str.c_str());
		ImGui::Text(u8"ActiveChildNode�@%s", child_str.c_str());

		ImGui::TextColored(ImVec4(1, 0, 1, 1), u8"-------�m�[�h�c���[-------");
		ai_tree->DrawNodeGUI();
	}

	ImGui::Checkbox("AttackRight", &rightofattack);
}