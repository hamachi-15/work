/*!
	@file	StageObjectData.h
	@brief	ツールにて自動生成しています。絶対に書き換えないでください。
	@autor	DataBase.xlsm
	@data	2022/03/27
**/
#pragma	once

#include	"ActorType.h"


/*!
	@struct	StageObjectData
	@brief	データ構造体
**/
struct	StageObjectData
{
	int	id;	//!<	id
	char*	object_name;	//!<	名前
	char*	model_path;	//!<	モデルパス
	ActorType	object_category;	//!<	オブジェクトの種類
	float	scale_x;	//!<	大きさ(X)
	float	scale_y;	//!<	大きさ(Y)
	float	scale_z;	//!<	大きさ(Z)
	float	angle_x;	//!<	角度(X)
	float	angle_y;	//!<	角度(Y)
	float	angle_z;	//!<	角度(Z)
};
