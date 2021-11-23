/*!
	@file	AttackCollitionTime.h
	@brief	�c�[���ɂĎ����������Ă��܂��B��΂ɏ��������Ȃ��ł��������B
	@autor	DataBase.xlsm
	@data	2021/11/16
**/
#pragma	once

#include	"AttackCategory.h"


/*!
	@struct	AttackCollitionTime
	@brief	�f�[�^�\����
**/
struct	AttackCollitionTime
{
	int	id;	//!<	Id
	AttackCategory	attack_category;	//!<	�J�e�S���[
	float	start_time;	//!<	�����蔻��J�n����
	float	end_time;	//!<	�����蔻��I������
};
