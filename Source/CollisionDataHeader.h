#pragma once
#include <DirectXMath.h>

enum class ActorCategory;
enum class CollisionUpdateType;

//-----------------------------------
// 判定結果
//-----------------------------------
struct HitResult
{
	DirectX::XMFLOAT3 position = { 0, 0, 0 }; // レイとポリゴンの交点
	DirectX::XMFLOAT3 normal = { 0,0,0 };	  // 衝突したポリゴンの法線ベクトル
	float			  distance = 0.0f;		  // レイの視点から交点までの距離
	int				  material_index = -1;	  // 衝突したポリゴンのマテリアル番号
};

struct ObjectCollisionResult
{
	DirectX::XMFLOAT3 positionA = { 0, 0, 0 };
	DirectX::XMFLOAT3 positionB = { 0, 0, 0 };
	DirectX::XMVECTOR vector = { 0, 0, 0 };
};

struct CollisionParameter
{
	DirectX::XMFLOAT3		position = { 0, 0, 0 };
	DirectX::XMFLOAT3		local_position = { 0, 0, 0 };
	DirectX::XMFLOAT3		node_position = { 0, 0, 0 };
	DirectX::XMFLOAT3		xmfloat_radius = { 0, 0, 0 };
	std::string				actor_name;
	std::string				name;
	std::string				node_name;
	int						actor_id = -1;		// コリジョンを持っているアクターのID
	float					angle = 0.0f;
	float					radius = 0.0f;
	float					height = 0.0f;
	float					weight = 1.0f;
	bool					collision_flg = false;
	ActorCategory			actor_type;
	CollisionUpdateType		update_type;
};
