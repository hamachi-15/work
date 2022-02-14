#pragma once
#include <memory>
#include "UI.h"
#include "UIData.h"
#include "Component.h"


//*************************************
// 
// プレイヤーHPクラス
// 
//*************************************
class PlayerHealthUI : public UI, public Component
{
public:
	// コンストラクタ
	PlayerHealthUI(){}

	// デストラクタ
	~PlayerHealthUI() override {}
	
	// 名前取得
	std::string GetName() const override { return "PlayerHealthUI"; }
	
	// 開始処理
	void Start() override;

private:
	std::unique_ptr<UIData> health_red_gauge_data;
	std::unique_ptr<UIData> health_green_gauge_data;
	std::unique_ptr<UIData> health_gauge_base_data;
	std::unique_ptr<UIData> health_gauge_freame_data;
};