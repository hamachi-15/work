#pragma once
#include <DirectXMath.h>
#include "Actor.h"
#include "ModelResource.h"
#include "MetaAI.h"
#include "Component.h"
#include "CollisionDataHeader.h"
class Model;
class Telegram;


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
	void Update(float elapsed_time) override;

	// 名前取得
	std::string GetName() const override { return name.c_str(); }

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

//*************************************
// 
// ボックス衝突オブジェクト
// 
//*************************************
class CollisionBox : public CollisionObject
{
public:
	CollisionBox() {}
	~CollisionBox() override {}

	// 更新処理
	//void Update(float elapsed_time) override;

	// 描画処理
	void Draw();

	// 半径設定
	void SetRadius(const DirectX::XMFLOAT3 radius) { this->radius = radius; }

	// 半径取得
	const DirectX::XMFLOAT3& GetRadius() const { return radius; }
private:
	DirectX::XMFLOAT3 radius;
};

//*************************************
// 
// 球状衝突オブジェクト
// 
//*************************************
class CollisionSphere : public CollisionObject
{
public:
	CollisionSphere() {}
	~CollisionSphere()override {}

	// 更新処理
	//void Update(float elapsed_time) override;
	
	// 描画処理
	void Draw();

};

//*************************************
// 
// 円柱衝突オブジェクト
// 
//*************************************
class CollisionCylinder : public CollisionObject
{
public:
	CollisionCylinder() {}
	~CollisionCylinder()override {}

	// 高さ
	void SetHeight(float height) { this->height = height; }
	float GetHeight() const {return height; }

	// 更新処理
	//void Update(float elapsed_time) override;

	// 描画処理
	void Draw();

private:
	float				height = 0.5f;
};
