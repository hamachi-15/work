#pragma once
#include "UIActionBase.h"

//***********************************
// 
// �{�X�Q�[�W�A�N�V����
// 
//***********************************
class BossHealthUIAction : public UIActionBase
{
public:
	// �R���X�g���N�^
	BossHealthUIAction(UIData* data) :UIActionBase(data) {}

	// �A�N�V����
	void Action(float elapsed_time) override;
};