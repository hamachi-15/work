/*!
	@file	WorldMapData.h
	@brief	�c�[���ɂĎ����������Ă��܂��B��΂ɏ��������Ȃ��ł��������B
	@autor	DataBase.xlsm
	@data	2021/12/21
**/
#pragma	once

#include	"EnemyTerritoryTag.h"


/*!
	@struct	WorldMapData
	@brief	�f�[�^�\����
**/
struct	WorldMapData
{
	int	id;	//!<	ID
	int	enemy_id;	//!<	�GID
	EnemyTerritoryTag	tag;	//!<	�o������ʒu�̃^�O
};