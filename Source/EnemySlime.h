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

	// ビヘイビアのノード設定処理
	void SetBehaviorNode() override;
	
	// 更新処理
	void Update(float elapsed_time) override;

	// 敵の破棄
	void Destroy()override;

	// メッセージを受信したときの処理
	bool OnMessages(const Telegram& message)override;

private:
	std::string haed_collision_name;
	DirectX::XMFLOAT3 head_position;
};