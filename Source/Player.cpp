#include "Player.h"
#include "Graphics.h"
#include "Camera.h"
#include "Model.h"

#include "Mathf.h"

#include "CollisionManager.h"

#include "PlayerCollision.h"
#include "CullingCollision.h"

#include "DebugRenderer.h"
#include "ActorManager.h"
#include "EnemyManager.h"
#include "UIManager.h"
#include "AudioManager.h"

#include "GameDatabase.h"
#include "AttackCategory.h"
#include "AttackCollitionTime.h"

#include "MessageData.h"
#include "Messenger.h"

#include "PlayerUIHealth.h"

//-----------------------------------------
// ImGui描画
//-----------------------------------------
void Player::OnGUI()
{
	if (ImGui::CollapsingHeader("Stete", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::Button("Death"))
		{
			TransitionDeathState();
		}
		if (ImGui::Button("Revive"))
		{
			TransitionReviveState();
		}
		int c = (int)GetActor().use_count();
		ImGui::InputInt("actor_counter", &c);
		if (ImGui::CollapsingHeader("NowStete", ImGuiTreeNodeFlags_DefaultOpen))
		{
			switch (state)
			{
			case State::Idle:	ImGui::Text("Idle");	break;
			case State::Move:	ImGui::Text("Move");	break;
			case State::Land:	ImGui::Text("Land");	break;
			case State::Attack:	ImGui::Text("Attack");	break;
			case State::Avoid:	ImGui::Text("Avoid");	break;
			case State::Damage:	ImGui::Text("Damage");	break;
			case State::Death:	ImGui::Text("Death");	break;
			case State::Revive:	ImGui::Text("Revive");	break;
			}
			switch (sub_state)
			{
			case SubState::Light_Attack:		ImGui::Text("LightAttack");		break;
			case SubState::Heavy_Attack:		ImGui::Text("HeavyAttack");		break;
			case SubState::Move_Walk:			ImGui::Text("MoveWalk");		break;
			case SubState::Move_Running:		ImGui::Text("MoveRunning");		break;
			default:							ImGui::Text("NULL");			break;
			}
			switch (attack_conbo_state)
			{
			case AttackConbo::Dush_Light_Attack:	ImGui::Text("Dush_Light_Attack");	break;
			case AttackConbo::Light_Attack_Conbo1:	ImGui::Text("Light_Attack_Conbo1");	break;
			case AttackConbo::Light_Attack_Conbo2:	ImGui::Text("Light_Attack_Conbo2");	break;
			case AttackConbo::Light_Attack_Conbo3:	ImGui::Text("Light_Attack_Conbo3");	break;
			case AttackConbo::Heavy_Attack_Conbo1:	ImGui::Text("Heavy_Attack_Conbo1");	break;
			case AttackConbo::Heavy_Attack_Conbo2:	ImGui::Text("Heavy_Attack_Conbo2");	break;
			case AttackConbo::Dush_Heavy_Attack:	ImGui::Text("Dush_Heavy_Attack");	break;
			case AttackConbo::Conbo_End_Attack:		ImGui::Text("Conbo_End_Attack");	break;
			default:								ImGui::Text("NULL");				break;
			}
		}
	}

}

//-----------------------------------------
// 開始処理
//-----------------------------------------
void Player::Start()
{
	// アクターの取得
	std::shared_ptr<Actor> actor = GetActor();

	// キャラクターの取得
	charactor = actor->GetComponent<Charactor>();

	movement = actor->GetComponent<Movement>();

	// 走った時のスピード倍率設定
	charactor->SetRunSpeedScale(1.5f);
	
	// 待機状態へ遷移
	TransitionIdleState();
}

//-----------------------------------------
// 更新処理
//-----------------------------------------
void Player::Update(float elapsed_time)
{
	// カメラステートの更新処理
	UpdateCameraState(elapsed_time);

	// 状態ごとの更新処理
	switch (state)
	{
	case State::Idle:	UpdateIdleState(elapsed_time);	 break;
	case State::Move:	UpdateMoveState(elapsed_time);	 break;
	case State::Attack:	UpdateAttackState(elapsed_time); break;
	//case State::Avoid:	UpdateAvoidState(elapsed_time);	 break;
	case State::Damage:	UpdateDamageState(elapsed_time); break;
	case State::Death:	UpdateDeathState(elapsed_time);	 break;
	case State::Revive:	UpdateReviveState(elapsed_time); break;
	}

	// コマンド履歴クリア処理
	CommandListClear(elapsed_time);

	// ダメージフラグが立っていれば
	if (charactor->GetDamageFlag())
	{
		// ダメージ状態に遷移
		TransitionDamageState();
	}

	// 死亡フラグが立っていれば
	if (charactor->GetDeathFlag())
	{
		// 死亡状態に遷移
		TransitionDeathState();
	}
}


//-----------------------------------------------
// メッセージを受信したときの処理
//-----------------------------------------------
bool Player::OnMessages(const Telegram& message)
{
	switch (message.message_box.message)
	{// プレイヤーの攻撃が敵に当たった
	case MessageType::Message_Hit_Attack:

		// 攻撃ヒットフラグを立てる
		charactor->SetHitAttackFlag(true);
		// ヒットストップを行うフレーム数
		charactor->SetHitStopFrame();
		charactor->SetHitStopFlag(true);

		// ヒットSE再生
		AudioManager::Instance().PlaySoundEffect(SEType::SwordHit);
		return true;
		break;
	// 敵の攻撃がプレイヤーに当たった
	case MessageType::Message_GetHit_Attack:
		// ヒットSE再生
		AudioManager::Instance().PlaySoundEffect(SEType::PlayerGetHit);

		return true;
		break;
	case MessageType::Message_Hit_Boddy:

		break;
	}
	return false;
}

//-----------------------------------------
// 待機状態へ遷移
//-----------------------------------------
void Player::TransitionIdleState()
{
	state = State::Idle;
	sub_state = SubState::None;
	// 待機アニメーション再生
	Model* model = GetActor()->GetModel();
	if (model != nullptr)
	{
		// アニメーションデータ取得
		std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("PlayerIdle");
		// アニメーション再生
		model->PlayAnimation(animation->number, animation->roop_flag, animation->blend);
	}
}

//-----------------------------------------
// 移動状態へ遷移
//-----------------------------------------
void Player::TransitionMoveState()
{
	state = State::Move;
	// RTボタンが押されているときはランニング状態に遷移。それ以外の時は歩き状態に遷移
	if (Input::Instance().GetGamePad().GetButton() == GamePad::BTN_RIGHT_TRIGGER)
	{
		TransitionMoveRunState();
	}
	else
	{
		TransitionMoveWalkState();
	}
}

//-----------------------------------------
// 歩き状態へ遷移
//-----------------------------------------
void Player::TransitionMoveWalkState()
{
	sub_state = SubState::Move_Walk;
	// 移動アニメーション再生
	Model* model = GetActor()->GetModel();
	if (model != nullptr)
	{
		// アニメーションデータ取得
		std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("PlayerWalking");
		// アニメーション再生
		model->PlayAnimation(animation->number, animation->roop_flag, animation->blend);
	}
}

//-----------------------------------------
// ランニング状態へ遷移
//-----------------------------------------
void Player::TransitionMoveRunState()
{
	sub_state = SubState::Move_Running;
	// 移動アニメーション再生
	Model* model = GetActor()->GetModel();
	if (model != nullptr)
	{
		// アニメーションデータ取得
		std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("PlayerRunning");
		// アニメーション再生
		model->PlayAnimation(animation->number, animation->roop_flag, animation->blend);
	}
}

//-----------------------------------------
// 攻撃状態へ遷移
//-----------------------------------------
void Player::TransitionAttackState(SubState sub_state)
{
	// ステートを攻撃状態へ
	state = State::Attack;

	// サブステートが弱攻撃の時
	if (sub_state == SubState::Light_Attack)
	{
		if (this->sub_state == SubState::Move_Walk || this->sub_state == SubState::None)
		{
			TransitionSlashLightAttackState();
		}
		else if (this->sub_state == SubState::Move_Running)
		{
			TransitionDushLightAttackState();
		}
	}
	// サブステートが強攻撃だった時
	if (sub_state == SubState::Heavy_Attack)
	{
		if (this->sub_state == SubState::Move_Walk || this->sub_state == SubState::None)
		{
			TransitionSlashAttackState();
		}
		else if (this->sub_state == SubState::Move_Running)
		{
			TransitionDushHeavyAttackState();
		}
	}
	// サブステートを設定
	this->sub_state = sub_state;
}


//-----------------------------------------
// 弱攻撃状態へ遷移
//-----------------------------------------
void Player::TransitionLightAttackState()
{
	sub_state = SubState::Light_Attack;
}

//-----------------------------------------
// ダッシュ弱攻撃1段目更新処理
//-----------------------------------------
void Player::TransitionDushLightAttackState()
{
	attack_conbo_state = AttackConbo::Dush_Light_Attack;
	// 移動アニメーション再生
	Model* model = GetActor()->GetModel();
	if (model != nullptr)
	{
		// アニメーションデータ取得
		std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("PlayerDushLightAttack");
		// アニメーション再生
		model->PlayAnimation(animation->number, animation->roop_flag, animation->blend);
	}
}

//-----------------------------------------
// 弱攻撃1段目更新処理
//-----------------------------------------
void Player::TransitionSlashLightAttackState()
{
	attack_conbo_state = AttackConbo::Light_Attack_Conbo1;
	// 移動アニメーション再生
	Model* model = GetActor()->GetModel();
	if (model != nullptr)
	{
		// アニメーションデータ取得
		std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("PlayerSmartSlash");
		// アニメーション再生
		model->PlayAnimation(animation->number, animation->roop_flag, animation->blend);
	}
}

//-----------------------------------------
// 右キック攻撃更新処理
//-----------------------------------------
void Player::TransitionRightKickAttackState()
{
	attack_conbo_state = AttackConbo::Light_Attack_Conbo2;
	// 移動アニメーション再生
	Model* model = GetActor()->GetModel();
	if (model != nullptr)
	{
		// アニメーションデータ取得
		std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("PlayerKickRight");
		// アニメーション再生
		model->PlayAnimation(animation->number, animation->roop_flag, animation->blend);
	}
}

//-----------------------------------------
// 左キック攻撃更新処理
//-----------------------------------------
void Player::TransitionLeftKickAttackState()
{
	attack_conbo_state = AttackConbo::Light_Attack_Conbo3;
	// 移動アニメーション再生
	Model* model = GetActor()->GetModel();
	if (model != nullptr)
	{
		// アニメーションデータ取得
		std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("PlayerKickLeft");
		// アニメーション再生
		model->PlayAnimation(animation->number, animation->roop_flag, animation->blend);
	}
}

//-----------------------------------------
// 強攻撃状態へ遷移
//-----------------------------------------
void Player::TransitionHeavyAttackState()
{
	// サブステートを強攻撃へ
	sub_state = SubState::Heavy_Attack;
}

//-----------------------------------------
// ダッシュ強攻撃1段目更新処理
//-----------------------------------------
void Player::TransitionDushHeavyAttackState()
{
	attack_conbo_state = AttackConbo::Dush_Heavy_Attack;
	// ダッシュ攻撃アニメーション再生
	Model* model = GetActor()->GetModel();
	if (model != nullptr)
	{
		// アニメーションデータ取得
		std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("PlayerDushHeavyAttack");
		// アニメーション再生
		model->PlayAnimation(animation->number, animation->roop_flag, animation->blend);
	}
}

//-----------------------------------------
// 強攻撃1段目更新処理
//-----------------------------------------
void Player::TransitionSlashAttackState()
{
	attack_conbo_state = AttackConbo::Heavy_Attack_Conbo1;
	// 移動アニメーション再生
	Model* model = GetActor()->GetModel();
	if (model != nullptr)
	{
		// アニメーションデータ取得
		std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("PlayerSlash");
		// アニメーション再生
		model->PlayAnimation(animation->number, animation->roop_flag, animation->blend);
	}
}

//-----------------------------------------
// 強攻撃2段目更新処理
//-----------------------------------------
void Player::TransitionSideSlashAttackState()
{
	attack_conbo_state = AttackConbo::Heavy_Attack_Conbo2;
	// 移動アニメーション再生
	Model* model = GetActor()->GetModel();
	if (model != nullptr)
	{
		// アニメーションデータ取得
		std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("PlayerSideSlash");
		// アニメーション再生
		model->PlayAnimation(animation->number, animation->roop_flag, animation->blend);
	}
}

//-----------------------------------------
// 強攻撃3段目更新処理
//-----------------------------------------
void Player::TransitionSmartSlashAttackState()
{
	attack_conbo_state = AttackConbo::Conbo_End_Attack;
	// 移動アニメーション再生
	Model* model = GetActor()->GetModel();
	if (model != nullptr)
	{
		// アニメーションデータ取得
		std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("PlayerFullSwingAttack");
		// アニメーション再生
		model->PlayAnimation(animation->number, animation->roop_flag, animation->blend);
	}
}

//-----------------------------------------
// 回避状態へ遷移
//-----------------------------------------
void Player::TransitionAvoidState()
{
	std::shared_ptr<Actor> actor = GetActor();
	state = State::Avoid;
	// 現在の角度から前方向ベクトルを割り出し移動	
	DirectX::XMFLOAT3 angle = actor->GetAngle();
	float frontX = sinf(angle.y);
	float frontZ = cosf(angle.y);
	movement->Move({ frontX, 0.0f, frontZ }, 80);

	// ダメージアニメーション再生
	Model* model = GetActor()->GetModel();
	if (model != nullptr)
	{
		// アニメーションデータ取得
		std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("PlayerRollBackward");
		// アニメーション再生
		model->PlayAnimation(animation->number, animation->roop_flag, animation->blend);
	}
}

//-----------------------------------------
// ダメージ状態へ遷移
//-----------------------------------------
void Player::TransitionDamageState()
{
	state = State::Damage;
	// ダメージアニメーション再生
	Model* model = GetActor()->GetModel();
	if (model != nullptr)
	{
		// アニメーションデータ取得
		std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("PlayerGetHit1");
		// アニメーション再生
		model->PlayAnimation(animation->number, animation->roop_flag, animation->blend);
	}
	charactor->SetDamageFlag(false);
}

//-----------------------------------------
// 死亡状態へ遷移
//-----------------------------------------
void Player::TransitionDeathState()
{
	state = State::Death;
	std::shared_ptr<Actor> actor = GetActor();
	Model* model = actor->GetModel();

	// 死亡アニメーション再生
	if (model != nullptr)
	{
		// アニメーションデータ取得
		std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("PlayerDeath");
		// アニメーション再生
		model->PlayAnimation(animation->number, animation->roop_flag, animation->blend);
	}

	// 死亡の瞬間を描画
	Message message;
	message.message = MessageType::Message_Moment_Render;
	MetaAI::Instance().SendMessaging(
		static_cast<int>(MetaAI::Identity::Player),   // 送信元
		static_cast<int>(MetaAI::Identity::WorldMap),    // 受信先
		message);                                        // メッセージ

	// 死亡フラグを元に戻しておく(またここに処理が来ないように)
	charactor->SetDeathFlag(false);
}

//-----------------------------------------
// 復活状態へ遷移
//-----------------------------------------
void Player::TransitionReviveState()
{
	std::shared_ptr<Actor> actor = GetActor();
	Model* model = actor->GetModel();

	state = State::Revive;

	// 体力回復
	charactor->SetHealth(charactor->GetMaxHealth());

	// 復活アニメーション再生
	if (model != nullptr)
	{
		// アニメーションデータ取得
		std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("PlayerRevive");
		// アニメーション再生
		model->PlayAnimation(animation->number, animation->roop_flag, animation->blend);
	}
}

//-----------------------------------------
// 待機状態更新処理
//-----------------------------------------
void Player::UpdateIdleState(float elapsed_time)
{
	std::shared_ptr<Actor> actor = GetActor();
	Model* model = actor->GetModel();
	// 移動入力処理
	if (InputMove(elapsed_time, charactor->GetMoveSpeed()))
	{
		// 移動状態へ遷移
		TransitionMoveState();
	}

	// 攻撃入力処理
	SubState return_state = InputAttack();
	if (return_state != SubState::None)
	{
		TransitionAttackState(return_state);
	}

	//if (InputAvoid())
	//{
	//	// 回避状態へ遷移
	//	TransitionAvoidState();
	//}
}

//-----------------------------------------
// 移動状態更新処理
//-----------------------------------------
void Player::UpdateMoveState(float elapsed_time)
{
	switch (sub_state)
	{
	case SubState::Move_Walk:
		UpdateMoveWalkState(elapsed_time);
		break;
	case SubState::Move_Running:
		UpdateMoveRunState(elapsed_time);
		break;
	}
	//if (InputAvoid())
	//{
	//	// 回避状態へ遷移
	//	TransitionAvoidState();
	//}
}

//-----------------------------------------
// 歩き状態更新処理
//-----------------------------------------
void Player::UpdateMoveWalkState(float elapsed_time)
{
	// 移動入力処理
	if (!InputMove(elapsed_time, charactor->GetMoveSpeed()))
	{
		// 待機状態遷移
		TransitionIdleState();
	}
	else if (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_RIGHT_TRIGGER)
	{
		// 待機状態遷移
		TransitionMoveRunState();
	}

	// 攻撃入力処理
	SubState return_state = InputAttack();
	if (return_state != SubState::None)
	{
		TransitionAttackState(return_state);
	}
}

//-----------------------------------------
// ランニング状態更新処理
//-----------------------------------------
void Player::UpdateMoveRunState(float elapsed_time)
{
	std::shared_ptr<Actor> actor = GetActor();
	Model* model = actor->GetModel();
	// 移動入力処理
	// TODO マジックナンバー
	if (!InputMove(elapsed_time, charactor->GetMoveSpeed() * 1.5f))
	{
		// 待機状態遷移
		TransitionIdleState();
	}else if (Input::Instance().GetGamePad().GetButtonUp() & GamePad::BTN_RIGHT_TRIGGER)
	{
		// 移動状態遷移
		TransitionMoveWalkState();
	}

	// 攻撃入力処理
	SubState return_state = InputAttack();
	if (return_state != SubState::None)
	{
		TransitionAttackState(return_state);
	}
}

//-----------------------------------------
// 攻撃状態更新処理
//-----------------------------------------
void Player::UpdateAttackState(float elapsed_time)
{
	std::shared_ptr<Actor> actor = GetActor();
	Model* model = actor->GetModel();

	// 各攻撃状態更新関数へ
	switch (sub_state)
	{
	case SubState::Light_Attack:	UpdateLightAttackState(elapsed_time);	break;
	case SubState::Heavy_Attack:	UpdateHeavyAttackState(elapsed_time);	break;
	}
	
	// 攻撃コマンド入力処理
	InputAttack();

	// 攻撃アニメーション終了後
	if (!model->IsPlayAnimation())
	{
		charactor->SetHitAttackFlag(false);
		// コンボをクリアする
		attack_conbo_state = AttackConbo::None;
		// 待機状態へ遷移
		TransitionIdleState();
	}
}

//-----------------------------------------
// 強攻撃状態更新処理
//-----------------------------------------
void Player::UpdateHeavyAttackState(float elapsed_time)
{
	switch (attack_conbo_state)
	{
	case AttackConbo::Dush_Heavy_Attack:	UpdateDushHeavyAttackState(elapsed_time);	break;
	case AttackConbo::Heavy_Attack_Conbo1:	UpdateSlashAttackState(elapsed_time);		break;
	case AttackConbo::Heavy_Attack_Conbo2:	UpdateSideSlashAttackState(elapsed_time);	break;
	case AttackConbo::Light_Attack_Conbo3:	UpdateLeftKickAttackState(elapsed_time);	break;
	case AttackConbo::Conbo_End_Attack:		UpdateSmartSlashAttackState(elapsed_time);	break;
	}
}

//-----------------------------------------
// ダッシュ強攻撃1段目更新処理
//-----------------------------------------
void Player::UpdateDushHeavyAttackState(float elapsed_time)
{
	// TODO アニメーションの開始時間と終了時間をデータベースから取得
	std::shared_ptr<AttackCollitionTime> data = GameDataBase::Instance().GetAttackCollitionTimeData(AttackCategory::DushHeavyAttack);
	std::shared_ptr<Actor> actor = GetActor();
	// 現在のコリジョンフラグと1フレーム前のコリジョンフラグを取得
	bool old_collision_time_flag = charactor->GetOldCollisionTimeFlag();
	
	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	bool collision_time_flag = charactor->SearchAnimationTime(actor, 0.2f, 0.6f);

	// コリジョンフラグが変化したら
	if (old_collision_time_flag != collision_time_flag)
	{
		// コリジョンのあたり判定のオンオフを切り替える
		SetOnOrOffWepponCollisionFlag(collision_time_flag);
	}
	// 前フレームのコリジョンフラグを代入。コリジョンフラグが変化したタイミングを調べる用
	charactor->SetOldCollisionTimeFlag(collision_time_flag);

	// 一定フレーム内に続けて弱攻撃をすると次のコンボへ移行
	if (CommandCheck(GamePad::BTN_Y) && charactor->SearchAnimationTime(actor, 0.6f, 0.8f))
	{
		charactor->SetHitAttackFlag(false);
		// 右キック攻撃状態更新処理
		TransitionSideSlashAttackState();
	}
}

//-----------------------------------------
// 強攻撃1段目更新処理
//-----------------------------------------
void Player::UpdateSlashAttackState(float elapsed_time)
{
	std::shared_ptr<Actor> actor = GetActor();
	// 現在のコリジョンフラグと1フレーム前のコリジョンフラグを取得
	bool old_collision_time_flag = charactor->GetOldCollisionTimeFlag();
	
	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	bool collision_time_flag = charactor->SearchAnimationTime(actor, 0.2f, 0.6f);

	// コリジョンフラグが変化したら
	if (old_collision_time_flag != collision_time_flag)
	{
		// コリジョンのあたり判定のオンオフを切り替える
		SetOnOrOffWepponCollisionFlag(collision_time_flag);
	}
	// 前フレームのコリジョンフラグを代入。コリジョンフラグが変化したタイミングを調べる用
	charactor->SetOldCollisionTimeFlag(collision_time_flag);

	if (CommandCheck(GamePad::BTN_Y) && charactor->SearchAnimationTime(actor, 0.6f, 0.8f))
	{
		charactor->SetHitAttackFlag(false);
		// 攻撃へ遷移
		TransitionSideSlashAttackState();
	}
}

//-----------------------------------------
// 強攻撃2段目更新処理
//-----------------------------------------
void Player::UpdateSideSlashAttackState(float elapsed_time)
{
	std::shared_ptr<Actor> actor = GetActor();
	// 現在のコリジョンフラグと1フレーム前のコリジョンフラグを取得
	bool old_collision_time_flag = charactor->GetOldCollisionTimeFlag();
	
	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	bool collision_time_flag = charactor->SearchAnimationTime(actor, 0.2f, 0.6f);

	// コリジョンフラグが変化したら
	if (old_collision_time_flag != collision_time_flag)
	{
		// コリジョンのあたり判定のオンオフを切り替える
		SetOnOrOffWepponCollisionFlag(collision_time_flag);
	}
	// 前フレームのコリジョンフラグを代入。コリジョンフラグが変化したタイミングを調べる用
	charactor->SetOldCollisionTimeFlag(collision_time_flag);

	// 任意のアニメーション再生区間で強攻撃が入力されたら次の攻撃状態へ遷移
	if (CommandCheck(GamePad::BTN_Y) && charactor->SearchAnimationTime(actor, 0.6f, 0.8f))
	{
		charactor->SetHitAttackFlag(false);
		// 左キック攻撃へ遷移
		TransitionLeftKickAttackState();
	}
}

//-----------------------------------------
// 強攻撃3段目更新処理
//-----------------------------------------
void Player::UpdateSmartSlashAttackState(float elapsed_time)
{
	std::shared_ptr<Actor> actor = GetActor();
	// 現在のコリジョンフラグと1フレーム前のコリジョンフラグを取得
	bool old_collision_time_flag = charactor->GetOldCollisionTimeFlag();
	
	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	bool collision_time_flag = charactor->SearchAnimationTime(actor, 0.2f, 0.6f);

	// コリジョンフラグが変化したら
	if (old_collision_time_flag != collision_time_flag)
	{
		// コリジョンのあたり判定のオンオフを切り替える
		SetOnOrOffWepponCollisionFlag(collision_time_flag);
	}
	// 前フレームのコリジョンフラグを代入。コリジョンフラグが変化したタイミングを調べる用
	charactor->SetOldCollisionTimeFlag(collision_time_flag);
}

//-----------------------------------------
// 弱攻撃状態更新処理
//-----------------------------------------
void Player::UpdateLightAttackState(float elapsed_time)
{
	switch (attack_conbo_state)
	{
	case AttackConbo::Dush_Light_Attack:	UpdateDushLightAttackState(elapsed_time);	break;
	case AttackConbo::Light_Attack_Conbo1:	UpdateSlashLightAttackState(elapsed_time);	break;
	case AttackConbo::Light_Attack_Conbo2:	UpdateRightKickAttackState(elapsed_time);	break;
	case AttackConbo::Light_Attack_Conbo3:	UpdateLeftKickAttackState(elapsed_time);	break;
	case AttackConbo::Conbo_End_Attack:		UpdateSmartSlashAttackState(elapsed_time);	break;
	}
}

//-----------------------------------------
// ダッシュ弱攻撃状態処理
//-----------------------------------------
void Player::UpdateDushLightAttackState(float elapsed_time)
{
	std::shared_ptr<Actor> actor = GetActor();
	// 現在のコリジョンフラグと1フレーム前のコリジョンフラグを取得
	bool old_collision_time_flag = charactor->GetOldCollisionTimeFlag();
	
	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	bool collision_time_flag = charactor->SearchAnimationTime(actor, 0.2f, 0.6f);
	
	// コリジョンフラグが変化したら
	if (old_collision_time_flag != collision_time_flag)
	{
		// コリジョンのあたり判定のオンオフを切り替える
		SetOnOrOffWepponCollisionFlag(collision_time_flag);
	}
	// 前フレームのコリジョンフラグを代入。コリジョンフラグが変化したタイミングを調べる用
	charactor->SetOldCollisionTimeFlag(collision_time_flag);

	// 任意のアニメーション再生区間に入った時に弱攻撃ボタンが入力されていれば次の攻撃へ遷移
	if (CommandCheck(GamePad::BTN_B) && charactor->SearchAnimationTime(actor, 0.6f, 0.8f))
	{
		charactor->SetHitAttackFlag(false);
		// 左キック攻撃へ遷移
		TransitionLeftKickAttackState();
	}
}

//-----------------------------------------
// 弱攻撃一段目状態更新処理
//-----------------------------------------
void Player::UpdateSlashLightAttackState(float elpased_time)
{
	std::shared_ptr<Actor> actor = GetActor();
	// 現在のコリジョンフラグと1フレーム前のコリジョンフラグを取得
	bool old_collision_time_flag = charactor->GetOldCollisionTimeFlag();
	
	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	bool collision_time_flag = charactor->SearchAnimationTime(actor, 0.2f, 0.6f);

	// コリジョンフラグが変化したら
	if(old_collision_time_flag != collision_time_flag)
	{
		// コリジョンのあたり判定のオンオフを切り替える
		SetOnOrOffWepponCollisionFlag(collision_time_flag);
	}
	// 前フレームのコリジョンフラグを代入。コリジョンフラグが変化したタイミングを調べる用
	charactor->SetOldCollisionTimeFlag(collision_time_flag);

	// 任意のアニメーション再生区間に弱攻撃ボタンが押されたら
	if (CommandCheck(GamePad::BTN_B) && charactor->SearchAnimationTime(actor, 0.6f, 0.8f))
	{
		charactor->SetHitAttackFlag(false);
		// 左キック攻撃へ遷移
		TransitionLeftKickAttackState();
	}

}

//-----------------------------------------
// 左キック攻撃更新処理
//-----------------------------------------
void Player::UpdateLeftKickAttackState(float elapsed_time)
{
	std::shared_ptr<Actor> actor = GetActor();
	// 現在のコリジョンフラグと1フレーム前のコリジョンフラグを取得
	bool old_collision_time_flag = charactor->GetOldCollisionTimeFlag();
	
	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	bool collision_time_flag = charactor->SearchAnimationTime(actor, 0.2f, 0.6f);
	
	// コリジョンフラグが変化したら
	if (old_collision_time_flag != collision_time_flag)
	{
		// コリジョンのあたり判定のオンオフを切り替える
		SetOnOrOffFootCollisionFlag(collision_time_flag, "PlayerLeftFoot");
	}
	// 前フレームのコリジョンフラグを代入。コリジョンフラグが変化したタイミングを調べる用
	charactor->SetOldCollisionTimeFlag(collision_time_flag);

	// 任意のアニメーション再生区間に弱攻撃ボタンが押されたら
	if (CommandCheck(GamePad::BTN_B) && charactor->SearchAnimationTime(actor, 0.6f, 0.8f))
	{
		// 右キック攻撃へ遷移
		TransitionRightKickAttackState();
	}
	// 任意のアニメーション再生区間に強攻撃ボタンが押されたら
	else if (CommandCheck(GamePad::BTN_Y) && charactor->SearchAnimationTime(actor, 0.6f, 0.8f))
	{
		charactor->SetHitAttackFlag(false);
		// 攻撃へ遷移
		TransitionSmartSlashAttackState();
	}
}


//-----------------------------------------
// 右キック攻撃更新処理
//-----------------------------------------
void Player::UpdateRightKickAttackState(float elapsed_time)
{
	std::shared_ptr<Actor> actor = GetActor();
	// 現在のコリジョンフラグと1フレーム前のコリジョンフラグを取得
	bool old_collision_time_flag = charactor->GetOldCollisionTimeFlag();
	
	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	bool collision_time_flag = charactor->SearchAnimationTime(actor, 0.2f, 0.6f);
	
	// コリジョンフラグが変化したら
	if (old_collision_time_flag != collision_time_flag)
	{
		// コリジョンのあたり判定のオンオフを切り替える
		SetOnOrOffFootCollisionFlag(collision_time_flag, "PlayerRightFoot");
	}
	// 前フレームのコリジョンフラグを代入。コリジョンフラグが変化したタイミングを調べる用
	charactor->SetOldCollisionTimeFlag(collision_time_flag);

	// 任意のアニメーション再生区間に弱攻撃ボタンが押されたらInputLightAttack() && charactor->SearchAnimationTime(actor, 0.6f, 0.8f)
	if (CommandCheck(GamePad::BTN_B) && charactor->SearchAnimationTime(actor, 0.6f, 0.8f))
	{
		charactor->SetHitAttackFlag(false);
		TransitionSmartSlashAttackState();
	}
}


//-----------------------------------------
// 回避状態更新処理
//-----------------------------------------
void Player::UpdateAvoidState(float elapsed_time)
{
	CollisionManager& collision_manager = CollisionManager::Instance();
	std::shared_ptr<Actor> actor = GetActor();
	Model* model = actor->GetModel();
	// 現在のコリジョンフラグと1フレーム前のコリジョンフラグを取得
	bool old_collision_time_flag = charactor->GetOldCollisionTimeFlag();
	
	bool collision_time_flag;
	
	// 任意のアニメーション再生区間では衝突判定処理をしない
	if (charactor->SearchAnimationTime(actor, 0.2f, 0.7f))
	{
		collision_time_flag = false;
	}
	else
	{
		collision_time_flag = true;
	}
	// コリジョンフラグが変化したら
	if (old_collision_time_flag != collision_time_flag)
	{
		// コリジョンのあたり判定のオンオフを切り替える
		actor->GetComponent<PlayerCollision>()->GetPlayerBodyCollision()->SetCollisionFlag(collision_time_flag);
	}
	// 前フレームのコリジョンフラグを代入。コリジョンフラグが変化したタイミングを調べる用
	charactor->SetOldCollisionTimeFlag(collision_time_flag);

	// 任意のアニメーション再生区間でのみ移動入力処理を受け入れる
	if (charactor->SearchAnimationTime(actor, 0.6f, 0.8f))
	{
		if (InputMove(elapsed_time, charactor->GetMoveSpeed()))
		{
			actor->GetComponent<PlayerCollision>()->GetPlayerBodyCollision()->SetCollisionFlag(collision_time_flag);
			TransitionMoveState();
		}
	}

	// アニメーションが終了したら
	if (!model->IsPlayAnimation())
	{
		// 待機状態へ遷移
		TransitionIdleState();
	}
}

//-----------------------------------------
// ダメージ状態更新処理
//-----------------------------------------
void Player::UpdateDamageState(float elapsed_time)
{
	std::shared_ptr<Actor> actor = GetActor();
	Model* model = actor->GetModel();
	// ダメージアニメーションが終わったら待機状態へ遷移
	if (!model->IsPlayAnimation())
	{
		TransitionIdleState();
	}
}

//-----------------------------------------
// 死亡状態更新処理
//-----------------------------------------
void Player::UpdateDeathState(float elapsed_time)
{
	std::shared_ptr<Actor> actor = GetActor();
	Model* model = actor->GetModel();
	timer += elapsed_time * 60.0f;
	if (!model->IsPlayAnimation())
	{
		if (timer >= 90.0f)
		{
			Message message;
			message.message = MessageType::Message_GameOver;
			MetaAI::Instance().SendMessaging(
				static_cast<int>(MetaAI::Identity::Player),   // 送信元
				static_cast<int>(MetaAI::Identity::WorldMap),    // 受信先
				message);                                        // メッセージ
		}
	}
}

//-----------------------------------------
// 復活状態更新処理
//-----------------------------------------
void Player::UpdateReviveState(float elapsed_time)
{
	std::shared_ptr<Actor> actor = GetActor();
	Model* model = actor->GetModel();
	// 復活アニメーション終了後に待機状態へ遷移
	if (!model->IsPlayAnimation())
	{
		TransitionIdleState();
	}
}

//-----------------------------------------
// スティック入力値から移動ベクトルを算出
//-----------------------------------------
DirectX::XMFLOAT3 Player::GetMoveVec() const
{
	// 入力情報を取得
	GamePad& gamepad = Input::Instance().GetGamePad();
	float ax = gamepad.GetAxisLX();
	float ay = gamepad.GetAxisLY();

	// カメラ方向とスティック入力値によって進行方向を計算する
	Camera& camera = Camera::Instance();
	const DirectX::XMFLOAT3& camera_right = camera.GetRight();
	const DirectX::XMFLOAT3 & camera_front = camera.GetFront();

	// カメラ右方向ベクトルをXZ単位ベクトルに変換
	float camera_rightX = camera_right.x;
	float camera_rightZ = camera_right.z;
	float camera_right_length = sqrtf(camera_rightX * camera_rightX + camera_rightZ * camera_rightZ);
	if (camera_right_length > 0.0f)
	{
		// 単位ベクトル化
		camera_rightX /= camera_right_length;
		camera_rightZ /= camera_right_length;
	}

	// カメラ前方向ベクトルをXZ単位ベクトルに変換
	float camera_frontX = camera_front.x;
	float camera_frontZ = camera_front.z;
	float camera_front_length = sqrtf(camera_frontX * camera_frontX + camera_frontZ * camera_frontZ);
	if (camera_right_length > 0.0f)
	{
		// 単位ベクトル化
		camera_frontX /= camera_front_length;
		camera_frontZ /= camera_front_length;
	}

	// スティックの水平入力値をカメラ右方向に反映し、
	// スティックの垂直入力値をカメラ前方向に反映し、
	// 進行ベクトルを計算
	DirectX::XMFLOAT3 vec;
	vec.x = (camera_rightX * ax) + (camera_frontX * ay);
	vec.z = (camera_rightZ * ax) + (camera_frontZ * ay);
	// Y軸方向には移動しない
	vec.y = 0.0f;

	return vec;
}

//----------------------------------------------
// 武器コリジョンのあたり判定フラグを切り替える処理
//----------------------------------------------
void Player::SetOnOrOffWepponCollisionFlag(bool flag)
{
	CollisionManager& collision_manager = CollisionManager::Instance();
	std::shared_ptr<Actor> actor = GetActor();
	Model* model = actor->GetModel();
	std::string index;
	for (int i = 0; i < Weppon_Collision_Max; ++i)
	{
		index = std::to_string(i + 1);
		index = "PlayerWeppon" + index;
		CollisionManager::Instance().GetCollisionSphereFromName(index.c_str())->SetAttackFlag(flag);
	}
}

//---------------------------------------------
// 足コリジョンのあたり判定フラグを切り替える処理
//---------------------------------------------
void Player::SetOnOrOffFootCollisionFlag(bool flag, const char* node_name)
{
	CollisionManager& collision_manager = CollisionManager::Instance();
	std::shared_ptr<Actor> actor = GetActor();
	Model* model = actor->GetModel();

	collision_manager.GetCollisionSphereFromName(node_name)->SetAttackFlag(flag);
}

//-----------------------------------------
// 移動入力処理
//-----------------------------------------
bool Player::InputMove(float elapsed_time, float move_speed)
{
	// 進行ベクトルを取得
	DirectX::XMFLOAT3 move_vec = GetMoveVec();

	// 移動処理
	movement->Move(move_vec, move_speed);

	// 旋回処理
	movement->Turn(move_vec);

	return move_vec.x != 0.0f || move_vec.y != 0.0f || move_vec.z;
}

//-----------------------------------------
// 攻撃入力処理
//-----------------------------------------
Player::SubState Player::InputAttack()
{
	GamePad& gamepad = Input::Instance().GetGamePad();
	// 弱攻撃
	if (gamepad.GetButtonDown() & GamePad::BTN_B)
	{
		// 入力履歴に追加
		AddCommandList(GamePad::BTN_B);
		return SubState::Light_Attack;
	}
	// 強攻撃
	if (gamepad.GetButtonDown() & GamePad::BTN_Y)
	{
		// 入力履歴に追加
		AddCommandList(GamePad::BTN_Y);
		return SubState::Heavy_Attack;
	}
	return SubState::None;
}

//-----------------------------------------
// コマンドチェック
//-----------------------------------------
bool Player::CommandCheck(GamePadButton command)
{
	// 履歴が無ければ判定しない
	if (input_history.size() == 0) return false;

	// 最後に入力されたコマンドと同一ならtrue
	if (input_history.front() == command) return true;

	return false;
}

//-----------------------------------------
// コマンドリストをクリア
//-----------------------------------------
void Player::CommandListClear(float elapsed_time)
{
	// コマンドの有効時間が終了したら
	if (command_life_timer <= 0.0f)
	{
		// コマンドリストをクリア
		std::list<GamePadButton>::iterator iterat = input_history.begin();
		for (; iterat != input_history.end(); iterat = input_history.begin())
		{
			input_history.erase(iterat);
		}
	}

	// コマンド時間の有効時間を減らす
	command_life_timer -= elapsed_time;
}

//-----------------------------------------
// コマンドリストにコマンドを追加
//-----------------------------------------
void Player::AddCommandList(GamePadButton command)
{
	// コマンドの有効時間が0.0秒以下なら有効時間を設定
	if (command_life_timer <= 0.0f) command_life_timer = 0.5f;

	// コマンド履歴を追加
	input_history.push_front(command);
}

//-----------------------------------------
// 回避入力処理
//-----------------------------------------
bool Player::InputAvoid()
{
	GamePad& gamepad = Input::Instance().GetGamePad();
	if (gamepad.GetButtonDown() & GamePad::BTN_A)
	{
		return true;
	}
	return false;
}

//-----------------------------------------
// カメラステートの更新
//-----------------------------------------
void Player::UpdateCameraState(float elapsed_time)
{

	std::shared_ptr<Actor> actor = GetActor();
	DirectX::XMFLOAT3 position = actor->GetPosition();
	DirectX::XMFLOAT3 angle = actor->GetAngle();
	LockonState old_lockon_state = lockon_state;
	std::shared_ptr<Enemy> old_lockon_enemy = lockon_enemy;
	std::shared_ptr<Actor> old_lockon_enemy_actor;
	if (old_lockon_enemy != nullptr)
	{
		old_lockon_enemy_actor = old_lockon_enemy->GetActor();
	}
	lockon_state = LockonState::NotLocked;
	lockon_enemy = nullptr;
	switch (state)
	{
	case	State::Idle:
	case	State::Move:
	case	State::Jump:
	case	State::Land:
	case	State::Attack:
	case	State::Damage:
		{
			// ロックオンモード
			if (Input::Instance().GetGamePad().GetButton() & GamePad::BTN_LEFT_TRIGGER)
			{
				DirectX::XMVECTOR vec_position, vec_target, vec_vector;
				switch (old_lockon_state)
				{
				case	LockonState::NotLocked:
				{
					// 一番近い距離のキャラクターを検索
					float	length1, length2;
					EnemyManager& manager = EnemyManager::Instance();
					for (int i = 0; i < manager.GetEnemyCount(); ++i)
					{
						std::shared_ptr<Enemy> enemy = manager.GetEnemy(i);
						std::shared_ptr<Actor> enemy_actor = enemy->GetActor();

						if (lockon_state != LockonState::NotLocked)
						{
							vec_position = DirectX::XMLoadFloat3(&position);
							vec_target = DirectX::XMLoadFloat3(&lockon_enemy->GetActor()->GetPosition());
							vec_vector = DirectX::XMVectorSubtract(vec_target, vec_position);
							DirectX::XMStoreFloat(&length2, DirectX::XMVector3LengthSq(vec_vector));
							vec_position = DirectX::XMLoadFloat3(&position);
							vec_target = DirectX::XMLoadFloat3(&enemy_actor->GetPosition());
							vec_vector = DirectX::XMVectorSubtract(vec_target, vec_position);
							DirectX::XMStoreFloat(&length1, DirectX::XMVector3LengthSq(vec_vector));
							if (length1 < length2)
							{
								lockon_enemy = enemy;
								DirectX::XMStoreFloat3(&lock_direction, DirectX::XMVector3Normalize(vec_vector));
							}
						}
						else
						{
							vec_position = DirectX::XMLoadFloat3(&position);
							vec_target = DirectX::XMLoadFloat3(&enemy_actor->GetPosition());
							vec_vector = DirectX::XMVectorSubtract(vec_target, vec_position);
							DirectX::XMStoreFloat(&length1, DirectX::XMVector3LengthSq(vec_vector));

							lockon_enemy = enemy;
							DirectX::XMStoreFloat3(&lock_direction, DirectX::XMVector3Normalize(vec_vector));
							lockon_state = LockonState::Locked;
						}
					}
					break;
				}
				case	LockonState::Locked:
				{
					// ロックオン対象が存在しているかチェックして
					// 対象がいればロックオンを継続させる
					EnemyManager& manager = EnemyManager::Instance();
					for (int i = 0; i < manager.GetEnemyCount(); ++i)
					{
						std::shared_ptr<Enemy> enemy = manager.GetEnemy(i);
						std::shared_ptr<Actor> enemy_actor = enemy->GetActor();

						if (enemy == old_lockon_enemy)
						{
							lockon_enemy = enemy;
							lockon_state = LockonState::Locked;
							vec_position = DirectX::XMLoadFloat3(&position);
							vec_target = DirectX::XMLoadFloat3(&enemy_actor->GetPosition());
							vec_vector = DirectX::XMVectorSubtract(vec_target, vec_position);

							lockon_enemy = enemy;
							DirectX::XMStoreFloat3(&lock_direction, DirectX::XMVector3Normalize(vec_vector));
							break;
						}
					}
					// 右スティックでロックオン対象を変更する処理
					GamePad& gamePad = Input::Instance().GetGamePad();
					float ax = gamePad.GetAxisRX();	// 水平のみ
					// 垂直方向は使わないでおく
					lockon_target_change_time -= 60.0f * elapsed_time;
					if (lockon_enemy &&
						lockon_target_change_time <= 0 &&
						ax * ax > 0.3f)
					{
						lockon_target_change_time = lockon_target_change_time_max;
						// ロックオン対象と自分自身の位置からベクトルを算出
						float dx = old_lockon_enemy_actor->GetPosition().x - position.x;
						float dz = old_lockon_enemy_actor->GetPosition().z - position.z;
						float l = sqrtf(dx * dx + dz * dz);
						dx /= l;
						dz /= l;
						// 外積を用いて左右判定を行い、角度的に最も近い対象にロックオンを変える
						float angleMax = FLT_MAX;
						for (int i = 0; i < manager.GetEnemyCount(); ++i)
						{
							std::shared_ptr<Enemy> enemy = manager.GetEnemy(i);
							std::shared_ptr<Actor> enemy_actor = enemy->GetActor();
							if (enemy == old_lockon_enemy)
								continue;
							float ddx = enemy_actor->GetPosition().x - position.x;
							float ddz = enemy_actor->GetPosition().z - position.z;
							float ll = sqrtf(ddx * ddx + ddz * ddz);
							ddx /= ll;
							ddz /= ll;
							float cross = dx * ddz - dz * ddx;
							if (ax > 0 && cross < 0)
							{
								cross = abs(cross);
								if (cross < angleMax)
								{
									angleMax = cross;
									lockon_enemy = enemy;
								}
							}
							else if (ax < 0 && cross > 0)
							{
								if (cross < angleMax)
								{
									angleMax = cross;
									lockon_enemy = enemy;
								}
							}
						}
					}
					break;
				}
				}
				// ロックオン状態ならロックオンカメラに変更
				if (lockon_state == LockonState::Locked)
				{
					MessageData::CameraChangeLockonModeData	lockoncamera_data = { position, lockon_enemy->GetActor()->GetPosition() };
					Messenger::Instance().SendData(MessageData::CameraChangeLockonMode, &lockoncamera_data);
					break;
				}
			}

			MessageData::CameraChangeFreeModeData	freecamera_data = { position };
			Messenger::Instance().SendData(MessageData::CameraChangeFreeMode, &freecamera_data);
			break;
		}
	case	State::Death:
		{
			// 死亡時用のカメラモーション
			MessageData::CameraChangeMotionModeData	motioncamera_data;
			float vx = sinf(angle.y) * 10;
			float vz = cosf(angle.y) * 10;
			motioncamera_data.data.push_back({ 0, {position.x + vx, position.y + 50, position.z + vz }, position });
			motioncamera_data.data.push_back({ 90, {position.x + vx, position.y + 20, position.z + vz }, position });
			Messenger::Instance().SendData(MessageData::CameraChangeMotionMode, &motioncamera_data);
			break;
		}
	case	State::Revive:
		{
			// 復活時用のカメラモーション
			MessageData::CameraChangeMotionModeData	motioncamera_data;
			float vx = sinf(angle.y + DirectX::XM_PIDIV2) * 40;
			float vz = cosf(angle.y + DirectX::XM_PIDIV2) * 40;
			motioncamera_data.data.push_back({ 0, {position.x + vx, position.y + 40, position.z + vz }, position });
			motioncamera_data.data.push_back({ 30, {position.x + vx, position.y + 35, position.z + vz }, position });
			Messenger::Instance().SendData(MessageData::CameraChangeMotionMode, &motioncamera_data);
			break;
		}
	}
}
