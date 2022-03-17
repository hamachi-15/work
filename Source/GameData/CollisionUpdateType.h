/*!
	@file	CollisionUpdateType.h
	@brief	ツールにて自動生成しています。絶対に書き換えないでください。
	@autor	DataBase.xlsm
	@data	2022/03/18
**/
#pragma	once

/*!
	@enum	CollisionUpdateType
	@brief	コリジョンの更新方法
**/
enum	class	CollisionUpdateType
{
	Update_Actor,	//!<	アクター
	Update_Node,	//!<	ノード
	Update_Local,	//!<	ローカル
	Update_Castam,	//!<	カスタム
	None,	//!<	その他
};
