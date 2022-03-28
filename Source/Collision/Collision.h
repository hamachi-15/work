#pragma once
#include <DirectXMath.h>
#include "Actor.h"
#include "ModelResource.h"
#include "MetaAI.h"
#include "Component.h"
#include "CollisionDataHeader.h"

enum class CollisionUpdateType;
enum class ActorCategory;
class Model;
class Telegram;
class DebugRenderer;

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
class CollisionObject
{
public:
	// コンストラクタ
	CollisionObject(const CollisionParameter& parameter) : collision_name(parameter.name), actor_name(parameter.actor_name), position(parameter.position), node_name(parameter.node_name),
		local_position(parameter.local_position), weight(parameter.weight), height(parameter.height), 
		radius(parameter.radius), xmfloat_radius(parameter.xmfloat_radius), actor_id(parameter.actor_id), actor_type(parameter.actor_type), update_type(parameter.update_type){}
	
	// デストラクタ
	virtual ~CollisionObject() {}

	// 名前取得
	const char* GetName() const { return collision_name.c_str(); }

	// ノードの名前取得
	const char* GetNodeName() const { return node_name.c_str(); }

	// コリジョンを持つアクターの名前取得
	const char* GetActorName() const { return actor_name.c_str(); }

	// 位置
	DirectX::XMFLOAT3 GetPosition() const { return position; }
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

	DirectX::XMFLOAT3 GetLocalPosition() const { return local_position; }
	void SetLocalPosition(const DirectX::XMFLOAT3& local_position) { this->local_position = local_position; }

	// 色取得
	DirectX::XMFLOAT4 GetColor() const { return color; }

	// 色設定
	void SetColor() { this->color = color; }

	// 重み取得
	float GetWeight() const { return weight; }
	
	// 重み設定
	void SetWeight(float weight) { this->weight = weight; }

	// 半径取得
	float GetRadius() const { return radius; }
	const DirectX::XMFLOAT3& GetXMFloatRadius() const { return xmfloat_radius; }
	
	// 半径設定
	void SetRadius(float radius) { this->radius = radius; }
	void SetXMFloatRadius(const DirectX::XMFLOAT3 radius) { this->xmfloat_radius = radius; }

	// 高さ取得
	float GetHeight() const { return height; }

	// 高さ設定
	void SetHeight() { this->height = height; }

	// IDを取得
	int GetActorID() const { return actor_id; }
	
	// アクターの種類取得
	const ActorCategory& GetActorType() { return actor_type; }

	// 更新方法取得
	const CollisionUpdateType& GetUpdateType() const { return update_type; }

	// コリジョンフラグ取得
	bool GetCollisionFlag() { return collision_flag; }

	// コリジョンフラグ設定
	void SetCollisionFlag(bool flag) { this->collision_flag = flag; }

	// 衝突フラグ取得
	bool GetHitFlag() const { return hit_flag; }
	
	//衝突フラグ設定
	bool SetHitFlag(bool hit_flag) { this->hit_flag = hit_flag; }

	// 衝突フラグ取得
	bool GetAttackFlag() const { return attack_falg; }

	//衝突フラグ設定
	void SetAttackFlag(bool attack_falg) { this->attack_falg = attack_falg; }

private:
	std::string				collision_name = "";
	std::string				node_name = "";		// モデルノード検索用。コリジョン座標がActorの座標ではない時に使う
	std::string				actor_name = "";				// コリジョンを持つアクターの名前
	DirectX::XMFLOAT3		position = {};
	DirectX::XMFLOAT3		local_position = {}; //　モデルノードのない座標にコリジョンを置くときに使う
	DirectX::XMFLOAT3		xmfloat_radius = {0.0f, 0.0f, 0.0f};
	DirectX::XMFLOAT4		color = { 1.0f, 1.0f,0.0f, 1.0f };
	float					radius = 0.5f;
	float					height = 0.0f;
	float					weight = 0.0f;
	int						actor_id = 0;		// コリジョンを持っているアクターのID
	bool					collision_flag = true; // あたり判定を行うかのフラグ
	bool					attack_falg = false; // 攻撃の当たり判定を行うか
	bool					hit_flag = false;
	ActorCategory				actor_type;
	CollisionUpdateType		update_type;
};

//*************************************
// 
// ボックス衝突オブジェクト
// 
//*************************************
class CollisionBox : public CollisionObject
{
public:
	CollisionBox(CollisionParameter parameter) : CollisionObject(parameter) {}
	~CollisionBox() override {}

	// 描画処理
	virtual void Render(DebugRenderer* renderer);
};

//*************************************
// 
// 球状衝突オブジェクト
// 
//*************************************
class CollisionSphere : public CollisionObject
{
public:
	CollisionSphere(CollisionParameter parameter) : CollisionObject(parameter) {}
	~CollisionSphere()override {}
	
	// 描画処理
	virtual void Render(DebugRenderer* renderer);
};

//*************************************
// 
// 円柱衝突オブジェクト
// 
//*************************************
class CollisionCylinder : public CollisionObject
{
public:
	CollisionCylinder(CollisionParameter parameter) : CollisionObject(parameter) {}
	~CollisionCylinder()override {}

	// 描画処理
	virtual void Render(DebugRenderer* renderer);
};
