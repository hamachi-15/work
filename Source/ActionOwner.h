#pragma once
#include "ActionBase.h"
//*****************************
// 
// ����ʒu�ֈړ�
// 
//*****************************
class MoveToTargetAction : public ActionBase
{
public:
	MoveToTargetAction(Enemy* enemy, DirectX::XMFLOAT3& target_position) : ActionBase(enemy), target_position(target_position) {}

	// ���s�O����
	void Start() override;

	// ���s����
	ActionBase::State Run(float elapsed_time) override;
private:
	DirectX::XMFLOAT3 target_position = {};
};
//*****************************
// 
// �p�j�s��
// 
//*****************************
class WanderAction : public ActionBase
{
public:
	// �R���X�g���N�^
	WanderAction(Enemy* enemy) :ActionBase(enemy) {}

	// ���s�O����
	void Start() override;

	// ���s����
	ActionBase::State Run(float elapsed_time) override;
};

//*****************************
// 
// �ǐՍs��
// 
//*****************************
class PursuitAction : public ActionBase
{
public:
	// �R���X�g���N�^
	PursuitAction(Enemy* enemy) :ActionBase(enemy) {}

	// ���s�O����
	void Start() override;

	// ���s����
	ActionBase::State Run(float elapsed_time) override;
};

//*****************************
// 
// �ҋ@�s��
// 
//*****************************
class IdleAction : public ActionBase
{
public:
	// �R���X�g���N�^
	IdleAction(Enemy* enemy) :ActionBase(enemy) {}

	// ���s�O����
	void Start() override;

	// ���s����
	ActionBase::State Run(float elapsed_time) override;
};

//*****************************
// 
// �_���[�W�s��
// 
//*****************************
class DamageAction : public ActionBase
{
public:
	// �R���X�g���N�^
	DamageAction(Enemy* enemy) :ActionBase(enemy) {}

	// ���s�O����
	void Start() override;

	// ���s����
	ActionBase::State Run(float elapsed_time) override;
};

//*****************************
// 
// ���S�s��
// 
//*****************************
class DeathAction : public ActionBase
{
public:
	// �R���X�g���N�^
	DeathAction(Enemy* enemy) :ActionBase(enemy) {}

	// ���s�O����
	void Start() override;

	// ���s����
	ActionBase::State Run(float elapsed_time) override;
};

//*****************************
// 
// �����s��
// 
//*****************************
class LeaveAction : public ActionBase
{
public:
	// �R���X�g���N�^
	LeaveAction(Enemy* enemy) :ActionBase(enemy) {}

	// ���s�O����
	void Start() override;

	// ���s����
	ActionBase::State Run(float elapsed_time) override;
};

//*****************************
// 
// �񕜍s��
// 
//*****************************
class RecoverAction : public ActionBase
{
public:
	// �R���X�g���N�^
	RecoverAction(Enemy* enemy) :ActionBase(enemy) {}

	// ���s�O����
	void Start() override;

	// ���s����
	ActionBase::State Run(float elapsed_time) override;
};

//*****************************
// 
// �����s��
// 
//*****************************
class SleepAction : public ActionBase
{
public:
	// �R���X�g���N�^
	SleepAction(Enemy* enemy) : ActionBase(enemy) {}

	// ���s�O����
	void Start() override;

	// ���s����
	ActionBase::State Run(float elapsed_time) override;
};
