#pragma once
#include "Enemy.h"

class EnemyLAT : public Enemy
{
public:
	// コンストラクタ
	EnemyLAT();

	// デストラクタ
	~EnemyLAT() override;

	// GUI描画
	void OnGUI() override;

	// 開始処理
	void Start() override;

	// ビヘイビアのノード設定処理
	void SetBehaviorNode() override;

	// 敵の破棄処理
	void Destroy() override;
	
	// 更新処理
	void Update(float elapsed_time) override;

	// 当たり範囲デバッグプリミティブ描画
	void DrawDebugPrimitive()override;

	// メッセージを受信したときの処理
	bool OnMessages(const Telegram& message)override;

};