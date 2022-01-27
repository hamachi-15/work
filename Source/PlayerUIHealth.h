#pragma once
#include "UI.h"

class UINodeBase;

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
};