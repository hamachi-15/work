#pragma once
#include "Collision.h"

class Actor;
enum class ActorType;

class StageObjectCollision : public Component
{
public:
	// コンストラクタ
	StageObjectCollision(ActorType category) : category(category){}
	
	// デストラクタ
	~StageObjectCollision() {}

	// 名前取得
	std::string GetName() const { return "StageObjectCollision"; }

	// GUI描画
	void OnGUI() override;

	// 開始処理
	void Start() override;

	// 更新処理
	void Update(float elapsed_time) override;

	// 円柱コリジョン取得
	std::shared_ptr<CollisionCylinder> GetPlayerBodyCollision() { return collision_cylinder; }
private:
	std::shared_ptr<CollisionCylinder>				collision_cylinder;
	ActorType category;
};