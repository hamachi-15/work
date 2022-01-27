#pragma once
#include "UI.h"

//**********************************
// 
// ボスクラスの敵HPUIクラス
// 
//**********************************
class BossHealthUI : public UI
{
public:
	// コンストラクタ
	BossHealthUI() {}
	
	// デストラクタ
	~BossHealthUI() override {}

	// 名前取得
	std::string GetName() const  { return "BossHealthUI"; }
	
	// 開始処理
	void Start() override;
};