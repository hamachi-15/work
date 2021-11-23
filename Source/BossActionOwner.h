#pragma once
#include "ActionBase.h"

// �W�����v�U���s��
//class JumpAttackAction : public ActionBase
//{
//public:
//	JumpAttackAction(Enemy* enemy) : ActionBase(enemy) {}
//	void Start(std::string action_name);
//	ActionBase::State Run(float elapsed_time);
//};
//
// �p���`�U���s��
class PunchAttackAction : public ActionBase
{
public:
	PunchAttackAction(Enemy* enemy) : ActionBase(enemy) {}
	void Start(std::string action_name);
	ActionBase::State Run(float elapsed_time);
};

// ���ōU���s��
class SwipingAttackAction : public ActionBase
{
public:
	SwipingAttackAction(Enemy* enemy) : ActionBase(enemy) {}
	void Start(std::string action_name);
	ActionBase::State Run(float elapsed_time);

};