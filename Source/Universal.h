#pragma once
#include <DirectXMath.h>
#include <string>
#include "Model.h"


// 前方宣言
struct AttackCollitionTime;
class Actor;
class Enemy;
class CollisionObject;

//**********************************
// 
// 汎用関数クラス
// 
//**********************************
class Universal
{
public:
	// ターゲットが前方にいるか判定
	static bool JudgementTargetInFront(const DirectX::XMFLOAT3& origine_position, const DirectX::XMFLOAT3& origine_angle, const DirectX::XMFLOAT3& target_position);

	// 当たり判定を行う時間かの判定
	static bool JudgementCollisionTime(std::shared_ptr<Actor> actor, std::shared_ptr<AttackCollitionTime> collision_time_data);

	// コリジョン座標を指定のノード座標へ更新
	static void NodePositionUpdate(CollisionObject* collision_object, std::string node_name, Model* model);
	
	// コリジョン座標を指定のローカル座標へ更新
	static void LocalPositionUpdate(CollisionObject* collision_object, Model::Node* node);
	
	// コリジョン座標を指定のアクター座標へ更新
	static void ActorPositionUpdate(CollisionObject* collision_object, std::shared_ptr<Actor> actor);

	// コリジョン座標を指定のアクター、指定のローカル座標ではない座標へ更新
	static void CastamPositionUpdate(CollisionObject* collision_object, std::shared_ptr<Actor> actor, std::string node_name, Model* model);
};