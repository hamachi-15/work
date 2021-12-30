#pragma once
#include <DirectXMath.h>
#include "Actor.h"
#include "ModelResource.h"
#include "MetaAI.h"
#include "Component.h"
class Model;
class Telegram;

// アクターの座標で更新するかクラスメンバの座標で更新するか
enum class CollisionPositionMask
{
	Collision_Mask_Actor_Position,
	Collision_Mask_Member_Position,
	Collision_Mask_Local_Member_Position,
};

// コリジョンメッシュの種類
enum class CollisionMeshType
{
	Sphere,
	Cylinder,
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
	DirectX::XMVECTOR vector = {0, 0, 0};
};

struct CollisionParameter
{
	DirectX::XMFLOAT3		position = { 0, 0, 0 };
	DirectX::XMFLOAT3		local_position = { 0, 0, 0 };
	DirectX::XMFLOAT3		node_position = { 0, 0, 0 };
	const char*				name;
	const char*				node_name;
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

//-----------------------------------
// レイキャスト
//-----------------------------------
class CollisionRayCast
{
public:
	CollisionRayCast() {}
	~CollisionRayCast() {}

	// レイとモデルの交差判定
	bool IntersectRayVsModel(
		const DirectX::XMFLOAT3& start,
		const DirectX::XMFLOAT3& end,
		const Model* model,
		HitResult& hit_result);

	// 通常用
	int IntersectRayVsModel(const DirectX::XMVECTOR& start,
		const DirectX::XMVECTOR& dir,
		float neart,
		const ModelResource::Mesh& mesh,
		DirectX::XMVECTOR& hitposition,
		DirectX::XMVECTOR& hitnormal);

	// x軸専用
	int IntersectRayVsModelX(const DirectX::XMVECTOR& start,
		const DirectX::XMVECTOR& dir,
		float neart,
		const ModelResource::Mesh& mesh,
		DirectX::XMVECTOR& hitposition,
		DirectX::XMVECTOR& hitnormal);

	// y軸専用
	int IntersectRayVsModelY(const DirectX::XMVECTOR& start,
		const DirectX::XMVECTOR& dir,
		float neart,
		const ModelResource::Mesh& mesh,
		DirectX::XMVECTOR& hitposition,
		DirectX::XMVECTOR& hitnormal);

	// z軸専用
	int IntersectRayVsModelZ(const DirectX::XMVECTOR& start,
		const DirectX::XMVECTOR& dir,
		float neart,
		const ModelResource::Mesh& mesh,
		DirectX::XMVECTOR& hitposition,
		DirectX::XMVECTOR& hitnormal);

};

//-----------------------------------
// コリジョンオブジェクトの基底クラス
//-----------------------------------
class CollisionObject : public Component
{
public:
	CollisionObject(){}
	~CollisionObject() override{}

	// GUI描画
	void OnGUI() override;

	// 更新処理
	void Update(float elapsed_time) override {}

	// 名前取得
	const char* GetName() const override { return name.c_str(); }
	// 名前設定
	void SetName(const char* name) { this->name = name; }

	// ノードの名前取得
	const char* GetNodeName() const { return node_name.c_str(); }
	// ノードの名前設定
	void SetNodeName(const char* node_name) { this->node_name = node_name; }

	// 位置
	DirectX::XMFLOAT3 GetPosition() const { return position; }
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

	DirectX::XMFLOAT3 GetLocalPosition() const { return local_position; }
	void SetLocalPosition(const DirectX::XMFLOAT3& local_position) { this->local_position = local_position; }

	// 重み
	void SetWeight(float weight) { this->weight = weight; }
	float GetWeight() const { return weight; }

	// 半径
	void SetRadius(float radius) { this->radius = radius; }
	float GetRadius() const { return radius; }
	
	// IDを取得
	int GetActorID() const { return actor_id; }
	// IDを設定
	void SetActorID(int id) { actor_id = id; }
	
	// コリジョンの属性取得
	CollisionElement GetCollisionElement() { return element; }
	// コリジョンの属性設定
	void SetCollisionElement(CollisionElement element) { this->element = element; }
	
	// コリジョンメッシュの種類取得
	CollisionPositionMask GetPositionMask() { return position_mask; }
	// コリジョンメッシュの種類設定
	void SetPositionMask(CollisionPositionMask mask) { this->position_mask = mask; }

	// コリジョンフラグ取得
	bool GetCollisionFlag() { return collision_flag; }
	// コリジョンフラグ設定
	void SetCollisionFlag(bool flag) { this->collision_flag = flag; }
private:
	std::string				name;			// コリジョンの名前
	std::string				node_name;		// モデルノード検索用。コリジョン座標がActorの座標ではない時に使う
	DirectX::XMFLOAT3		position;
	DirectX::XMFLOAT3		local_position; //　モデルノードのない座標にコリジョンを置くときに使う
	float					radius = 0.5f;
	float					weight = 0.0f;
	int						actor_id;		// コリジョンを持っているアクターのID
	bool					collision_flag; // あたり判定を行うかのフラグ
	CollisionActorType		actor_type;
	CollisionElement		element;
	CollisionPositionMask	position_mask;
};

//-----------------------------------
// 球状衝突オブジェクト
//-----------------------------------
class CollisionSphere : public CollisionObject
{
public:
	CollisionSphere() {}
	~CollisionSphere()override {}

	// 更新処理
	void Update(float elapsed_time) override;
	
	// 描画処理
	void Draw();

private:
	float				radius = 0.5f;
};

//-----------------------------------
// 円柱コリジョン
//-----------------------------------
class CollisionCylinder : public CollisionObject
{
public:
	CollisionCylinder() {}
	~CollisionCylinder()override {}

	// 高さ
	void SetHeight(float height) { this->height = height; }
	float GetHeight() const {return height; }

	// 更新処理
	void Update(float elapsed_time) override;

	// 描画処理
	void Draw();

private:
	float				height = 0.5f;
};

//-----------------------------------
// コリジョンマネージャー
//-----------------------------------
class CollisionManager
{
private:
	CollisionManager() {}
	~CollisionManager() {}

public:
	// インスタンス取得
	static CollisionManager& Instance()
	{
		static CollisionManager instance;
		return instance;
	}

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// メッセージを受信したときの処理
	bool OnMessage(const Telegram& message);

	// コリジョンリストを全破棄
	void Destroy();

	//　球コリジョンの登録
	void ReregisterSphere(std::shared_ptr<CollisionSphere> collision);

	// 球コリジョンの解除
	void UnregisterSphere(std::shared_ptr<CollisionSphere> collision);

	//　円柱コリジョンの登録
	void ReregisterCylinder(std::shared_ptr<CollisionCylinder> collision);

	// 円柱コリジョンの解除
	void UnregisterCylinder(std::shared_ptr<CollisionCylinder> collision);

	// 押し出し処理
	void PushOutCollision(CollisionObject* collisionA, CollisionObject* collisionB, ObjectCollisionResult& result);

	// レイとモデルの交差判定
	bool IntersectRayVsModel(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, Model* model, HitResult& result);

	// 名前から球コリジョン取得
	std::shared_ptr<CollisionSphere> GetCollisionSphereFromName(const char* name);

	// IDから球コリジョンを取得
	std::vector<std::shared_ptr<CollisionSphere>> GetCollisionSphereFromID(int id);
	// 名前から円柱コリジョン取得
	std::shared_ptr<CollisionCylinder> GetCollisionCylinderFromName(const char* name);

private:
	// 球と球の交差判定
	bool IntersectSphereVsSphere(CollisionSphere* sphereA, CollisionSphere* sphereB);
	// 球と円柱の交差判定
	bool IntersectSphereVsCylinder(CollisionSphere* sphere, CollisionCylinder* cylinder);
	// 円柱と円柱の交差判定
	bool IntersectCylinderVsCylinder(CollisionCylinder* cylinderA, CollisionCylinder* cylinderB, ObjectCollisionResult& result);

	
private:
	CollisionRayCast raycast;
	
	std::vector<std::shared_ptr<CollisionSphere>>	spheres;// 更新などをする用の球コリジョン配列
	std::vector<std::shared_ptr<CollisionSphere>>	remove_spheres;// マネージャーから削除する用の球コリジョン配列

	std::vector<std::shared_ptr<CollisionCylinder>> cylinderes;// 更新などをする用の円柱コリジョン配列
	std::vector<std::shared_ptr<CollisionCylinder>> remove_cylinderes;// マネージャーから削除する用の円柱コリジョン配列
};
