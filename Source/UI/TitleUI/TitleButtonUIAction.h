#pragma once
#include "UIActionBase.h"
#include "Input.h"

class TitleButtonUIAction : public UIActionBase
{
public:
	// コンストラクタ
	TitleButtonUIAction(UIData* data) : UIActionBase(data) {}

	// 実行処理
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