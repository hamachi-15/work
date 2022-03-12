/*!
	@file	AttackCollitionTime.h
	@brief	ツールにて自動生成しています。絶対に書き換えないでください。
	@autor	DataBase.xlsm
	@data	2022/03/12
**/
#pragma	once

#include	"AttackCategory.h"
#include	"EnemyCategory.h"


/*!
	@struct	AttackCollitionTime
	@brief	データ構造体
**/
struct	AttackCollitionTime
{
	int	id;	//!<	Id
	AttackCategory	attack_category;	//!<	カテゴリー
	EnemyCategory	attacker_category;	//!<	攻撃者のカテゴリ
	float	start_time;	//!<	当たり判定開始時間
	float	end_time;	//!<	当たり判定終了時間
};
