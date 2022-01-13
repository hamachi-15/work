#include "ActionBase.h"
#include "Collision.h"
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
								 const char* node_name,
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
		std::shared_ptr<CollisionSphere> collision;
		switch (collision_type)
		{
		case CollisionMeshType::Sphere:
			// コリジョンのあたり判定のオンオフを切り替える
			collision = collision_manager.GetCollisionSphereFromName(node_name);
			break;
		case CollisionMeshType::Cylinder:
			// コリジョンのあたり判定のオンオフを切り替える
			collision = collision_manager.GetCollisionSphereFromName(node_name);
			break;
		}

		// 当たり判定を行うフラグを変化させる
		collision->SetCollisionFlag(collision_time_flag);
	}
}

void ActionBase::AttackCollision(std::shared_ptr<Actor> actor,
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
		std::shared_ptr<CollisionSphere> collision;
		switch (collision_type)
		{
		case CollisionMeshType::Sphere:
			// コリジョンのあたり判定のオンオフを切り替える
			collision = collision_manager.GetCollisionSphereFromName(node_name);
			break;
		case CollisionMeshType::Cylinder:
			// コリジョンのあたり判定のオンオフを切り替える
			collision = collision_manager.GetCollisionSphereFromName(node_name);
			break;
		}

		// 当たり判定を行うフラグを変化させる
		collision->SetCollisionFlag(collision_time_flag);
	}
}
