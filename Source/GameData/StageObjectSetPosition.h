/*!
	@file	StageObjectSetPosition.h
	@brief	ツールにて自動生成しています。絶対に書き換えないでください。
	@autor	DataBase.xlsm
	@data	2022/03/18
**/
#pragma	once

#include	"StageObjectCategory.h"


/*!
	@struct	StageObjectSetPosition
	@brief	データ構造体
**/
struct	StageObjectSetPosition
{
	int	id;	//!<	id
	char*	name;	//!<	名前
	StageObjectCategory	object_category;	//!<	オブジェクトの種類
	float	position_x;	//!<	座標(X)
	float	position_y;	//!<	座標(Y)
	float	position_z;	//!<	座標(Z)
};
