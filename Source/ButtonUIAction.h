#pragma once
#include "UIActionBase.h"

//******************************************
// 
// �X�^�[�g�{�^���A�N�V����
// 
//******************************************
class StartButtonAction : public UIActionBase
{
public:
	// �R���X�g���N�^
	StartButtonAction(UIData* ui_data) : UIActionBase(ui_data) {}

	// �A�N�V����
	void Action(float elapsed_time) override;
};
