#pragma once
#include <memory>
#include <map>
#include <vector>

#include "Collision.h"

class DebugRenderer;
struct Plane;
class CullingCollision;

//*************************************
// 
// コリジョンマネージャー
// 
//*************************************
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

	// 球コリジョンリスト取得
	std::vector<std::shared_ptr<CollisionSphere>> GetCollisionSpheres() const { return spheres; }

	// 円柱コリジョンリスト取得
	std::vector<std::shared_ptr<CollisionCylinder>> GetCollisionCylinderes() const { return cylinderes; }
	
	// カリングコリジョン登録
	void RegisterCulling(std::shared_ptr<CullingCollision> culling);

	// カリングコリジョン解除
	void UnregisterCulling(std::shared_ptr<CullingCollision> culling);

	//　球コリジョンの登録
	void RegisterSphere(std::shared_ptr<CollisionSphere> collision);

	// 球コリジョンの解除
	void UnregisterSphere(std::shared_ptr<CollisionSphere> collision);

	//　円柱コリジョンの登録
	void RegisterCylinder(std::shared_ptr<CollisionCylinder> collision);

	// 円柱コリジョンの解除
	void UnregisterCylinder(std::shared_ptr<CollisionCylinder> collision);

	// 押し出し処理
	void PushOutCollision(CollisionObject* collisionA, std::shared_ptr<Actor> collisionA_actor, CollisionObject* collisionB, std::shared_ptr<Actor> collisionB_actor, ObjectCollisionResult& result);

	// レイとモデルの交差判定
	bool IntersectRayVsModel(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, Model* model, HitResult& result);

	// 名前からカリングコリジョン取得
	std::shared_ptr<CullingCollision> GetCollisionCullingFromName(std::string name);

	// 球コリジョンの数取得
	size_t GetCollisionSphereCount() const { return spheres.size(); }

	// インデックスから球コリジョン取得
	std::shared_ptr<CollisionSphere> GetCollisionSphere(int index) { return spheres.at(index); }
	
	// 名前から球コリジョン取得
	std::shared_ptr<CollisionSphere> GetCollisionSphereFromName(std::string name);

	// IDから球コリジョンを取得
	std::vector<std::shared_ptr<CollisionSphere>> GetCollisionSphereFromID(int id);

	// IDと名前から球コリジョンを取得
	std::shared_ptr<CollisionSphere> GetCollisionSphereFromNameAndID(int id, std::string name);

	// 円柱コリジョンの数取得
	size_t GetCollisionCylinderCount() const { return cylinderes.size(); }
	
	// インデックスから円柱コリジョン取得
	std::shared_ptr<CollisionCylinder> GetCollisionCylinder(int index) { return cylinderes.at(index); }
	
	// 名前から円柱コリジョン取得
	std::shared_ptr<CollisionCylinder> GetCollisionCylinderFromName(std::string name);

	// IDと名前から円柱コリジョンを取得
	std::shared_ptr<CollisionCylinder> GetCollisionCylinderFromNameAndID(int id, std::string name);
	
	// カメラの錐台とAABBの内外判定
	bool IntersectFrustumVsAABB(std::shared_ptr<CollisionBox> aabb, std::vector<Plane> frustum_list);

	// 球と球の交差判定
	bool IntersectSphereVsSphere(CollisionSphere* sphereA, CollisionSphere* sphereB);

	// 球と円柱の交差判定
	bool IntersectSphereVsCylinder(CollisionSphere* sphere, CollisionCylinder* cylinder, ObjectCollisionResult& result);

	// 円柱と円柱の交差判定
	/// 引数------------------------------
	/// 円柱コリジョンA
	/// 円柱コリジョンAを持っているアクター
	/// 円柱コリジョンB
	/// 円柱コリジョンBを持っているアクター
	/// 当たり判定のリザルトを格納する
	/// ----------------------------------
	bool IntersectCylinderVsCylinder(CollisionCylinder* cylinderA, std::shared_ptr<Actor> collisionA_actor,
		CollisionCylinder* cylinderB, std::shared_ptr<Actor> collisionB_actor, 
		ObjectCollisionResult& result);


private:
	static const int Frustum_Plan_Max = 6; // 四錐台の平面の数
private:
	CollisionRayCast raycast;

	std::vector<std::shared_ptr<CullingCollision>> cullings;//カリングコリジョン配列
	std::vector<std::shared_ptr<CullingCollision>> remove_cullings;// マネージャーから削除する用のカリングコリジョン配列

	std::vector<std::shared_ptr<CollisionSphere>>	spheres;// 更新などをする用の球コリジョン配列
	std::vector<std::shared_ptr<CollisionSphere>>	remove_spheres;// マネージャーから削除する用の球コリジョン配列

	std::vector<std::shared_ptr<CollisionCylinder>> cylinderes;// 更新などをする用の円柱コリジョン配列
	std::vector<std::shared_ptr<CollisionCylinder>> remove_cylinderes;// マネージャーから削除する用の円柱コリジョン配列
};
