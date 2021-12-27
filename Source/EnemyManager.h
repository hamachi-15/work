#pragma once
#include <memory>
#include <vector>
#include <string>
#include <DirectXMath.h>
#include "Enemy.h"
#include "GameDatabase.h"

// 前方宣言
class Actor;

class EnemyManager
{
private:
	EnemyManager();
	~EnemyManager();
public:
	// インスタンス取得
	static EnemyManager& Instance()
	{
		static EnemyManager instance;
		return instance;
	}
	// 縄張りの設定
	void CreateTerritory();

	// エネミー登録
	void EnemyRegister(std::shared_ptr<Enemy> enemy);

	// エネミー削除
	void EnemyRemove(std::shared_ptr<Enemy> enemy);

	// 敵を生成
	void CreateEnemies();
	
	void CreateEnemies(int id);
	
	// スクリプトから敵情報を取得して敵を生成する
	bool CreateEnemyScriptData();
	
	// 敵のステータスを設定
	void SetEnemyStatus(std::shared_ptr<Actor> actor, std::shared_ptr<EnemyData> enemy_data, std::shared_ptr<EnemyAppearancePosition> appearance_data);
	void SetEnemyStatus(std::shared_ptr<Actor> actor, std::shared_ptr<EnemyData> enemy_data, int& string_id, DirectX::XMFLOAT3& appearance_position);

	// ゲームデータから得た出現位置を原点に出現位置をランダム算出
	void GetAppearancePosition(std::shared_ptr<Actor> actor, DirectX::XMFLOAT3 appearance_origin, float appearance_range);

	// 全ての敵を破棄
	void AllRemove();

	// デバッグプリミティブ描画
	void DrawDebugPrimitive();

	// 敵のコンポーネントを追加する処理
	void AddComponent(std::shared_ptr<Actor> actor, std::shared_ptr<EnemyData> data, EnemyTerritoryTag& tag);

	// 敵を取得
	std::shared_ptr<Enemy> GetEnemy(int index) { return enemies.at(index); }
	// IDで敵を取得
	std::shared_ptr<Enemy> GetEnemyFromID(int id);

	// インデントリ取得
	const int& GetIndentity() const { return identity; }

	// 敵のカウント取得
	int GetEnemyCount() const { return static_cast<int>(enemies.size()); }
public:
	// 一度に出現させる敵の上限と下限
	enum EnemyCountLimit
	{
		Min_Limit = 1,
		Max_Limit = 4,
	};
private:
	std::vector<std::shared_ptr<Enemy>> enemies;
	int identity;
};