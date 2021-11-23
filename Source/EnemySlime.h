#pragma once
#include "Enemy.h"
#include "Collision.h"
#include "AnimationData.h"

class EnemySlime : public Enemy
{
public:
	EnemySlime();
	~EnemySlime() override;
	// GUI描画
	void OnGUI() override;

	// 開始処理
	void Start() override;

	// 更新処理
	void Update(float elapsed_time) override;

	// 当たり範囲デバッグプリミティブ描画
	void DrawDebugPrimitive()override;

	// 敵の破棄
	void Destroy()override;

	// メッセージを受信したときの処理
	bool OnMessages(const Telegram& message)override;

	// アニメーション再生
	void PlayAnimation(std::shared_ptr<AnimationData> animation);
private:
	std::string haed_collision_name;
	DirectX::XMFLOAT3 head_position;
};