#pragma once
#include "Enemy.h"

//********************************
// 
// ナイトメアドラゴンクラス
// 
//********************************
class EnemyDragonNightmare : public Enemy
{
public:
	// コンストラクタ
	EnemyDragonNightmare();

	// デストラクタ
	~EnemyDragonNightmare() override;

	// GUI描画
	void OnGUI() override;

	// メッセージを受信したときの処理
	bool OnMessages(const Telegram& message) override;

	// 開始処理
	void Start() override;

	// ビヘイビアのノード設定処理
	void SetBehaviorNode() override;

	// 敵の破棄処理
	void Destroy() override;

	// 更新処理
	void Update(float elapsed_time) override;
};