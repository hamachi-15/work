/*!
	@file	StageObjectSetPosition.h
	@brief	�c�[���ɂĎ����������Ă��܂��B��΂ɏ��������Ȃ��ł��������B
	@autor	DataBase.xlsm
	@data	2022/03/18
**/
#pragma	once

#include	"StageObjectCategory.h"


/*!
	@struct	StageObjectSetPosition
	@brief	�f�[�^�\����
**/
struct	StageObjectSetPosition
{
	int	id;	//!<	id
	char*	name;	//!<	���O
	StageObjectCategory	object_category;	//!<	�I�u�W�F�N�g�̎��
	float	position_x;	//!<	���W(X)
	float	position_y;	//!<	���W(Y)
	float	position_z;	//!<	���W(Z)
};
