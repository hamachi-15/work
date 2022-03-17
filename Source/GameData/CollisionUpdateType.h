/*!
	@file	CollisionUpdateType.h
	@brief	�c�[���ɂĎ����������Ă��܂��B��΂ɏ��������Ȃ��ł��������B
	@autor	DataBase.xlsm
	@data	2022/03/18
**/
#pragma	once

/*!
	@enum	CollisionUpdateType
	@brief	�R���W�����̍X�V���@
**/
enum	class	CollisionUpdateType
{
	Update_Actor,	//!<	�A�N�^�[
	Update_Node,	//!<	�m�[�h
	Update_Local,	//!<	���[�J��
	Update_Castam,	//!<	�J�X�^��
	None,	//!<	���̑�
};
