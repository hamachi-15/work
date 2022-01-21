#include "ActionBase.h"
#include "CollisionManager.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "GameDatabase.h"
#include "Charactor.h"

// �ėp�֐�
#include "Universal.h"
//************************************
// 
// �s���������N���X
// 
//************************************
//-----------------------------------
// �U���̓����蔻�菈��
//-----------------------------------
void ActionBase::AttackCollision(std::shared_ptr<Actor> actor,
								 const char* collision_name,
								 std::shared_ptr<AttackCollitionTime> collision_time_data,
								 CollisionMeshType collision_type)
{
	// �R���W�����}�l�[�W���[�擾
	CollisionManager& collision_manager = CollisionManager::Instance();

	std::shared_ptr<Charactor> charactor = actor->GetComponent<Charactor>();

	// �����蔻����s�����Ԃ��̔���

	// 1�t���[���O����R���W�����t���O���ω����Ă�����
	if (Universal::JudgementCollisionTime(actor, collision_time_data))
	{
		std::shared_ptr<CollisionObject> collision;
		switch (collision_type)
		{
		case CollisionMeshType::Sphere:
			// �R���W�����̂����蔻��̃I���I�t��؂�ւ���
			collision = collision_manager.GetCollisionSphereFromName(collision_name);
			break;
		case CollisionMeshType::Cylinder:
			// �R���W�����̂����蔻��̃I���I�t��؂�ւ���
			collision = collision_manager.GetCollisionCylinderFromName(collision_name);
			break;
		}

		// �����蔻����s���t���O��ω�������
		collision->SetCollisionFlag(charactor->GetOldCollisionTimeFlag());
	}
}

void ActionBase::AttackCollision(std::shared_ptr<Actor> actor,
	const char* collision_name,
	int id,
	std::shared_ptr<AttackCollitionTime> collision_time_data,
	CollisionMeshType collision_type)
{
	// �R���W�����}�l�[�W���[�擾
	CollisionManager& collision_manager = CollisionManager::Instance();

	// �����蔻����s�����Ԃ��̔���
	bool collision_time_flag = Universal::JudgementCollisionTime(actor, collision_time_data);

	// 1�t���[���O����R���W�����t���O���ω����Ă�����
	if (collision_time_flag != NULL)
	{
		std::shared_ptr<CollisionObject> collision;
		// �ΏۂɂȂ�R���W�������Ƃ�
		switch (collision_type)
		{
		case CollisionMeshType::Sphere:
			// ID�Ɩ��O���狅�R���W�������擾
			collision = collision_manager.GetCollisionSphereFromNameAndID(id, collision_name);
			break;
		case CollisionMeshType::Cylinder:
			// ID�Ɩ��O����~���R���W�������擾
			collision = collision_manager.GetCollisionCylinderFromNameAndID(id, collision_name);
			break;
		}

		// �����蔻����s���t���O��ω�������
		collision->SetCollisionFlag(collision_time_flag);
	}
}

//-----------------------------------
// �ړI�n�ɓ���������
//-----------------------------------
bool ActionBase::JedgmentToTargetPosition(const DirectX::XMFLOAT3& target_position, const DirectX::XMFLOAT3& owner_position, const char* collsion_name)
{	
	// �ړI�n�_�܂ł�XZ���ʂł̋�������
	float vx = target_position.x - owner_position.x;
	float vz = target_position.z - owner_position.z;
	float distSq = vx * vx + vz * vz;

	// �ړI�n�֒�����
	std::shared_ptr<CollisionCylinder> collision = CollisionManager::Instance().GetCollisionCylinderFromName(collsion_name);
	float radius = collision->GetRadius();
	if (distSq < radius * radius)
	{
		owner->SetRunTimer(0.0f);
		return true;
	}
	return false;
}
