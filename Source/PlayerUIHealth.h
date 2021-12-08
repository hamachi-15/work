#pragma once
#include "UI.h"
#include "Charactor.h"

//------------------------------
// プレイヤーHPクラス
//------------------------------
class PlayerHealth : public UI
{
public:
	// コンストラクタ
	PlayerHealth(const UIParameter& parameter) : UI(parameter) {}

	// デストラクタ
	~PlayerHealth() override;

	// 更新処理
	void Update(float elapsed_time);

	// キャラクタコンポーネント設定
	void SetCharactor(std::shared_ptr<Charactor> charactor) { this->charactor = charactor; }
private:
	std::shared_ptr<Charactor> charactor;
};