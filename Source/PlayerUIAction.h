#pragma once
#include "UIActionBase.h"

class PlayerGreenHealthAction : public UIActionBase
{
public:
	// コンストラクタ
	PlayerGreenHealthAction(UIData* ui_data) : UIActionBase(ui_data) {}

	// アクション
	void Action(float elapsed_time) override;
};

class PlayerRedHealthAction : public UIActionBase
{
public:
	// コンストラクタ
	PlayerRedHealthAction(UIData* ui_data) : UIActionBase(ui_data) {}

	// アクション
	void Action(float elapsed_time) override;
private:
	float speed = 100.0f; // ゲージが減少する速度
};