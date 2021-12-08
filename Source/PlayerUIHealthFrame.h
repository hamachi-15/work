#pragma once
#include "UI.h"

class PlayerUIHealthFrame : public UI
{
public:
	// コンストラクタ
	PlayerUIHealthFrame(const UIParameter& parameter) : UI(parameter){}

	// デストラクタ
	~PlayerUIHealthFrame();
	
	// 更新処理
	void Update(float elapsed_time);
};