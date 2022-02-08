/*!
	@file	CullingCollisionParameterData.h
	@brief	�c�[���ɂĎ����������Ă��܂��B��΂ɏ��������Ȃ��ł��������B
	@autor	DataBase.xlsm
	@data	2022/02/09
**/
#pragma	once

#include	"EnemyCategory.h"


/*!
	@struct	CullingCollisionParameterData
	@brief	�f�[�^�\����
**/
struct	CullingCollisionParameterData
{
	int	id;	//!<	id
	EnemyCategory	enemy_category;	//!<	�A�N�^�[�̎��
	char*	collision_name;	//!<	���O
	char*	node_name;	//!<	�m�[�h�̖��O
	float	angle;	//!<	�A���O��
	float	radius_x;	//!<	���aX
	float	radius_y;	//!<	���aY
	float	radius_z;	//!<	���aZ
};
