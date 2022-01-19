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
// 行動処理基底クラス
// 
//************************************
class ActionBase
{
public:
	ActionBase(Enemy* enemy) :owner(enemy) {}
	// 実行情報
	enum class State
	{
		Run,		// 実行中
		Failed,		// 実行失敗
		Complete,	// 実行成功
	};
	// 実行前処理(純粋仮想関数)
	virtual void Start() = 0;

	// 実行処理(純粋仮想関数)
	virtual ActionBase::State Run(float elapsed_time) = 0;
protected:
	// 攻撃の当たり判定処理
	// TODO　関数名再考
	void AttackCollision(std::shared_ptr<Actor> actor,
						 const char* node_name,
						 std::shared_ptr<AttackCollitionTime> collision_time_data,
						 CollisionMeshType collision_type = CollisionMeshType::Sphere);
	void AttackCollision(std::shared_ptr<Actor> actor,
						 const char* node_name,
						 int id,
						 std::shared_ptr<AttackCollitionTime> collision_time_data,
						 CollisionMeshType collision_type = CollisionMeshType::Sphere);
	// 目的地に到着したか
	bool JedgmentToTargetPosition(const DirectX::XMFLOAT3& target_position, const DirectX::XMFLOAT3& owner_position, const char* collsion_name);
protected:
	Enemy* owner;
	std::shared_ptr<AttackCollitionTime> collision_time_data;
};
