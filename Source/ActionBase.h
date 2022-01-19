#pragma once
#include <string>
#include <DirectXMath.h>
#include <memory>
#include "Collision.h"

class Actor;
class Enemy;
struct AttackCollitionTime;

//************************************
// 
// �s���������N���X
// 
//************************************
class ActionBase
{
public:
	ActionBase(Enemy* enemy) :owner(enemy) {}
	// ���s���
	enum class State
	{
		Run,		// ���s��
		Failed,		// ���s���s
		Complete,	// ���s����
	};
	// ���s�O����(�������z�֐�)
	virtual void Start() = 0;

	// ���s����(�������z�֐�)
	virtual ActionBase::State Run(float elapsed_time) = 0;
protected:
	// �U���̓����蔻�菈��
	// TODO�@�֐����čl
	void AttackCollision(std::shared_ptr<Actor> actor,
						 const char* node_name,
						 std::shared_ptr<AttackCollitionTime> collision_time_data,
						 CollisionMeshType collision_type = CollisionMeshType::Sphere);
	void AttackCollision(std::shared_ptr<Actor> actor,
						 const char* node_name,
						 int id,
						 std::shared_ptr<AttackCollitionTime> collision_time_data,
						 CollisionMeshType collision_type = CollisionMeshType::Sphere);
	// �ړI�n�ɓ���������
	bool JedgmentToTargetPosition(const DirectX::XMFLOAT3& target_position, const DirectX::XMFLOAT3& owner_position, const char* collsion_name);
protected:
	Enemy* owner;
	std::shared_ptr<AttackCollitionTime> collision_time_data;
};
