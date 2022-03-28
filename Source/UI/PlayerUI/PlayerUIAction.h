#pragma once
#include "UIActionBase.h"
//TODO クラス名を何か使ってもいいように改名
//***********************************
// 
// 緑ゲージアクション
// 
//***********************************
class PlayerGreenHealthAction : public UIActionBase
{
public:
	// コンストラクタ
	PlayerGreenHealthAction(UIData* ui_data) : UIActionBase(ui_data) {}

	// アクション
	void Action(float elapsed_time) override;
};

//***********************************
// 
// 赤ゲージアクション
// 
//***********************************
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