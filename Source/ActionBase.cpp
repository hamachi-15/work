#include "ActionBase.h"
#include "Collision.h"
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
								 const char* node_name,
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
		std::shared_ptr<CollisionSphere> collision;
		switch (collision_type)
		{
		case CollisionMeshType::Sphere:
			// �R���W�����̂����蔻��̃I���I�t��؂�ւ���
			collision = collision_manager.GetCollisionSphereFromName(node_name);
			break;
		case CollisionMeshType::Cylinder:
			// �R���W�����̂����蔻��̃I���I�t��؂�ւ���
			collision = collision_manager.GetCollisionSphereFromName(node_name);
			break;
		}

		// �����蔻����s���t���O��ω�������
		collision->SetCollisionFlag(collision_time_flag);
	}
}

void ActionBase::AttackCollision(std::shared_ptr<Actor> actor,
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
		std::shared_ptr<CollisionSphere> collision;
		switch (collision_type)
		{
		case CollisionMeshType::Sphere:
			// �R���W�����̂����蔻��̃I���I�t��؂�ւ���
			collision = collision_manager.GetCollisionSphereFromName(node_name);
			break;
		case CollisionMeshType::Cylinder:
			// �R���W�����̂����蔻��̃I���I�t��؂�ւ���
			collision = collision_manager.GetCollisionSphereFromName(node_name);
			break;
		}

		// �����蔻����s���t���O��ω�������
		collision->SetCollisionFlag(collision_time_flag);
	}
}
