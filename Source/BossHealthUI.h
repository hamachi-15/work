#pragma once
#include "UI.h"
#include "Component.h"
#include "UIData.h"
//**********************************
// 
// ボスクラスの敵HPUIクラス
// 
//**********************************
class BossHealthUI : public UI, public Component
{
public:
	// コンストラクタ
	BossHealthUI() {}
	
	// デストラクタ
	~BossHealthUI() override { 
		delete health_gage_freame_data;
		delete health_base_data;
		delete health_gold_gage_data;
		delete health_red_gage_data;
	}

	// 名前取得
	std::string GetName() const override { return "BossHealthUI"; }
	
	// 開始処理
	void Start() override;

	UIData* health_gage_freame_data;
	UIData* health_base_data;
	UIData* health_gold_gage_data;
	UIData* health_red_gage_data;
};