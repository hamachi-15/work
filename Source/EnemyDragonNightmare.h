#pragma once
#include "Enemy.h"

//--------------------------------
// ナイトメアドラゴンクラス
//--------------------------------
class EnemyDragonNightmare : public Enemy
{
public:
	// コンストラクタ
	EnemyDragonNightmare();

	// デストラクタ
	~EnemyDragonNightmare()override;

	// GUI描画
	void OnGUI() override;

	// 開始処理
	void Start() override;

	// 更新処理
	void Update(float elapsed_time)override;

	// 当たり範囲デバッグプリミティブ描画
	void DrawDebugPrimitive()override;

	// メッセージを受信したときの処理
	bool OnMessages(const Telegram& message)override;

	// アニメーション再生
	void PlayAnimation(std::shared_ptr<AnimationData> animation);
};