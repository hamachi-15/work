/*!
	@file	CullingCollisionParameterData.h
	@brief	ツールにて自動生成しています。絶対に書き換えないでください。
	@autor	DataBase.xlsm
	@data	2022/02/09
**/
#pragma	once

#include	"EnemyCategory.h"


/*!
	@struct	CullingCollisionParameterData
	@brief	データ構造体
**/
struct	CullingCollisionParameterData
{
	int	id;	//!<	id
	EnemyCategory	enemy_category;	//!<	アクターの種類
	char*	collision_name;	//!<	名前
	char*	node_name;	//!<	ノードの名前
	float	angle;	//!<	アングル
	float	radius_x;	//!<	半径X
	float	radius_y;	//!<	半径Y
	float	radius_z;	//!<	半径Z
};
