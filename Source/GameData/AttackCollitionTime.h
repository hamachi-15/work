/*!
	@file	AttackCollitionTime.h
	@brief	ツールにて自動生成しています。絶対に書き換えないでください。
	@autor	DataBase.xlsm
	@data	2021/11/16
**/
#pragma	once

#include	"AttackCategory.h"


/*!
	@struct	AttackCollitionTime
	@brief	データ構造体
**/
struct	AttackCollitionTime
{
	int	id;	//!<	Id
	AttackCategory	attack_category;	//!<	カテゴリー
	float	start_time;	//!<	当たり判定開始時間
	float	end_time;	//!<	当たり判定終了時間
};
