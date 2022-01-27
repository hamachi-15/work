#pragma once
#include "UI.h"

class UINodeBase;

//*************************************
// 
// 雑魚敵HPクラス
// 
//*************************************
class NormalEnemyHealthUI : public UI, public Component
{
public:
	// コンストラクタ
	NormalEnemyHealthUI() {}

	// デストラクタ
	~NormalEnemyHealthUI() override {}

	// 名前取得
	std::string GetName() const override { return "NormalEnemyHealthUI"; }

	// 開始処理
	void Start() override;

private:
};