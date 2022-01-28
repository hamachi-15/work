#pragma once
#include "UI.h"
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
};