#pragma once
#include "Enemy.h"

//********************************
// 
// ミュータントクラス
// 
//********************************
class EnemyMutant : public Enemy
{
public:
	EnemyMutant();
	~EnemyMutant() override;
	// 名前取得
	const char* GetName() const override { return "Mutant"; }
	void OnGUI() override;
	// 開始処理
	void Start() override;
	// ビヘイビアのノード設定処理
	void SetBehaviorNode() override;
	// 敵の破棄処理
	void Destroy() override;
	// 更新処理
	void Update(float elapsed_time) override;
	void DrawDebugPrimitive() override;
	// メッセージを受信したときの処理
	bool OnMessages(const Telegram& message)override { return true; };

private:
	enum Animation
	{
		Anim_Idle1,
		Anim_Idle2,
		Anim_Idle3,
		Anim_Walking,
		Anim_Running,
		Anim_Punch,
		Anim_Swiping,
		Anim_JumpAttack,
		Anim_Dath
	};
};
