/*!
	@file	EnemyTerritoryPosition.h
	@brief	ツールにて自動生成しています。絶対に書き換えないでください。
	@autor	DataBase.xlsm
	@data	2022/01/28
**/
#pragma	once

#include	"EnemyTerritoryTag.h"


/*!
	@struct	EnemyTerritoryPosition
	@brief	データ構造体
**/
struct	EnemyTerritoryPosition
{
	int	id;	//!<	ID
	EnemyTerritoryTag	tag;	//!<	テリトリータグ
	float	position_x;	//!<	X座標
	float	position_y;	//!<	Y座標
	float	position_z;	//!<	Z座標
	float	radius;	//!<	半径
};
