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
// �͈͓��Ƀv���C���[�����Ȃ����T��
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
// �ړI�n�_�ֈړ�
void Enemy::MoveToTarget(float elapsed_time, float speed_rate)
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
	movement->Turn(direction, charactor->GetTurnSpeed() * speed_rate);
}

//-----------------------------------------
// �^�[�Q�b�g�ʒu�������_���ݒ�
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
// �A�j���[�V�����Đ�
//-----------------------------------------
void Enemy::PlayAnimation(std::shared_ptr<AnimationData> animation)
{
	// ���f���擾
	Model* model = GetActor()->GetModel();
	// �A�j���[�V�����Đ�
	model->PlayAnimation(animation->number, animation->roop_flag, animation->blend);
}

//-----------------------------------------
// �_���[�W����
//-----------------------------------------
void Enemy::OnDamaged()
{
	GetCharactor()->SetDamageFlag(true);
}

//-----------------------------------------
// ���S����
//-----------------------------------------
void Enemy::OnDead()
{
	GetCharactor()->SetDeathFlag(true);
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
}
