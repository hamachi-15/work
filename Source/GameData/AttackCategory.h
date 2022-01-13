/*!
	@file	AttackCategory.h
	@brief	�c�[���ɂĎ����������Ă��܂��B��΂ɏ��������Ȃ��ł��������B
	@autor	DataBase.xlsm
	@data	2022/01/13
**/
#pragma	once

/*!
	@enum	AttackCategory
	@brief	�U���J�e�S��
**/
enum	class	AttackCategory
{
	LightAttack1,	//!<	��U��1�i��
	LightAttack2,	//!<	��U��2�i��
	LightAttack3,	//!<	��U��3�i��
	LightAttack4,	//!<	��U��4�i��
	DushLightAttack,	//!<	�_�b�V����U��
	HeavyAttack1,	//!<	���U��1�i��
	HeavyAttack2,	//!<	���U��2�i��
	HeavyAttack3,	//!<	���U��3�i��
	HeavyAttack4,	//!<	���U��4�i��
	DushHeavyAttack,	//!<	�_�b�V�����U��
	Avoid,	//!<	���
	BodyPressAttack,	//!<	�{�f�B�v���X�U��
	ClawAttack,	//!<	�Ђ������U��
	HornAttack,	//!<	�p�U��
	BitingAttack,	//!<	���݂��U��
	PunchAttack,	//!<	�p���`�U��
	BodyAttack,	//!<	�̓�����U��
	JumpAttack,	//!<	�W�����v�U��
};
