#pragma once
#include "UI.h"

class UINodeBase;

//------------------------------
// プレイヤーHPクラス
//------------------------------
class PlayerHealthGage : public UI
{
public:
	// コンストラクタ
	PlayerHealthGage(){}

	// デストラクタ
	~PlayerHealthGage();
	
	// 開始処理
	void Start() override;

private:
};