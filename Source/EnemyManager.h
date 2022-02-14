#pragma once
#include <memory>
#include <vector>
#include <string>
#include <DirectXMath.h>
#include "Enemy.h"
#include "GameDatabase.h"
#include "SceneManager.h"
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
	
	// 敵データIDから敵を生成
	void CreateEnemies(int id, EnemyTerritoryTag territory_tag);
	
	// エンカウントデータから敵情報を取得して敵を生成する
	bool CreateEnemyEncountData();
	
	// 敵のステータスを設定
	void SetEnemyStatus(std::shared_ptr<Actor> actor, std::shared_ptr<EnemyData> enemy_data, std::shared_ptr<EnemyTerritoryPosition> teritory_datam, DirectX::XMFLOAT3 appearance_position = {0.0f, 0.0f, 0.0f});

	// ゲームデータから得たテリトリーを原点に出現位置をランダム算出
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

	// 敵の総数取得
	int GetEnemyCount() const { return static_cast<int>(enemies.size()); }

	// テリトリーの敵が撃破されているかのコンテナを取得
	std::map<EnemyTerritoryTag, bool> GetDefeatTeritory() { return defeat_territory; }

	bool GetDefeatTeritory(EnemyTerritoryTag tag) { return defeat_territory[tag]; }

	// テリトリーの敵が撃破されているかのフラグを設定
	void SetDefeatTeritory(EnemyTerritoryTag tag, bool flag) { defeat_territory[tag] = flag; }

	// バトルシーンか判定
	bool IsBattleScene() const { return (SceneManager::Instance().GetCurrentScene()->GetName() == "SceneBattle"); }
private:
	std::vector<std::shared_ptr<Enemy>>		enemies;						// 敵の配列
	std::map<EnemyTerritoryTag, bool>		defeat_territory;				// テリトリーの敵が撃破されているかのコンテナ
	int										identity;
	bool									appearance_ramdam_flag = true;	// ランダムな位置に出現させるかのフラグ
};