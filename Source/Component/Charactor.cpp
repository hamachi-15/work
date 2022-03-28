#include "ImGuiRenderer.h"
#include "Charactor.h"
#include "Model.h"
#include "Actor.h"
#include "CollisionManager.h"
#include "Telegram.h"
//-----------------------------------------
// ダメージを与える
//-----------------------------------------
bool Charactor::ApplyDamage(int damage, float invincible_time)
{
	// ダメージが0の場合、健康状態を変更する
	if (damage == 0) return false;

	// 死亡している場合は健康状態を変更しない
	if (health <= 0) return false;

	// 無敵時間中はダメージを与えない
	if (invincible_timer > 0.0f) return false;

	// 無敵時間設定
	invincible_timer = invincible_time;

	// ダメージ処理
	health -= damage;

	// 死亡通知
	if (health <= 0)
	{
		death_flag = true;
	}
	// ダメージ通知
	else
	{
		damage_flag = true;
	}

	return true;
}

void Charactor::Start()
{
}

void Charactor::Update(float elapsed_time)
{
	// 無敵時間更新処理
	UpdateInvincibleTimer(elapsed_time);

	// ヒットストップの更新処理
	UpdateHitStop(elapsed_time);
}

void Charactor::OnGUI()
{
	ImGui::InputInt("Health", &health);
	ImGui::InputInt("ID", &id);
	ImGui::Checkbox("AttackFlag", &hit_attack_flag);
	ImGui::Checkbox("DamageFlag", &damage_flag);

}

//-----------------------------------------
// 無敵時間更新
//-----------------------------------------
void Charactor::UpdateInvincibleTimer(float elapsed_time)
{
	// 無敵時間があれば減少させていく
	if (invincible_timer > 0.0f)
	{
		invincible_timer -= elapsed_time;
	}
}

//-----------------------------------------
// アニメーションの範囲区間判定
//-----------------------------------------
bool Charactor::SearchAnimationTime(std::shared_ptr<Actor> actor, float start_time, float end_time)
{
	Model* model = actor->GetModel();
	// アニメーション時間が一定内か調べる
	float animation_time = model->GetCurrentAnimationSeconds();
	return animation_time >= start_time && animation_time <= end_time;
}

//-----------------------------------------
// ヒットストップの更新処理
//-----------------------------------------
void Charactor::UpdateHitStop(float elapsed_time)
{
	// ヒットストップフラグが立っていなければ処理はしない
	if (!hitstop_flag) return;

	// ヒットストップフレームが0以下ならフラグをfalseにする
	if (hitstop_frame <= 0.0f)
	{
		// フラグをOFFに
		hitstop_flag = false;
		// 0.0fに初期化
		hitstop_frame = 0.0f;
		return;
	}
	// ヒットストップフレームのデクリメント
	hitstop_frame--;
}
