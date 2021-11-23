#include "Enemy.h"
#include "Mathf.h"
#include "Charactor.h"
#include "ActorManager.h"
#include "BehaviorTree.h"
#include "BehaviorData.h"
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
