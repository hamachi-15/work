/*!
	@file	CullingCollisionParameterData.h
	@brief	ツールにて自動生成しています。絶対に書き換えないでください。
	@autor	DataBase.xlsm
	@data	2022/03/27
**/
#pragma	once

#include	"ActorType.h"
#include	"CollisionUpdateType.h"


/*!
	@struct	CullingCollisionParameterData
	@brief	データ構造体
**/
struct	CullingCollisionParameterData
{
	int	id;	//!<	id
	ActorType	enemy_category;	//!<	アクターの種類
	char*	collision_name;	//!<	名前
	char*	node_name;	//!<	ノードの名前
	float	angle;	//!<	アングル
	float	radius_x;	//!<	半径X
	float	radius_y;	//!<	半径Y
	float	radius_z;	//!<	半径Z
	float	local_x;	//!<	ローカルX座標
	float	local_y;	//!<	ローカルY座標
	float	local_z;	//!<	ローカルZ座標
	CollisionUpdateType	update_type;	//!<	更新方法の種類
};
