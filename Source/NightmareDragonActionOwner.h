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
	// �R���X�g���N�^
	DefendAction(Enemy* enemy) : ActionBase(enemy) {}

	// ���s�O����
	void Start(std::string action_name)override;

	// ���s����
	ActionBase::State Run(float elapsed_time)override;
};

//*****************************
// ���݂��U��
//*****************************
class BasicAttackAction : public ActionBase
{
public:
	// �R���X�g���N�^
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
	// �R���X�g���N�^
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
	// �R���X�g���N�^
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
	// �R���X�g���N�^
	JumpAttackAction(Enemy* enemy) : ActionBase(enemy) {}

	// ���s�O����
	void Start(std::string action_name)override;

	// ���s����
	ActionBase::State Run(float elapsed_time)override;
};