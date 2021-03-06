#pragma once
#include "Component.h"
#include "CollisionDataHeader.h"

class Telegram;

//**************************************
//
// キャラクタークラス
//
//**************************************
class Charactor : public Component
{
public:
	// コンストラクタ
	Charactor(int id) : id(id) {}

	// デストラクタ
	~Charactor() override {}

	// 名前取得
	std::string GetName() const override { return "Charactor"; }

	// 開始処理
	void Start()override;

	// 更新処理
	void Update(float elapsed_time)override;

	// GUI描画
	void OnGUI()override;

	// 攻撃ヒットフラグ設定
	void SetHitAttackFlag(bool flag) { hit_attack_flag = flag; }

	// 攻撃ヒットフラグ取得
	bool GetHitAttackFlag() const { return hit_attack_flag; }

	// ダメージフラグ設定
	void SetDamageFlag(bool flag) { damage_flag = flag; }

	// ダメージフラグ取得
	bool GetDamageFlag() const { return damage_flag; }

	// ヒットストップフラグの設定
	void SetHitStopFlag(bool hitstop_flag) { this->hitstop_flag = hitstop_flag; }

	// ヒットストップフラグの取得
	bool GetHitStopFlag() const { return hitstop_flag; }

	// 1フレーム前の当たり判定を行う時間のフラグ取得
	void SetOldCollisionTimeFlag(bool flag) { this->old_collision_time_flag = flag; }

	//  1フレーム前の当たり判定を行う時間のフラグ設定
	bool GetOldCollisionTimeFlag() const { return old_collision_time_flag; }

	// 死亡フラグ設定
	void SetDeathFlag(bool flag) { this->death_flag = flag; }

	// 死亡フラグ取得
	bool GetDeathFlag() const { return death_flag; }

	// 体力を設定
	void SetHealth(int health) { this->health = health; }

	// 体力を取得
	int GetHealth() const { return health; }

	// 最大体力を設定
	void SetMaxHealth(int max_health) { this->max_health = max_health; }

	// 最大体力を取得
	int GetMaxHealth() const { return max_health; }

	// 攻撃力設定
	void SetAttack(int attck) { this->attack = attack; }

	// 攻撃力取得
	int GetAttack() const { return attack; }

	// 移動速度設定
	void SetMoveSpeed(float speed) { this->move_speed = speed; }

	// 移動速度取得
	float GetMoveSpeed() const { return this->move_speed; }

	// 移動速度設定
	void SetTurnSpeed(float speed) { this->turn_speed = speed; }

	// 走り時の倍率設定
	void SetRunSpeedScale(float speed_scale) { this->run_speed_scale = speed_scale; }

	// 走り時の倍率取得
	float GetRunSpeedScale() { return this->run_speed_scale; }

	// 移動速度取得
	float GetTurnSpeed() const { return this->turn_speed; }

	// ヒットストップタイマー設定
	void SetHitStopFrame(float hitstop_frame = 9.0f) { this->hitstop_frame = hitstop_frame; }

	// ヒットストップタイマー取得
	float GetHitStopFrame() const { return hitstop_frame; }

	// 無敵時間取得
	float GetInvincibleTimer() const { return invincible_timer; }
	
	// 識別番号取得
	int GetID() const { return id; }

	// ダメージを与える
	bool ApplyDamage(int damage, float invincible_time);

	// メッセージを受信したときの処理
	virtual bool OnMessages(const Telegram& message) { return false; }

	// アニメーションの範囲区間判定
	bool SearchAnimationTime(std::shared_ptr<Actor> actor, float start_time, float end_time);

	// ヒットストップの更新処理
	void UpdateHitStop(float elapsed_time);

	// 無敵時間更新
	void UpdateInvincibleTimer(float elapsed_time);

	// デバッグ用GUI描画
	virtual	void DrawDebugGUI() {}

	// デバッグプリミティブ描画
	virtual	void DrawDebugPrimitive() {}

	// 着地した時に呼ばれる
	virtual void OnLanding() {}

private:
	int		id = 0;							// アクターの識別番号
	bool	hit_attack_flag = false;	// 攻撃がヒットしたかのフラグ
	bool	old_collision_time_flag = false;
	
	int		max_health = 50;
	int		health = 50;
	int		attack = 10;
	float	invincible_timer = 0.0f;
	float	move_speed = 30.0f;
	float	run_speed_scale = 0.0f;
	float	turn_speed = DirectX::XMConvertToRadians(360);
	bool	damage_flag = false;		// ダメージを受けているかのフラグ
	bool	death_flag = false;			// 死亡状態かのフラグ
	float	hitstop_frame = 0.0f;		// ヒットストップタイマー
	bool	hitstop_flag = false;		// ヒットストップを行うかのフラグ

	int level;							// レベル
	int exp;							// 現在の経験値
	int next_exp;						// 次のレベルまでの経験値
};
