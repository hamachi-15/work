/*!
	@file	EnemyData.h
	@brief	�c�[���ɂĎ����������Ă��܂��B��΂ɏ��������Ȃ��ł��������B
	@autor	DataBase.xlsm
	@data	2021/12/14
**/
#pragma	once

#include	"EnemyCategory.h"


/*!
	@struct	EnemyData
	@brief	�f�[�^�\����
**/
struct	EnemyData
{
	int	id;	//!<	Id
	char*	name;	//!<	���O
	EnemyCategory	category;	//!<	�G�̎��
	char*	model_path;	//!<	���f���p�X
	char*	animation_node_name;	//!<	�A�j���[�V�����̌��_�m�[�h��
	float	scale_x;	//!<	�傫��(X)
	float	scale_y;	//!<	�傫��(Y)
	float	scale_z;	//!<	�傫��(Z)
	float	angle_x;	//!<	�p�x(X)
	float	angle_y;	//!<	�p�x(Y)
	float	angle_z;	//!<	�p�x(Z)
	int	hp;	//!<	�̗�
	int	deffence;	//!<	�h���
	int	attack;	//!<	�U����
	float	speed;	//!<	�ړ����x
	float	speed_scale;	//!<	���鑬�x�̔{��
};
