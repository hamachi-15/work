/*!
	@file	StageObjectData.h
	@brief	�c�[���ɂĎ����������Ă��܂��B��΂ɏ��������Ȃ��ł��������B
	@autor	DataBase.xlsm
	@data	2022/03/27
**/
#pragma	once

#include	"ActorType.h"


/*!
	@struct	StageObjectData
	@brief	�f�[�^�\����
**/
struct	StageObjectData
{
	int	id;	//!<	id
	char*	object_name;	//!<	���O
	char*	model_path;	//!<	���f���p�X
	ActorType	object_category;	//!<	�I�u�W�F�N�g�̎��
	float	scale_x;	//!<	�傫��(X)
	float	scale_y;	//!<	�傫��(Y)
	float	scale_z;	//!<	�傫��(Z)
	float	angle_x;	//!<	�p�x(X)
	float	angle_y;	//!<	�p�x(Y)
	float	angle_z;	//!<	�p�x(Z)
};
