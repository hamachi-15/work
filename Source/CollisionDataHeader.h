#pragma once
#include <DirectXMath.h>

// コリジョンメッシュの種類
//enum class CollisionMeshType
//{
//	Sphere,
//	Cylinder,
//	AABB
//};

// コリジョン座標の更新方法
//enum class CollisionUpdateType
//{
//	Update_Actor,			// アクター座標に更新
//	Update_Node_Position,	// モデルの特定ノード座標に更新
//	Update_Local_Position,	// 特定ノードのローカル位置をワールド座標に変換して更新
//	Update_Castam_poition, // 上記のどれでもない方法で更新
//};

// コリジョンを持っているアクターのタイプ
//enum class CollisionActorType
//{
//	Enemy,
//	Player,
//	None		// その他
//};
enum class ActorType;
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
	ActorType				actor_type;
	CollisionUpdateType		update_type;
};
