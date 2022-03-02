/*!
	@file	CollisionParameterData.h
	@brief	�c�[���ɂĎ����������Ă��܂��B��΂ɏ��������Ȃ��ł��������B
	@autor	DataBase.xlsm
	@data	2022/03/03
**/
#pragma	once

#include	"EnemyCategory.h"
#include	"CollisionMeshType.h"
#include	"ActorType.h"
#include	"CollisionUpdateType.h"


/*!
	@struct	CollisionParameterData
	@brief	�f�[�^�\����
**/
struct	CollisionParameterData
{
	int	id;	//!<	ID
	EnemyCategory	enemy_category;	//!<	�A�N�^�[
	CollisionMeshType	collision_type;	//!<	�R���W�����̎��
	char*	collision_name;	//!<	�R���W�����̖��O
	char*	node_name;	//!<	�m�[�h�̖��O
	float	angle;	//!<	�A���O��
	float	radius;	//!<	���a
	float	height;	//!<	����
	float	weight;	//!<	�E�G�C�g
	float	local_x;	//!<	���[�J��X���W
	float	local_y;	//!<	���[�J��Y���W
	float	local_z;	//!<	���[�J��Z���W
	int	collision_flag;	//!<	�R���W�����t���O
	ActorType	actor_type;	//!<	��������A�N�^�[�̃^�C�v
	CollisionUpdateType	collision_update_type;	//!<	�X�V���@�̎��
};
