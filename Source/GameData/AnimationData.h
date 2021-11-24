/*!
	@file	AnimationData.h
	@brief	ツールにて自動生成しています。絶対に書き換えないでください。
	@autor	DataBase.xlsm
	@data	2021/11/24
**/
#pragma	once



/*!
	@struct	AnimationData
	@brief	データ構造体
**/
struct	AnimationData
{
	int	id;	//!<	ID
	char*	animation_name;	//!<	アニメーション
	int	number;	//!<	番号
	int	roop_flag;	//!<	ループ再生
	float	blend;	//!<	アニメーション遷移時のブレンド率
};
