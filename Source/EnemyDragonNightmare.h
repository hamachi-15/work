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

	// 当たり範囲デバッグプリミティブ描画
	void DrawDebugPrimitive() override;
private:
	// 連続突進のターゲット座標
	DirectX::XMFLOAT3 lunges_target_position_data[5] =
	{
		{-305.0f, 5.8f, 205.0f},
		{-207.0f, 8.8f, 261.0f},
		{-381.0f, 4.5f, 212.0f},
		{-113.0f, 2.5f, 108.0f},
		{-346.0f, 4.8f, 237.0f},
	};
};