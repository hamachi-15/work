#pragma once
#include "UIActionBase.h"
#include "Input.h"

class TitleButtonUIAction : public UIActionBase
{
public:
	// �R���X�g���N�^
	TitleButtonUIAction(UIData* data) : UIActionBase(data) {}

	// ���s����
	void Action(float elapsed_time) override;
private:
	const GamePadButton any_button =
		GamePad::BTN_A
		| GamePad::BTN_B
		| GamePad::BTN_X
		| GamePad::BTN_Y;
private:
	UIData* data;
};