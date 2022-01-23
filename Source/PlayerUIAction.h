#pragma once
#include "UIActionBase.h"

class PlayerGreenHealthAction : public UIActionBase
{
public:
	// �R���X�g���N�^
	PlayerGreenHealthAction(UIData* ui_data) : UIActionBase(ui_data) {}

	// �A�N�V����
	void Action(float elapsed_time) override;
};

class PlayerRedHealthAction : public UIActionBase
{
public:
	// �R���X�g���N�^
	PlayerRedHealthAction(UIData* ui_data) : UIActionBase(ui_data) {}

	// �A�N�V����
	void Action(float elapsed_time) override;
private:
	float speed = 100.0f; // �Q�[�W���������鑬�x
};