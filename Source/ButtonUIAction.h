#pragma once
#include "UIActionBase.h"

//******************************************
// 
// スタートボタンアクション
// 
//******************************************
class StartButtonAction : public UIActionBase
{
public:
	// コンストラクタ
	StartButtonAction(UIData* ui_data) : UIActionBase(ui_data) {}

	// アクション
	void Action(float elapsed_time) override;
};
