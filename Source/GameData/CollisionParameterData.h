/*!
	@file	CollisionParameterData.h
	@brief	ツールにて自動生成しています。絶対に書き換えないでください。
	@autor	DataBase.xlsm
	@data	2022/03/27
**/
#pragma	once

#include	"ActorType.h"
#include	"CollisionMeshType.h"
#include	"ActorCategory.h"
#include	"CollisionUpdateType.h"


/*!
	@struct	CollisionParameterData
	@brief	データ構造体
**/
struct	CollisionParameterData
{
	int	id;	//!<	ID
	ActorType	enemy_category;	//!<	アクター
	CollisionMeshType	collision_type;	//!<	コリジョンの種類
	char*	collision_name;	//!<	コリジョンの名前
	char*	node_name;	//!<	ノードの名前
	float	angle;	//!<	アングル
	float	radius;	//!<	半径
	float	height;	//!<	高さ
	float	weight;	//!<	ウエイト
	float	local_x;	//!<	ローカルX座標
	float	local_y;	//!<	ローカルY座標
	float	local_z;	//!<	ローカルZ座標
	int	collision_flag;	//!<	コリジョンフラグ
	ActorCategory	actor_type;	//!<	所持するアクターのタイプ
	CollisionUpdateType	collision_update_type;	//!<	更新方法の種類
};
