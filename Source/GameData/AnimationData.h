/*!
	@file	AnimationData.h
	@brief	�c�[���ɂĎ����������Ă��܂��B��΂ɏ��������Ȃ��ł��������B
	@autor	DataBase.xlsm
	@data	2021/11/24
**/
#pragma	once



/*!
	@struct	AnimationData
	@brief	�f�[�^�\����
**/
struct	AnimationData
{
	int	id;	//!<	ID
	char*	animation_name;	//!<	�A�j���[�V����
	int	number;	//!<	�ԍ�
	int	roop_flag;	//!<	���[�v�Đ�
	float	blend;	//!<	�A�j���[�V�����J�ڎ��̃u�����h��
};
