#pragma once
#include "Collision.h"
class Actor;
//************************************
// 
// プレイヤーのコリジョン
// 
//************************************
class PlayerCollision : public Component
{
public:
	// コンストラクタ
	PlayerCollision() {}
	
	// デストラクタ
	~PlayerCollision();
	
	// 名前取得
	std::string GetName() const override { return this->name; }

	// GUI描画
	void OnGUI() override;
	
	// 開始処理
	void Start() override;

	// 更新処理
	void Update(float elapsed_time) override;

	void UpdateCollision(std::shared_ptr<CollisionObject> collision,std::shared_ptr<Actor> actor, Model* model);

	// 衝突時のリアクション処理
	void Reaction(int receiver, const Message& message);

	// 円柱コリジョン取得
	std::shared_ptr<CollisionCylinder> GetPlayerBodyCollision() { return collision_cylinder; }

private:
	std::shared_ptr<CollisionCylinder>				collision_cylinder;
	std::vector<std::shared_ptr<CollisionSphere>>	collision_spheres;
	std::string										name = "";
};