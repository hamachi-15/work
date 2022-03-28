/*!
	@file	CullingCollisionParameterData.h
	@brief	�c�[���ɂĎ����������Ă��܂��B��΂ɏ��������Ȃ��ł��������B
	@autor	DataBase.xlsm
	@data	2022/03/27
**/
#pragma	once

#include	"ActorType.h"
#include	"CollisionUpdateType.h"


/*!
	@struct	CullingCollisionParameterData
	@brief	�f�[�^�\����
**/
struct	CullingCollisionParameterData
{
	int	id;	//!<	id
	ActorType	enemy_category;	//!<	�A�N�^�[�̎��
	char*	collision_name;	//!<	���O
	char*	node_name;	//!<	�m�[�h�̖��O
	float	angle;	//!<	�A���O��
	float	radius_x;	//!<	���aX
	float	radius_y;	//!<	���aY
	float	radius_z;	//!<	���aZ
	float	local_x;	//!<	���[�J��X���W
	float	local_y;	//!<	���[�J��Y���W
	float	local_z;	//!<	���[�J��Z���W
	CollisionUpdateType	update_type;	//!<	�X�V���@�̎��
};
