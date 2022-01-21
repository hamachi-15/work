#include "ActionBase.h"
#include "CollisionManager.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "GameDatabase.h"
#include "Charactor.h"

// 汎用関数
#include "Universal.h"
//************************************
// 
// 行動処理基底クラス
// 
//************************************
//-----------------------------------
// 攻撃の当たり判定処理
//-----------------------------------
void ActionBase::AttackCollision(std::shared_ptr<Actor> actor,
								 const char* collision_name,
								 std::shared_ptr<AttackCollitionTime> collision_time_data,
								 CollisionMeshType collision_type)
{
	// コリジョンマネージャー取得
	CollisionManager& collision_manager = CollisionManager::Instance();

	std::shared_ptr<Charactor> charactor = actor->GetComponent<Charactor>();

	// 当たり判定を行う時間かの判定

	// 1フレーム前からコリジョンフラグが変化していたら
	if (Universal::JudgementCollisionTime(actor, collision_time_data))
	{
		std::shared_ptr<CollisionObject> collision;
		switch (collision_type)
		{
		case CollisionMeshType::Sphere:
			// コリジョンのあたり判定のオンオフを切り替える
			collision = collision_manager.GetCollisionSphereFromName(collision_name);
			break;
		case CollisionMeshType::Cylinder:
			// コリジョンのあたり判定のオンオフを切り替える
			collision = collision_manager.GetCollisionCylinderFromName(collision_name);
			break;
		}

		// 当たり判定を行うフラグを変化させる
		collision->SetCollisionFlag(charactor->GetOldCollisionTimeFlag());
	}
}

void ActionBase::AttackCollision(std::shared_ptr<Actor> actor,
	const char* collision_name,
	int id,
	std::shared_ptr<AttackCollitionTime> collision_time_data,
	CollisionMeshType collision_type)
{
	// コリジョンマネージャー取得
	CollisionManager& collision_manager = CollisionManager::Instance();

	// 当たり判定を行う時間かの判定
	bool collision_time_flag = Universal::JudgementCollisionTime(actor, collision_time_data);

	// 1フレーム前からコリジョンフラグが変化していたら
	if (collision_time_flag != NULL)
	{
		std::shared_ptr<CollisionObject> collision;
		// 対象になるコリジョンごとに
		switch (collision_type)
		{
		case CollisionMeshType::Sphere:
			// IDと名前から球コリジョンを取得
			collision = collision_manager.GetCollisionSphereFromNameAndID(id, collision_name);
			break;
		case CollisionMeshType::Cylinder:
			// IDと名前から円柱コリジョンを取得
			collision = collision_manager.GetCollisionCylinderFromNameAndID(id, collision_name);
			break;
		}

		// 当たり判定を行うフラグを変化させる
		collision->SetCollisionFlag(collision_time_flag);
	}
}

//-----------------------------------
// 目的地に到着したか
//-----------------------------------
bool ActionBase::JedgmentToTargetPosition(const DirectX::XMFLOAT3& target_position, const DirectX::XMFLOAT3& owner_position, const char* collsion_name)
{	
	// 目的地点までのXZ平面での距離判定
	float vx = target_position.x - owner_position.x;
	float vz = target_position.z - owner_position.z;
	float distSq = vx * vx + vz * vz;

	// 目的地へ着いた
	std::shared_ptr<CollisionCylinder> collision = CollisionManager::Instance().GetCollisionCylinderFromName(collsion_name);
	float radius = collision->GetRadius();
	if (distSq < radius * radius)
	{
		owner->SetRunTimer(0.0f);
		return true;
	}
	return false;
}
