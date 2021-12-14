/*!
	@file	EnemyData.h
	@brief	ツールにて自動生成しています。絶対に書き換えないでください。
	@autor	DataBase.xlsm
	@data	2021/12/14
**/
#pragma	once

#include	"EnemyCategory.h"


/*!
	@struct	EnemyData
	@brief	データ構造体
**/
struct	EnemyData
{
	int	id;	//!<	Id
	char*	name;	//!<	名前
	EnemyCategory	category;	//!<	敵の種類
	char*	model_path;	//!<	モデルパス
	char*	animation_node_name;	//!<	アニメーションの原点ノード名
	float	scale_x;	//!<	大きさ(X)
	float	scale_y;	//!<	大きさ(Y)
	float	scale_z;	//!<	大きさ(Z)
	float	angle_x;	//!<	角度(X)
	float	angle_y;	//!<	角度(Y)
	float	angle_z;	//!<	角度(Z)
	int	hp;	//!<	体力
	int	deffence;	//!<	防御力
	int	attack;	//!<	攻撃力
	float	speed;	//!<	移動速度
	float	speed_scale;	//!<	走る速度の倍率
};
