#pragma once
#include <list>
#include "Input.h"
#include "Component.h"
#include "Charactor.h"
#include "Movement.h"

class Player : public Component
{
	enum class SubState;
public:
	// コンストラクタ
	Player(){}

	// デストラクタ
	~Player()override{}

	// 名前取得
	std::string GetName() const override { return "Player"; }

	// GUI描画
	void OnGUI() override;

	// 開始処理
	void Start() override;

	// 更新処理
	void Update(float elapsed_time) override;

	// メッセージを受信したときの処理
	bool OnMessages(const Telegram& message);

private: // プレイヤーの状態処理
	// 待機状態へ遷移
	void TransitionIdleState();

	// 移動状態へ遷移
	void TransitionMoveState();
	
	// 歩き状態へ遷移
	void TransitionMoveWalkState();

	// ランニング状態へ遷移
	void TransitionMoveRunState();

	// 攻撃状態へ遷移
	void TransitionAttackState(SubState sub_state);

	// 強攻撃状態遷移
	void TransitionHeavyAttackState();

	// ダッシュ強攻撃1段目更新処理
	void TransitionDushHeavyAttackState();

	// 強攻撃1段目更新処理
	void TransitionSlashAttackState();

	// 強攻撃2段目更新処理
	void TransitionSideSlashAttackState();

	// 強攻撃3段目更新処理
	void TransitionSmartSlashAttackState();

	// 弱攻撃状態遷移
	void TransitionLightAttackState();

	// ダッシュ弱攻撃1段目更新処理
	void TransitionDushLightAttackState();

	// 弱攻撃1段目更新処理
	void TransitionSlashLightAttackState();

	// 右キック攻撃更新処理
	void TransitionRightKickAttackState();

	// 左キック攻撃更新処理
	void TransitionLeftKickAttackState();

	// 回避状態へ遷移
	void TransitionAvoidState();

	// ダメージ状態へ遷移
	void TransitionDamageState();

	// 死亡状態へ遷移
	void TransitionDeathState();

	// 復活状態へ遷移
	void TransitionReviveState();

	// 待機状態更新処理
	void UpdateIdleState(float elapsed_time);

	// 移動状態更新処理
	void UpdateMoveState(float elapsed_time);

	// 歩き状態更新処理
	void UpdateMoveWalkState(float elapsed_time);

	// ランニング状態更新処理
	void UpdateMoveRunState(float elapsed_time);

	// 攻撃状態更新処理
	void UpdateAttackState(float elapsed_time);

	// 強攻撃状態更新処理
	void UpdateHeavyAttackState(float elapsed_time);
	
	// ダッシュ強攻撃1段目更新処理
	void UpdateDushHeavyAttackState(float elapsed_time);
	
	// 強攻撃1段目更新処理
	void UpdateSlashAttackState(float elapsed_time);

	// 強攻撃2段目更新処理
	void UpdateSideSlashAttackState(float elapsed_time);

	// 強攻撃3段目更新処理
	void UpdateSmartSlashAttackState(float elapsed_time);

	// 弱攻撃状態更新処理
	void UpdateLightAttackState(float elpased_time);

	// ダッシュ弱攻撃1段目更新処理
	void UpdateDushLightAttackState(float elapsed_time);

	// 弱攻撃1段目更新処理
	void UpdateSlashLightAttackState(float elpased_time);
		
	// 右キック攻撃更新処理
	void UpdateRightKickAttackState(float elapsed_time);
	
	// 左キック攻撃更新処理
	void UpdateLeftKickAttackState(float elapsed_time);

	// 回避状態更新処理
	void UpdateAvoidState(float elapsed_time);

	// ダメージ状態更新処理
	void UpdateDamageState(float elapsed_time);

	// 死亡状態更新処理
	void UpdateDeathState(float elapsed_time);

	// 復活状態更新処理
	void UpdateReviveState(float elapsed_time);

private:
	// スティック入力値から移動ベクトルを算出
	DirectX::XMFLOAT3 GetMoveVec() const;

	// 武器コリジョンのあたり判定フラグを切り替える処理
	void SetOnOrOffWepponCollisionFlag(bool flag);

	// 足コリジョンのあたり判定フラグを切り替える処理
	void SetOnOrOffFootCollisionFlag(bool flag, const char* node_name);

	// 移動入力処理
	bool InputMove(float elapsed_time, float move_speed);

	// 攻撃入力処理
	SubState InputAttack();

	// コマンドチェック
	bool CommandCheck(GamePadButton command);

	// コマンドリストをクリア
	void CommandListClear(float elapsed_time);
	
	// コマンドリストにコマンドを追加
	void AddCommandList(GamePadButton command);

	// 回避入力処理
	bool InputAvoid();

	// カメラステートの更新
	void UpdateCameraState(float elapsedtime);
private:
	enum class State
	{
		Idle,
		Move,
		Jump,
		Land,
		Attack,
		Avoid,
		Damage,
		Death,
		Revive,
		Encounter
	};
	enum class SubState
	{
		None,
		Idle,
		Move_Walk,
		Move_Running,
		Heavy_Attack,
		Light_Attack,
	};

	// ロックオンステート
	enum class LockonState
	{
		NotLocked,
		Locked,
	};

	enum class AttackConbo
	{
		None,
		Heavy_Attack_Conbo1,		// 強攻撃1段目攻撃
		Dush_Heavy_Attack,			// ダッシュ強攻撃1段目攻撃
		Dush_Light_Attack,			// 切り上げる
		Heavy_Attack_Conbo2,		// 左から右へ薙ぎ払い
		Light_Attack_Conbo1,		// 切り上げる。HardSlashから繋がる攻撃
		Light_Attack_Conbo2,
		Light_Attack_Conbo3,
		Conbo_End_Attack,			// 強攻撃弱攻撃のコンボ最終攻撃
	};

	// 武器のあたり判定のローカル座標を格納する用
	static const int Weppon_Collision_Max = 4;
private:
	std::shared_ptr<Movement>   movement;
	std::shared_ptr<Charactor>  charactor;
	State						state = State::Idle;
	State						child_state = (State)-1;
	SubState					sub_state = SubState::None;
	SubState					old_sub_state = sub_state;
	AttackConbo					attack_conbo_state = AttackConbo::None;
	std::list<GamePadButton>	input_history;
	bool						attack_conbo_flag = false;
	float						command_life_timer = 0.0f;
	// カメラ
	LockonState					lockon_state = LockonState::NotLocked;
	float						lockon_target_change_time = 0;
	float						lockon_target_change_time_max = 8;
	std::shared_ptr<Enemy>		lockon_enemy;
	DirectX::XMFLOAT3			lock_direction;

	float timer = 0.0f;
};