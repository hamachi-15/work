#pragma once
#include "UI.h"

class Charactor;

//--------------------------------------
// プレイヤーHPの赤ゲージクラス
//--------------------------------------
class PlayerUIRedHealth : public UI
{
public:
	// コンストラクタ
	PlayerUIRedHealth(const UIParameter& parameter) : UI(parameter){}
	// デストラクタ
	~PlayerUIRedHealth() override {}

	// 更新処理
	void Update(float elapsed_time) override;
	
	// キャラクタコンポーネント設定
	void SetCharactor(std::shared_ptr<Charactor> charactor) { this->charactor = charactor; }
private:
	float speed = 100.0f; // ゲージが減少する速度
	std::shared_ptr<Charactor> charactor;
};