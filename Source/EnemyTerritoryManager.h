#pragma once
#include "EnemyTerritory.h"
#include <memory>
#include <vector>

//***************************************
// 
// 敵の縄張りマネージャークラス
//
//***************************************
class EnemyTerritoryManager
{
private:
	EnemyTerritoryManager(){}
	~EnemyTerritoryManager(){}
public:
	// インスタンス取得
	static EnemyTerritoryManager& Instance()
	{
		static EnemyTerritoryManager instance;
		return instance;
	}

	// 敵の縄張りを登録
	void RegisterTerritory(std::shared_ptr<EnemyTerritory> territory);

	// 縄張りタグから縄張りを取得
	std::shared_ptr<EnemyTerritory> GetTerritory(const EnemyTerritoryTag& tag);
	
	// 破棄
	void Destroy();

	// 縄張りの描画処理
	void Render();
private:
	std::vector<std::shared_ptr<EnemyTerritory>> enemy_territory_list;
};