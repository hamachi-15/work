#pragma once
#include <DirectXMath.h>

// アクターの座標で更新するかクラスメンバの座標で更新するか
enum class CollisionPositionMask
{
	Collision_Mask_Actor_Position,
	Collision_Mask_Member_Position,
	Collision_Mask_Local_Member_Position,
	Collision_Mask_Castam_poition,
};

// コリジョンメッシュの種類
enum class CollisionMeshType
{
	Sphere,
	Cylinder,
	AABB
};

// コリジョンの属性
enum class CollisionElement
{
	Weppon,
	Body,
};

// コリジョンを持っているアクターのタイプ
enum class CollisionActorType
{
	Enemy,
	Player,
	None		// その他
};

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
	DirectX::XMFLOAT3		float3_radius = { 0, 0, 0 };
	const char* name;
	const char* node_name;
	int						actor_id = -1;		// コリジョンを持っているアクターのID
	float					angle = 0.0f;
	float					radius = 0.0f;
	float					height = 0.0f;
	float					weight = 0.0f;
	bool					collision_flg = false;
	CollisionActorType		actor_type;
	CollisionElement		element;
	CollisionPositionMask	position_mask;
};
