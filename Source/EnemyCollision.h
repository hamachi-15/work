#pragma once
#include "Collision.h"

enum class EnemyCategory;
class Effect;

//************************************
// 
// プレイヤーのコリジョン
// 
//************************************
class EnemyCollision : public Component
{
public:
	// コンストラクタ
	EnemyCollision(EnemyCategory enemy_category, int identity) : enemy_category(enemy_category), identity(identity){}

	// デストラクタ
	~EnemyCollision();

	// 名前取得
	std::string GetName() const override { return this->name; }

	// GUI描画
	void OnGUI() override;

	// 開始処理
	void Start() override;

	// 更新処理
	void Update(float elapsed_time) override;

	// コリジョンの更新処理
	void UpdateCollision(std::shared_ptr<CollisionObject> collision, std::shared_ptr<Actor> actor, Model* model);

	// 衝突時のリアクション処理
	void Reaction(int receiver, const Message& message);

	// 円柱コリジョン取得
	std::shared_ptr<CollisionCylinder> GetEnemyBodyCollision() { return collision_cylinder; }
private:
	std::shared_ptr<Effect>							hit_effect = nullptr;
	float											hit_effect_scale = 5.0f;
	std::shared_ptr<CollisionCylinder>				collision_cylinder;
	std::vector<std::shared_ptr<CollisionSphere>>	collision_spheres;
	std::string										name = "";
	bool											attack_falg = false;
	EnemyCategory									enemy_category; // データベースのコリジョンデータ検索に使う
	int												identity = 0;
};