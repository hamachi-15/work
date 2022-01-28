#pragma once
#include "UIActionBase.h"

//***********************************
// 
// ボスゲージアクション
// 
//***********************************
class BossHealthUIAction : public UIActionBase
{
public:
	// コンストラクタ
	BossHealthUIAction(UIData* data) :UIActionBase(data) {}

	// アクション
	void Action(float elapsed_time) override;
};