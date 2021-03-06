#pragma once
#include <memory>
#include "UI.h"
#include "UIData.h"
#include "Component.h"


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
	~BossHealthUI() override {}

	// 名前取得
	std::string GetName() const override { return "BossHealthUI"; }
	
	// 開始処理
	void Start() override;

	std::unique_ptr<UIData> health_gage_freame_data;
	std::unique_ptr<UIData> health_base_data;
	std::unique_ptr<UIData> health_gold_gage_data;
	std::unique_ptr<UIData> health_red_gage_data;
};