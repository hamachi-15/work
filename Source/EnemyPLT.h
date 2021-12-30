#pragma once
#include "Enemy.h"

//**********************************
// 
// プラットクラス
// 
//**********************************
class EnemyPLT : public Enemy
{
public:
	// コンストラクタ
	EnemyPLT();

	// デストラクタ
	~EnemyPLT() override;

	// 破棄処理
	void Destroy() override;

	// GUI描画
	void OnGUI() override;

	// メッセージを受信したときの処理
	bool OnMessages(const Telegram& message) override;

	// 開始処理
	void Start() override;
	
	// ビヘイビアのノード設定処理
	void SetBehaviorNode() override;

	// 更新処理
	void Update(float elapsed_time) override;

	// 当たり範囲デバッグプリミティブ描画
	void DrawDebugPrimitive() override;

private:
	std::string right_hand_collision_name;
};