/*!
	@file	ButtleMapObjectPositionData.h
	@brief	ツールにて自動生成しています。絶対に書き換えないでください。
	@autor	DataBase.xlsm
	@data	2022/03/27
**/
#pragma	once

#include	"ActorType.h"


/*!
	@struct	ButtleMapObjectPositionData
	@brief	データ構造体
**/
struct	ButtleMapObjectPositionData
{
	int	id;	//!<	id
	char*	name;	//!<	名前
	ActorType	object_category;	//!<	オブジェクトの種類
	float	position_x;	//!<	座標(X)
	float	position_y;	//!<	座標(Y)
	float	position_z;	//!<	座標(Z)
};
