#pragma once
#include "ActionBase.h"
//***********************************
// 
// �W�����v�U���s��
// 
//***********************************
class HeavyBodyAttackAction : public ActionBase
{
public:
	// �R���X�g���N�^
	HeavyBodyAttackAction(Enemy* enemy) :ActionBase(enemy) {}
	void Start(std::string action_name);
	ActionBase::State Run(float elapsed_time);
};

//***********************************
// 
// �̓�����U���s��
// 
//***********************************
class BodyAttackAction : public ActionBase
{
public:
	// �R���X�g���N�^
	BodyAttackAction(Enemy* enemy) :ActionBase(enemy) {}
	void Start(std::string action_name);
	ActionBase::State Run(float elapsed_time);
};