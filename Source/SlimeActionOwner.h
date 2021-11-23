#pragma once
#include "ActionBase.h"

// �W�����v�U���s��
class JumpAttackAction : public ActionBase
{
public:
	JumpAttackAction(Enemy* enemy) :ActionBase(enemy) {}
	void Start(std::string action_name);
	ActionBase::State Run(float elapsed_time);
};

// �̓�����U���s��
class BodyAttackAction : public ActionBase
{
public:
	BodyAttackAction(Enemy* enemy) :ActionBase(enemy) {}
	void Start(std::string action_name);
	ActionBase::State Run(float elapsed_time);
};