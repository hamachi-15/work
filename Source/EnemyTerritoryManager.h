#pragma once
#include "EnemyTerritory.h"
#include <memory>
#include <vector>

//***************************************
// 
// �G�̓꒣��}�l�[�W���[�N���X
//
//***************************************
class EnemyTerritoryManager
{
private:
	EnemyTerritoryManager(){}
	~EnemyTerritoryManager(){}
public:
	// �C���X�^���X�擾
	static EnemyTerritoryManager& Instance()
	{
		static EnemyTerritoryManager instance;
		return instance;
	}

	// �G�̓꒣���o�^
	void RegisterTerritory(std::shared_ptr<EnemyTerritory> territory);

	// �꒣��^�O����꒣����擾
	std::shared_ptr<EnemyTerritory> GetTerritory(const EnemyTerritoryTag& tag);
	
	// �j��
	void Destroy();

	// �꒣��̕`�揈��
	void Render();
private:
	std::vector<std::shared_ptr<EnemyTerritory>> enemy_territory_list;
};