/*!
	@file	WorldMapData.h
	@brief	ツールにて自動生成しています。絶対に書き換えないでください。
	@autor	DataBase.xlsm
	@data	2022/01/04
**/
#pragma	once

#include	"EnemyTerritoryTag.h"


/*!
	@struct	WorldMapData
	@brief	データ構造体
**/
struct	WorldMapData
{
	int	id;	//!<	ID
	int	enemy_id;	//!<	敵ID
	EnemyTerritoryTag	tag;	//!<	出現する位置のタグ
};
