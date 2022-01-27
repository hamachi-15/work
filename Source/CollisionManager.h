#pragma once
#include <memory>
#include <vector>

#include "Collision.h"

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

	// AABBコリジョン登録
	void RegisterBox(std::shared_ptr<CollisionBox> collision);

	// AABBコリジョン解除
	void UnregisterBox(std::shared_ptr<CollisionBox> collision);

	//　球コリジョンの登録
	void RegisterSphere(std::shared_ptr<CollisionSphere> collision);

	// 球コリジョンの解除
	void UnregisterSphere(std::shared_ptr<CollisionSphere> collision);

	//　円柱コリジョンの登録
	void RegisterCylinder(std::shared_ptr<CollisionCylinder> collision);

	// 円柱コリジョンの解除
	void UnregisterCylinder(std::shared_ptr<CollisionCylinder> collision);

	// 押し出し処理
	void PushOutCollision(CollisionObject* collisionA, CollisionObject* collisionB, ObjectCollisionResult& result);

	// レイとモデルの交差判定
	bool IntersectRayVsModel(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, Model* model, HitResult& result);

	// 名前から立方体コリジョン取得
	std::shared_ptr<CollisionBox> GetCollisionBoxFromName(std::string name);

	// 名前から球コリジョン取得
	std::shared_ptr<CollisionSphere> GetCollisionSphereFromName(std::string name);

	// IDから球コリジョンを取得
	std::vector<std::shared_ptr<CollisionSphere>> GetCollisionSphereFromID(int id);

	// IDと名前から球コリジョンを取得
	std::shared_ptr<CollisionSphere> GetCollisionSphereFromNameAndID(int id, std::string name);

	// 名前から円柱コリジョン取得
	std::shared_ptr<CollisionCylinder> GetCollisionCylinderFromName(std::string name);

	// IDと名前から円柱コリジョンを取得
	std::shared_ptr<CollisionCylinder> GetCollisionCylinderFromNameAndID(int id, std::string name);

private:
	// 球と球の交差判定
	bool IntersectSphereVsSphere(CollisionSphere* sphereA, CollisionSphere* sphereB);
	// 球と円柱の交差判定
	bool IntersectSphereVsCylinder(CollisionSphere* sphere, CollisionCylinder* cylinder);
	// 円柱と円柱の交差判定
	bool IntersectCylinderVsCylinder(CollisionCylinder* cylinderA, CollisionCylinder* cylinderB, ObjectCollisionResult& result);


private:
	CollisionRayCast raycast;

	std::vector<std::shared_ptr<CollisionBox>> boxes;// 更新などをする用の円柱コリジョン配列
	std::vector<std::shared_ptr<CollisionBox>> remove_boxes;// マネージャーから削除する用の円柱コリジョン配列

	std::vector<std::shared_ptr<CollisionSphere>>	spheres;// 更新などをする用の球コリジョン配列
	std::vector<std::shared_ptr<CollisionSphere>>	remove_spheres;// マネージャーから削除する用の球コリジョン配列

	std::vector<std::shared_ptr<CollisionCylinder>> cylinderes;// 更新などをする用の円柱コリジョン配列
	std::vector<std::shared_ptr<CollisionCylinder>> remove_cylinderes;// マネージャーから削除する用の円柱コリジョン配列
};
