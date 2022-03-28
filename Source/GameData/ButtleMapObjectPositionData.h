/*!
	@file	ButtleMapObjectPositionData.h
	@brief	�c�[���ɂĎ����������Ă��܂��B��΂ɏ��������Ȃ��ł��������B
	@autor	DataBase.xlsm
	@data	2022/03/27
**/
#pragma	once

#include	"ActorType.h"


/*!
	@struct	ButtleMapObjectPositionData
	@brief	�f�[�^�\����
**/
struct	ButtleMapObjectPositionData
{
	int	id;	//!<	id
	char*	name;	//!<	���O
	ActorType	object_category;	//!<	�I�u�W�F�N�g�̎��
	float	position_x;	//!<	���W(X)
	float	position_y;	//!<	���W(Y)
	float	position_z;	//!<	���W(Z)
};
