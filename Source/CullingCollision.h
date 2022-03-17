#pragma once
#include <string>
#include <memory>

enum class EnemyCategory;
struct CollisionParameter;
class CollisionBox;
class Actor;
class DebugRenderer;

//************************************
// 
// プレイヤーのカリングコリジョン
// 
//************************************
class CullingCollision
{
public:
	// コンストラクタ
	CullingCollision(EnemyCategory enemy_category, std::shared_ptr<Actor> actor);

	// デストラクタ
	~CullingCollision();

	// 名前取得
	std::string GetName() const  { return this->name; }

	// GUI描画
	void OnGUI();

	// 開始処理
	void Start();

	// 更新処理
	void Update();

	// 描画処理
	void Render(DebugRenderer* renderer);

	// 衝突時のリアクション処理
	void Reaction(bool flag);

	// コリジョン取得
	std::shared_ptr<CollisionBox> GetCullingCollisiton() { return collision_culling; }

private:
	std::shared_ptr<Actor> actor;
	std::shared_ptr<CollisionBox> collision_culling;
	std::string name = "";
	EnemyCategory	enemy_category; // データベースのコリジョンデータ検索に使う
};
