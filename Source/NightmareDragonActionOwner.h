#pragma once
#include "ActionBase.h"

//*****************************
// �����A�N�V����
//*****************************
class SleepAction : public ActionBase
{
public:
	// �R���X�g���N�^
	SleepAction(Enemy* enemy) : ActionBase(enemy){}

	// ���s�O����
	void Start(std::string action_name)override;

	// ���s����
	ActionBase::State Run(float elapsed_time)override;

};

//*****************************
// �K�[�h�A�N�V����
//*****************************
class DefendAction : public ActionBase
{
public:
	DefendAction(Enemy* enemy) : ActionBase(enemy) {}

	// ���s�O����
	void Start(std::string action_name)override;

	// ���s����
	ActionBase::State Run(float elapsed_time)override;
};

//*****************************
// �x�[�V�b�N�A�^�b�N
//*****************************
class BasicAttackAction : public ActionBase
{
public:
	BasicAttackAction(Enemy* enemy) : ActionBase(enemy) {}

	// ���s�O����
	void Start(std::string action_name)override;

	// ���s����
	ActionBase::State Run(float elapsed_time)override;
};

//*****************************
// �܍U��
//*****************************
class ClawAttackAction : public ActionBase
{
public:
	ClawAttackAction(Enemy* enemy) : ActionBase(enemy){}

	// ���s�O����
	void Start(std::string action_name)override;

	// ���s����
	ActionBase::State Run(float elapsed_time)override;
};

//*****************************
// �p�U��
//*****************************
class HornAttackAction : public ActionBase
{
public:
	HornAttackAction(Enemy* enemy) : ActionBase(enemy) {}

	// ���s�O����
	void Start(std::string action_name)override;

	// ���s����
	ActionBase::State Run(float elapsed_time)override;
};

//*****************************
// �W�����v�U��
//*****************************
class JumpAttackAction : public ActionBase
{
public:
	JumpAttackAction(Enemy* enemy) : ActionBase(enemy) {}

};