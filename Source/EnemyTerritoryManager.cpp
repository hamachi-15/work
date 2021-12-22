#include "EnemyTerritoryManager.h"

//***************************************
// 
// “G‚Ì“ê’£‚èƒ}ƒl[ƒWƒƒ[
//
//***************************************
//----------------------------
// “G‚Ì“ê’£‚è‚ğ“o˜^
//----------------------------
void EnemyTerritoryManager::RegisterTerritory(std::shared_ptr<EnemyTerritory> territory)
{
	enemy_territory_list.emplace_back(territory);
}

//----------------------------
// “ê’£‚èƒ^ƒO‚©‚ç“ê’£‚è‚ğæ“¾
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
// ”jŠü
//----------------------------
void EnemyTerritoryManager::Destroy()
{
}

//-------------------------------
// “ê’£‚è‚Ì•`‰æˆ—
//-------------------------------
void EnemyTerritoryManager::Render()
{
	for (std::shared_ptr<EnemyTerritory> territory : enemy_territory_list)
	{
		territory->DebugRenderTerritory();
	}
}
