#pragma once
#include "ActionBase.h"

// �p���`�U���s��
class BlowAttackAction : public ActionBase
{
public:
	// �R���X�g���N�^
	BlowAttackAction(Enemy* enemy) : ActionBase(enemy){}

	// �p���`�U���s���O����
	void Start(std::string action_name)override;
	
	// �p���`�U���s������
	ActionBase::State Run(float elapsed_time) override;

};