#pragma once
#include "ActionBase.h"

// �p�j�s��
class WanderAction : public ActionBase
{
public:
	WanderAction(Enemy* enemy) :ActionBase(enemy) {}
	void Start(std::string action_name);
	ActionBase::State Run(float elapsed_time);
};

// �ǐՍs��
class PursuitAction : public ActionBase
{
public:
	PursuitAction(Enemy* enemy) :ActionBase(enemy) {}
	void Start(std::string action_name);
	ActionBase::State Run(float elapsed_time);
};

// �ҋ@�s��
class IdleAction : public ActionBase
{
public:
	IdleAction(Enemy* enemy) :ActionBase(enemy) {}
	void Start(std::string action_name);
	ActionBase::State Run(float elapsed_time);
};
// �_���[�W�s��
class DamageAction : public ActionBase
{
public:
	DamageAction(Enemy* enemy) :ActionBase(enemy) {}
	void Start(std::string action_name);
	ActionBase::State Run(float elapsed_time);
};

// ���S�s��
class DeathAction : public ActionBase
{
public:
	DeathAction(Enemy* enemy) :ActionBase(enemy) {}
	void Start(std::string action_name);
	ActionBase::State Run(float elapsed_time);
};

// �����s��
class LeaveAction : public ActionBase
{
public:
	LeaveAction(Enemy* enemy) :ActionBase(enemy) {}
	void Start(std::string action_name);
	ActionBase::State Run(float elapsed_time);
};

// �񕜍s��
class RecoverAction : public ActionBase
{
public:
	RecoverAction(Enemy* enemy) :ActionBase(enemy) {}
	void Start(std::string action_name);
	ActionBase::State Run(float elapsed_time);
};