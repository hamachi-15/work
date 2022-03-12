#pragma once
#include "ActionBase.h"

//*****************************
// 
// �����킹�A�N�V����
// 
//*****************************
class TurnToTargetAction : public ActionBase
{
public:
	// �R���X�g���N�^
	TurnToTargetAction(Enemy* enemy) : ActionBase(enemy) {}
	
	// ���s�O����
	void Start() override;

	// ���s����
	ActionBase::State Run(float elapsed_time) override;
private:
	DirectX::XMFLOAT3 turn_direction; // ���������x�N�g��
	DirectX::XMFLOAT3 front_direction;
	float			larp_timer = 0.0f;
};

//*****************************
// 
// ���K�s��
// 
//*****************************
class ScreamAction : public ActionBase
{
public:
	// �R���X�g���N�^
	ScreamAction(Enemy* enemy) : ActionBase(enemy) {}

	// ���s�O����
	void Start() override;

	// ���s����
	ActionBase::State Run(float elapsed_time) override;
};

//*****************************
// 
// �K�[�h�A�N�V����
// 
//*****************************
class DefendAction : public ActionBase
{
public:
	// �R���X�g���N�^
	DefendAction(Enemy* enemy) : ActionBase(enemy) {}

	// ���s�O����
	void Start() override;

	// ���s����
	ActionBase::State Run(float elapsed_time) override;
};

//*****************************
// 
// ���݂��U��
// 
//*****************************
class BasicAttackAction : public ActionBase
{
public:
	// �R���X�g���N�^
	BasicAttackAction(Enemy* enemy) : ActionBase(enemy) {}

	// ���s�O����
	void Start() override;

	// ���s����
	ActionBase::State Run(float elapsed_time) override;
};

//*****************************
// 
// �܍U��
// 
//*****************************
class ClawAttackAction : public ActionBase
{
public:
	// �R���X�g���N�^
	ClawAttackAction(Enemy* enemy) : ActionBase(enemy) {}

	// ���s�O����
	void Start() override;

	// ���s����
	ActionBase::State Run(float elapsed_time) override;
};

//*****************************
// 
// �p�U��
// 
//*****************************
class HornAttackAction : public ActionBase
{
public:
	// �R���X�g���N�^
	HornAttackAction(Enemy* enemy) : ActionBase(enemy) {}

	// ���s�O����
	void Start() override;

	// ���s����
	ActionBase::State Run(float elapsed_time) override;
};

//*****************************
// 
// �{�f�B�v���X�U��
// 
//*****************************
class BodyPressAttackAction : public ActionBase
{
public:
	// �R���X�g���N�^
	BodyPressAttackAction(Enemy* enemy) : ActionBase(enemy) {}

	// ���s�O����
	void Start() override;

	// ���s����
	ActionBase::State Run(float elapsed_time) override;
};

//*****************************
// 
// �ːi�U��
// 
//*****************************
class LungesAttackAction : public ActionBase
{
public:
	// �R���X�g���N�^
	LungesAttackAction(Enemy* enemy) : ActionBase(enemy) {}

	// ���s�O����
	void Start() override;

	// ���s����
	ActionBase::State Run(float elapsed_time) override;
private:
	DirectX::XMFLOAT3 target_direction = {}; // �^�[�Q�b�g�ւ̕����x�N�g��
};

//*****************************
// 
// �΋��U��
// 
//*****************************
class FireBollAttackAction : public ActionBase
{
public:
	// �R���X�g���N�^
	FireBollAttackAction (Enemy* enemy) : ActionBase(enemy) {}

	// ���s�O����
	void Start() override;

	// ���s����
	ActionBase::State Run(float elapsed_time) override;
private:
	DirectX::XMFLOAT3	direction;
	bool				flag = false;
};

//*****************************
// 
// �K���U��
// 
//*****************************
class TailAttackAction : public ActionBase
{
public:
	// �R���X�g���N�^
	TailAttackAction(Enemy* enemy) : ActionBase(enemy) {}

	// ���s�O����
	void Start() override;

	// ���s����
	ActionBase::State Run(float elapsed_time) override;

};