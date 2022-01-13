/*!
	@file	EnemyAppearancePosition.h
	@brief	�c�[���ɂĎ����������Ă��܂��B��΂ɏ��������Ȃ��ł��������B
	@autor	DataBase.xlsm
	@data	2022/01/13
**/
#pragma	once

#include	"EnemyTerritoryTag.h"


/*!
	@struct	EnemyAppearancePosition
	@brief	�f�[�^�\����
**/
struct	EnemyAppearancePosition
{
	int	id;	//!<	ID
	EnemyTerritoryTag	tag;	//!<	�e���g���[�^�O
	float	position_x;	//!<	X���W
	float	position_y;	//!<	Y���W
	float	position_z;	//!<	Z���W
	float	radius;	//!<	���a
};
