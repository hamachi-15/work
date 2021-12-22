#include "EnemyTerritoryManager.h"

//***************************************
// 
// �G�̓꒣��}�l�[�W���[
//
//***************************************
//----------------------------
// �G�̓꒣���o�^
//----------------------------
void EnemyTerritoryManager::RegisterTerritory(std::shared_ptr<EnemyTerritory> territory)
{
	enemy_territory_list.emplace_back(territory);
}

//----------------------------
// �꒣��^�O����꒣����擾
//----------------------------
std::shared_ptr<EnemyTerritory> EnemyTerritoryManager::GetTerritory(EnemyTerritoryTag& tag)
{
	for (std::shared_ptr<EnemyTerritory> territory : enemy_territory_list)
	{
		if (territory->GetTerritoryTag() == tag)
		{
			return territory;
		}
	}

	return nullptr;
}

//----------------------------
// �j��
//----------------------------
void EnemyTerritoryManager::Destroy()
{
}

//-------------------------------
// �꒣��̕`�揈��
//-------------------------------
void EnemyTerritoryManager::Render()
{
	for (std::shared_ptr<EnemyTerritory> territory : enemy_territory_list)
	{
		territory->DebugRenderTerritory();
	}
}
