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
// ImGui�`��
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
// �J�n����
//-----------------------------------------
void Player::Start()
{
	// �A�N�^�[�̎擾
	std::shared_ptr<Actor> actor = GetActor();

	// �L�����N�^�[�̎擾
	charactor = actor->GetComponent<Charactor>();

	movement = actor->GetComponent<Movement>();

	// ���������̃X�s�[�h�{���ݒ�
	charactor->SetRunSpeedScale(1.5f);
	
	// �ҋ@��Ԃ֑J��
	TransitionIdleState();
}

//-----------------------------------------
// �X�V����
//-----------------------------------------
void Player::Update(float elapsed_time)
{
	// �J�����X�e�[�g�̍X�V����
	UpdateCameraState(elapsed_time);

	// ��Ԃ��Ƃ̍X�V����
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

	// �R�}���h�����N���A����
	CommandListClear(elapsed_time);

	// �_���[�W�t���O�������Ă����
	if (charactor->GetDamageFlag())
	{
		// �_���[�W��ԂɑJ��
		TransitionDamageState();
	}

	// ���S�t���O�������Ă����
	if (charactor->GetDeathFlag())
	{
		// ���S��ԂɑJ��
		TransitionDeathState();
	}
}


//-----------------------------------------------
// ���b�Z�[�W����M�����Ƃ��̏���
//-----------------------------------------------
bool Player::OnMessages(const Telegram& message)
{
	switch (message.message_box.message)
	{// �v���C���[�̍U�����G�ɓ�������
	case MessageType::Message_Hit_Attack:

		// �U���q�b�g�t���O�𗧂Ă�
		charactor->SetHitAttackFlag(true);
		// �q�b�g�X�g�b�v���s���t���[����
		charactor->SetHitStopFrame();
		charactor->SetHitStopFlag(true);

		// �q�b�gSE�Đ�
		AudioManager::Instance().PlaySoundEffect(SEType::SwordHit);
		return true;
		break;
	// �G�̍U�����v���C���[�ɓ�������
	case MessageType::Message_GetHit_Attack:
		// �q�b�gSE�Đ�
		AudioManager::Instance().PlaySoundEffect(SEType::PlayerGetHit);

		return true;
		break;
	case MessageType::Message_Hit_Boddy:

		break;
	}
	return false;
}

//-----------------------------------------
// �ҋ@��Ԃ֑J��
//-----------------------------------------
void Player::TransitionIdleState()
{
	state = State::Idle;
	sub_state = SubState::None;
	// �ҋ@�A�j���[�V�����Đ�
	Model* model = GetActor()->GetModel();
	if (model != nullptr)
	{
		// �A�j���[�V�����f�[�^�擾
		std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("PlayerIdle");
		// �A�j���[�V�����Đ�
		model->PlayAnimation(animation->number, animation->roop_flag, animation->blend);
	}
}

//-----------------------------------------
// �ړ���Ԃ֑J��
//-----------------------------------------
void Player::TransitionMoveState()
{
	state = State::Move;
	// RT�{�^����������Ă���Ƃ��̓����j���O��ԂɑJ�ځB����ȊO�̎��͕�����ԂɑJ��
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
// ������Ԃ֑J��
//-----------------------------------------
void Player::TransitionMoveWalkState()
{
	sub_state = SubState::Move_Walk;
	// �ړ��A�j���[�V�����Đ�
	Model* model = GetActor()->GetModel();
	if (model != nullptr)
	{
		// �A�j���[�V�����f�[�^�擾
		std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("PlayerWalking");
		// �A�j���[�V�����Đ�
		model->PlayAnimation(animation->number, animation->roop_flag, animation->blend);
	}
}

//-----------------------------------------
// �����j���O��Ԃ֑J��
//-----------------------------------------
void Player::TransitionMoveRunState()
{
	sub_state = SubState::Move_Running;
	// �ړ��A�j���[�V�����Đ�
	Model* model = GetActor()->GetModel();
	if (model != nullptr)
	{
		// �A�j���[�V�����f�[�^�擾
		std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("PlayerRunning");
		// �A�j���[�V�����Đ�
		model->PlayAnimation(animation->number, animation->roop_flag, animation->blend);
	}
}

//-----------------------------------------
// �U����Ԃ֑J��
//-----------------------------------------
void Player::TransitionAttackState(SubState sub_state)
{
	// �X�e�[�g���U����Ԃ�
	state = State::Attack;

	// �T�u�X�e�[�g����U���̎�
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
	// �T�u�X�e�[�g�����U����������
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
	// �T�u�X�e�[�g��ݒ�
	this->sub_state = sub_state;
}


//-----------------------------------------
// ��U����Ԃ֑J��
//-----------------------------------------
void Player::TransitionLightAttackState()
{
	sub_state = SubState::Light_Attack;
}

//-----------------------------------------
// �_�b�V����U��1�i�ڍX�V����
//-----------------------------------------
void Player::TransitionDushLightAttackState()
{
	attack_conbo_state = AttackConbo::Dush_Light_Attack;
	// �ړ��A�j���[�V�����Đ�
	Model* model = GetActor()->GetModel();
	if (model != nullptr)
	{
		// �A�j���[�V�����f�[�^�擾
		std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("PlayerDushLightAttack");
		// �A�j���[�V�����Đ�
		model->PlayAnimation(animation->number, animation->roop_flag, animation->blend);
	}
}

//-----------------------------------------
// ��U��1�i�ڍX�V����
//-----------------------------------------
void Player::TransitionSlashLightAttackState()
{
	attack_conbo_state = AttackConbo::Light_Attack_Conbo1;
	// �ړ��A�j���[�V�����Đ�
	Model* model = GetActor()->GetModel();
	if (model != nullptr)
	{
		// �A�j���[�V�����f�[�^�擾
		std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("PlayerSmartSlash");
		// �A�j���[�V�����Đ�
		model->PlayAnimation(animation->number, animation->roop_flag, animation->blend);
	}
}

//-----------------------------------------
// �E�L�b�N�U���X�V����
//-----------------------------------------
void Player::TransitionRightKickAttackState()
{
	attack_conbo_state = AttackConbo::Light_Attack_Conbo2;
	// �ړ��A�j���[�V�����Đ�
	Model* model = GetActor()->GetModel();
	if (model != nullptr)
	{
		// �A�j���[�V�����f�[�^�擾
		std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("PlayerKickRight");
		// �A�j���[�V�����Đ�
		model->PlayAnimation(animation->number, animation->roop_flag, animation->blend);
	}
}

//-----------------------------------------
// ���L�b�N�U���X�V����
//-----------------------------------------
void Player::TransitionLeftKickAttackState()
{
	attack_conbo_state = AttackConbo::Light_Attack_Conbo3;
	// �ړ��A�j���[�V�����Đ�
	Model* model = GetActor()->GetModel();
	if (model != nullptr)
	{
		// �A�j���[�V�����f�[�^�擾
		std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("PlayerKickLeft");
		// �A�j���[�V�����Đ�
		model->PlayAnimation(animation->number, animation->roop_flag, animation->blend);
	}
}

//-----------------------------------------
// ���U����Ԃ֑J��
//-----------------------------------------
void Player::TransitionHeavyAttackState()
{
	// �T�u�X�e�[�g�����U����
	sub_state = SubState::Heavy_Attack;
}

//-----------------------------------------
// �_�b�V�����U��1�i�ڍX�V����
//-----------------------------------------
void Player::TransitionDushHeavyAttackState()
{
	attack_conbo_state = AttackConbo::Dush_Heavy_Attack;
	// �_�b�V���U���A�j���[�V�����Đ�
	Model* model = GetActor()->GetModel();
	if (model != nullptr)
	{
		// �A�j���[�V�����f�[�^�擾
		std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("PlayerDushHeavyAttack");
		// �A�j���[�V�����Đ�
		model->PlayAnimation(animation->number, animation->roop_flag, animation->blend);
	}
}

//-----------------------------------------
// ���U��1�i�ڍX�V����
//-----------------------------------------
void Player::TransitionSlashAttackState()
{
	attack_conbo_state = AttackConbo::Heavy_Attack_Conbo1;
	// �ړ��A�j���[�V�����Đ�
	Model* model = GetActor()->GetModel();
	if (model != nullptr)
	{
		// �A�j���[�V�����f�[�^�擾
		std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("PlayerSlash");
		// �A�j���[�V�����Đ�
		model->PlayAnimation(animation->number, animation->roop_flag, animation->blend);
	}
}

//-----------------------------------------
// ���U��2�i�ڍX�V����
//-----------------------------------------
void Player::TransitionSideSlashAttackState()
{
	attack_conbo_state = AttackConbo::Heavy_Attack_Conbo2;
	// �ړ��A�j���[�V�����Đ�
	Model* model = GetActor()->GetModel();
	if (model != nullptr)
	{
		// �A�j���[�V�����f�[�^�擾
		std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("PlayerSideSlash");
		// �A�j���[�V�����Đ�
		model->PlayAnimation(animation->number, animation->roop_flag, animation->blend);
	}
}

//-----------------------------------------
// ���U��3�i�ڍX�V����
//-----------------------------------------
void Player::TransitionSmartSlashAttackState()
{
	attack_conbo_state = AttackConbo::Conbo_End_Attack;
	// �ړ��A�j���[�V�����Đ�
	Model* model = GetActor()->GetModel();
	if (model != nullptr)
	{
		// �A�j���[�V�����f�[�^�擾
		std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("PlayerFullSwingAttack");
		// �A�j���[�V�����Đ�
		model->PlayAnimation(animation->number, animation->roop_flag, animation->blend);
	}
}

//-----------------------------------------
// �����Ԃ֑J��
//-----------------------------------------
void Player::TransitionAvoidState()
{
	std::shared_ptr<Actor> actor = GetActor();
	state = State::Avoid;
	// ���݂̊p�x����O�����x�N�g��������o���ړ�	
	DirectX::XMFLOAT3 angle = actor->GetAngle();
	float frontX = sinf(angle.y);
	float frontZ = cosf(angle.y);
	movement->Move({ frontX, 0.0f, frontZ }, 80);

	// �_���[�W�A�j���[�V�����Đ�
	Model* model = GetActor()->GetModel();
	if (model != nullptr)
	{
		// �A�j���[�V�����f�[�^�擾
		std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("PlayerRollBackward");
		// �A�j���[�V�����Đ�
		model->PlayAnimation(animation->number, animation->roop_flag, animation->blend);
	}
}

//-----------------------------------------
// �_���[�W��Ԃ֑J��
//-----------------------------------------
void Player::TransitionDamageState()
{
	state = State::Damage;
	// �_���[�W�A�j���[�V�����Đ�
	Model* model = GetActor()->GetModel();
	if (model != nullptr)
	{
		// �A�j���[�V�����f�[�^�擾
		std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("PlayerGetHit1");
		// �A�j���[�V�����Đ�
		model->PlayAnimation(animation->number, animation->roop_flag, animation->blend);
	}
	charactor->SetDamageFlag(false);
}

//-----------------------------------------
// ���S��Ԃ֑J��
//-----------------------------------------
void Player::TransitionDeathState()
{
	state = State::Death;
	std::shared_ptr<Actor> actor = GetActor();
	Model* model = actor->GetModel();

	// ���S�A�j���[�V�����Đ�
	if (model != nullptr)
	{
		// �A�j���[�V�����f�[�^�擾
		std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("PlayerDeath");
		// �A�j���[�V�����Đ�
		model->PlayAnimation(animation->number, animation->roop_flag, animation->blend);
	}

	// ���S�̏u�Ԃ�`��
	Message message;
	message.message = MessageType::Message_Moment_Render;
	MetaAI::Instance().SendMessaging(
		static_cast<int>(MetaAI::Identity::Player),   // ���M��
		static_cast<int>(MetaAI::Identity::WorldMap),    // ��M��
		message);                                        // ���b�Z�[�W

	// ���S�t���O�����ɖ߂��Ă���(�܂������ɏ��������Ȃ��悤��)
	charactor->SetDeathFlag(false);
}

//-----------------------------------------
// ������Ԃ֑J��
//-----------------------------------------
void Player::TransitionReviveState()
{
	std::shared_ptr<Actor> actor = GetActor();
	Model* model = actor->GetModel();

	state = State::Revive;

	// �̗͉�
	charactor->SetHealth(charactor->GetMaxHealth());

	// �����A�j���[�V�����Đ�
	if (model != nullptr)
	{
		// �A�j���[�V�����f�[�^�擾
		std::shared_ptr<AnimationData> animation = GameDataBase::Instance().GetAnimationData("PlayerRevive");
		// �A�j���[�V�����Đ�
		model->PlayAnimation(animation->number, animation->roop_flag, animation->blend);
	}
}

//-----------------------------------------
// �ҋ@��ԍX�V����
//-----------------------------------------
void Player::UpdateIdleState(float elapsed_time)
{
	std::shared_ptr<Actor> actor = GetActor();
	Model* model = actor->GetModel();
	// �ړ����͏���
	if (InputMove(elapsed_time, charactor->GetMoveSpeed()))
	{
		// �ړ���Ԃ֑J��
		TransitionMoveState();
	}

	// �U�����͏���
	SubState return_state = InputAttack();
	if (return_state != SubState::None)
	{
		TransitionAttackState(return_state);
	}

	//if (InputAvoid())
	//{
	//	// �����Ԃ֑J��
	//	TransitionAvoidState();
	//}
}

//-----------------------------------------
// �ړ���ԍX�V����
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
	//	// �����Ԃ֑J��
	//	TransitionAvoidState();
	//}
}

//-----------------------------------------
// ������ԍX�V����
//-----------------------------------------
void Player::UpdateMoveWalkState(float elapsed_time)
{
	// �ړ����͏���
	if (!InputMove(elapsed_time, charactor->GetMoveSpeed()))
	{
		// �ҋ@��ԑJ��
		TransitionIdleState();
	}
	else if (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_RIGHT_TRIGGER)
	{
		// �ҋ@��ԑJ��
		TransitionMoveRunState();
	}

	// �U�����͏���
	SubState return_state = InputAttack();
	if (return_state != SubState::None)
	{
		TransitionAttackState(return_state);
	}
}

//-----------------------------------------
// �����j���O��ԍX�V����
//-----------------------------------------
void Player::UpdateMoveRunState(float elapsed_time)
{
	std::shared_ptr<Actor> actor = GetActor();
	Model* model = actor->GetModel();
	// �ړ����͏���
	// TODO �}�W�b�N�i���o�[
	if (!InputMove(elapsed_time, charactor->GetMoveSpeed() * 1.5f))
	{
		// �ҋ@��ԑJ��
		TransitionIdleState();
	}else if (Input::Instance().GetGamePad().GetButtonUp() & GamePad::BTN_RIGHT_TRIGGER)
	{
		// �ړ���ԑJ��
		TransitionMoveWalkState();
	}

	// �U�����͏���
	SubState return_state = InputAttack();
	if (return_state != SubState::None)
	{
		TransitionAttackState(return_state);
	}
}

//-----------------------------------------
// �U����ԍX�V����
//-----------------------------------------
void Player::UpdateAttackState(float elapsed_time)
{
	std::shared_ptr<Actor> actor = GetActor();
	Model* model = actor->GetModel();

	// �e�U����ԍX�V�֐���
	switch (sub_state)
	{
	case SubState::Light_Attack:	UpdateLightAttackState(elapsed_time);	break;
	case SubState::Heavy_Attack:	UpdateHeavyAttackState(elapsed_time);	break;
	}
	
	// �U���R�}���h���͏���
	InputAttack();

	// �U���A�j���[�V�����I����
	if (!model->IsPlayAnimation())
	{
		charactor->SetHitAttackFlag(false);
		// �R���{���N���A����
		attack_conbo_state = AttackConbo::None;
		// �ҋ@��Ԃ֑J��
		TransitionIdleState();
	}
}

//-----------------------------------------
// ���U����ԍX�V����
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
// �_�b�V�����U��1�i�ڍX�V����
//-----------------------------------------
void Player::UpdateDushHeavyAttackState(float elapsed_time)
{
	// TODO �A�j���[�V�����̊J�n���ԂƏI�����Ԃ��f�[�^�x�[�X����擾
	std::shared_ptr<AttackCollitionTime> data = GameDataBase::Instance().GetAttackCollitionTimeData(AttackCategory::DushHeavyAttack);
	std::shared_ptr<Actor> actor = GetActor();
	// ���݂̃R���W�����t���O��1�t���[���O�̃R���W�����t���O���擾
	bool old_collision_time_flag = charactor->GetOldCollisionTimeFlag();
	
	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	bool collision_time_flag = charactor->SearchAnimationTime(actor, 0.2f, 0.6f);

	// �R���W�����t���O���ω�������
	if (old_collision_time_flag != collision_time_flag)
	{
		// �R���W�����̂����蔻��̃I���I�t��؂�ւ���
		SetOnOrOffWepponCollisionFlag(collision_time_flag);
	}
	// �O�t���[���̃R���W�����t���O�����B�R���W�����t���O���ω������^�C�~���O�𒲂ׂ�p
	charactor->SetOldCollisionTimeFlag(collision_time_flag);

	// ���t���[�����ɑ����Ď�U��������Ǝ��̃R���{�ֈڍs
	if (CommandCheck(GamePad::BTN_Y) && charactor->SearchAnimationTime(actor, 0.6f, 0.8f))
	{
		charactor->SetHitAttackFlag(false);
		// �E�L�b�N�U����ԍX�V����
		TransitionSideSlashAttackState();
	}
}

//-----------------------------------------
// ���U��1�i�ڍX�V����
//-----------------------------------------
void Player::UpdateSlashAttackState(float elapsed_time)
{
	std::shared_ptr<Actor> actor = GetActor();
	// ���݂̃R���W�����t���O��1�t���[���O�̃R���W�����t���O���擾
	bool old_collision_time_flag = charactor->GetOldCollisionTimeFlag();
	
	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	bool collision_time_flag = charactor->SearchAnimationTime(actor, 0.2f, 0.6f);

	// �R���W�����t���O���ω�������
	if (old_collision_time_flag != collision_time_flag)
	{
		// �R���W�����̂����蔻��̃I���I�t��؂�ւ���
		SetOnOrOffWepponCollisionFlag(collision_time_flag);
	}
	// �O�t���[���̃R���W�����t���O�����B�R���W�����t���O���ω������^�C�~���O�𒲂ׂ�p
	charactor->SetOldCollisionTimeFlag(collision_time_flag);

	if (CommandCheck(GamePad::BTN_Y) && charactor->SearchAnimationTime(actor, 0.6f, 0.8f))
	{
		charactor->SetHitAttackFlag(false);
		// �U���֑J��
		TransitionSideSlashAttackState();
	}
}

//-----------------------------------------
// ���U��2�i�ڍX�V����
//-----------------------------------------
void Player::UpdateSideSlashAttackState(float elapsed_time)
{
	std::shared_ptr<Actor> actor = GetActor();
	// ���݂̃R���W�����t���O��1�t���[���O�̃R���W�����t���O���擾
	bool old_collision_time_flag = charactor->GetOldCollisionTimeFlag();
	
	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	bool collision_time_flag = charactor->SearchAnimationTime(actor, 0.2f, 0.6f);

	// �R���W�����t���O���ω�������
	if (old_collision_time_flag != collision_time_flag)
	{
		// �R���W�����̂����蔻��̃I���I�t��؂�ւ���
		SetOnOrOffWepponCollisionFlag(collision_time_flag);
	}
	// �O�t���[���̃R���W�����t���O�����B�R���W�����t���O���ω������^�C�~���O�𒲂ׂ�p
	charactor->SetOldCollisionTimeFlag(collision_time_flag);

	// �C�ӂ̃A�j���[�V�����Đ���Ԃŋ��U�������͂��ꂽ�玟�̍U����Ԃ֑J��
	if (CommandCheck(GamePad::BTN_Y) && charactor->SearchAnimationTime(actor, 0.6f, 0.8f))
	{
		charactor->SetHitAttackFlag(false);
		// ���L�b�N�U���֑J��
		TransitionLeftKickAttackState();
	}
}

//-----------------------------------------
// ���U��3�i�ڍX�V����
//-----------------------------------------
void Player::UpdateSmartSlashAttackState(float elapsed_time)
{
	std::shared_ptr<Actor> actor = GetActor();
	// ���݂̃R���W�����t���O��1�t���[���O�̃R���W�����t���O���擾
	bool old_collision_time_flag = charactor->GetOldCollisionTimeFlag();
	
	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	bool collision_time_flag = charactor->SearchAnimationTime(actor, 0.2f, 0.6f);

	// �R���W�����t���O���ω�������
	if (old_collision_time_flag != collision_time_flag)
	{
		// �R���W�����̂����蔻��̃I���I�t��؂�ւ���
		SetOnOrOffWepponCollisionFlag(collision_time_flag);
	}
	// �O�t���[���̃R���W�����t���O�����B�R���W�����t���O���ω������^�C�~���O�𒲂ׂ�p
	charactor->SetOldCollisionTimeFlag(collision_time_flag);
}

//-----------------------------------------
// ��U����ԍX�V����
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
// �_�b�V����U����ԏ���
//-----------------------------------------
void Player::UpdateDushLightAttackState(float elapsed_time)
{
	std::shared_ptr<Actor> actor = GetActor();
	// ���݂̃R���W�����t���O��1�t���[���O�̃R���W�����t���O���擾
	bool old_collision_time_flag = charactor->GetOldCollisionTimeFlag();
	
	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	bool collision_time_flag = charactor->SearchAnimationTime(actor, 0.2f, 0.6f);
	
	// �R���W�����t���O���ω�������
	if (old_collision_time_flag != collision_time_flag)
	{
		// �R���W�����̂����蔻��̃I���I�t��؂�ւ���
		SetOnOrOffWepponCollisionFlag(collision_time_flag);
	}
	// �O�t���[���̃R���W�����t���O�����B�R���W�����t���O���ω������^�C�~���O�𒲂ׂ�p
	charactor->SetOldCollisionTimeFlag(collision_time_flag);

	// �C�ӂ̃A�j���[�V�����Đ���Ԃɓ��������Ɏ�U���{�^�������͂���Ă���Ύ��̍U���֑J��
	if (CommandCheck(GamePad::BTN_B) && charactor->SearchAnimationTime(actor, 0.6f, 0.8f))
	{
		charactor->SetHitAttackFlag(false);
		// ���L�b�N�U���֑J��
		TransitionLeftKickAttackState();
	}
}

//-----------------------------------------
// ��U����i�ڏ�ԍX�V����
//-----------------------------------------
void Player::UpdateSlashLightAttackState(float elpased_time)
{
	std::shared_ptr<Actor> actor = GetActor();
	// ���݂̃R���W�����t���O��1�t���[���O�̃R���W�����t���O���擾
	bool old_collision_time_flag = charactor->GetOldCollisionTimeFlag();
	
	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	bool collision_time_flag = charactor->SearchAnimationTime(actor, 0.2f, 0.6f);

	// �R���W�����t���O���ω�������
	if(old_collision_time_flag != collision_time_flag)
	{
		// �R���W�����̂����蔻��̃I���I�t��؂�ւ���
		SetOnOrOffWepponCollisionFlag(collision_time_flag);
	}
	// �O�t���[���̃R���W�����t���O�����B�R���W�����t���O���ω������^�C�~���O�𒲂ׂ�p
	charactor->SetOldCollisionTimeFlag(collision_time_flag);

	// �C�ӂ̃A�j���[�V�����Đ���ԂɎ�U���{�^���������ꂽ��
	if (CommandCheck(GamePad::BTN_B) && charactor->SearchAnimationTime(actor, 0.6f, 0.8f))
	{
		charactor->SetHitAttackFlag(false);
		// ���L�b�N�U���֑J��
		TransitionLeftKickAttackState();
	}

}

//-----------------------------------------
// ���L�b�N�U���X�V����
//-----------------------------------------
void Player::UpdateLeftKickAttackState(float elapsed_time)
{
	std::shared_ptr<Actor> actor = GetActor();
	// ���݂̃R���W�����t���O��1�t���[���O�̃R���W�����t���O���擾
	bool old_collision_time_flag = charactor->GetOldCollisionTimeFlag();
	
	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	bool collision_time_flag = charactor->SearchAnimationTime(actor, 0.2f, 0.6f);
	
	// �R���W�����t���O���ω�������
	if (old_collision_time_flag != collision_time_flag)
	{
		// �R���W�����̂����蔻��̃I���I�t��؂�ւ���
		SetOnOrOffFootCollisionFlag(collision_time_flag, "PlayerLeftFoot");
	}
	// �O�t���[���̃R���W�����t���O�����B�R���W�����t���O���ω������^�C�~���O�𒲂ׂ�p
	charactor->SetOldCollisionTimeFlag(collision_time_flag);

	// �C�ӂ̃A�j���[�V�����Đ���ԂɎ�U���{�^���������ꂽ��
	if (CommandCheck(GamePad::BTN_B) && charactor->SearchAnimationTime(actor, 0.6f, 0.8f))
	{
		// �E�L�b�N�U���֑J��
		TransitionRightKickAttackState();
	}
	// �C�ӂ̃A�j���[�V�����Đ���Ԃɋ��U���{�^���������ꂽ��
	else if (CommandCheck(GamePad::BTN_Y) && charactor->SearchAnimationTime(actor, 0.6f, 0.8f))
	{
		charactor->SetHitAttackFlag(false);
		// �U���֑J��
		TransitionSmartSlashAttackState();
	}
}


//-----------------------------------------
// �E�L�b�N�U���X�V����
//-----------------------------------------
void Player::UpdateRightKickAttackState(float elapsed_time)
{
	std::shared_ptr<Actor> actor = GetActor();
	// ���݂̃R���W�����t���O��1�t���[���O�̃R���W�����t���O���擾
	bool old_collision_time_flag = charactor->GetOldCollisionTimeFlag();
	
	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	bool collision_time_flag = charactor->SearchAnimationTime(actor, 0.2f, 0.6f);
	
	// �R���W�����t���O���ω�������
	if (old_collision_time_flag != collision_time_flag)
	{
		// �R���W�����̂����蔻��̃I���I�t��؂�ւ���
		SetOnOrOffFootCollisionFlag(collision_time_flag, "PlayerRightFoot");
	}
	// �O�t���[���̃R���W�����t���O�����B�R���W�����t���O���ω������^�C�~���O�𒲂ׂ�p
	charactor->SetOldCollisionTimeFlag(collision_time_flag);

	// �C�ӂ̃A�j���[�V�����Đ���ԂɎ�U���{�^���������ꂽ��InputLightAttack() && charactor->SearchAnimationTime(actor, 0.6f, 0.8f)
	if (CommandCheck(GamePad::BTN_B) && charactor->SearchAnimationTime(actor, 0.6f, 0.8f))
	{
		charactor->SetHitAttackFlag(false);
		TransitionSmartSlashAttackState();
	}
}


//-----------------------------------------
// �����ԍX�V����
//-----------------------------------------
void Player::UpdateAvoidState(float elapsed_time)
{
	CollisionManager& collision_manager = CollisionManager::Instance();
	std::shared_ptr<Actor> actor = GetActor();
	Model* model = actor->GetModel();
	// ���݂̃R���W�����t���O��1�t���[���O�̃R���W�����t���O���擾
	bool old_collision_time_flag = charactor->GetOldCollisionTimeFlag();
	
	bool collision_time_flag;
	
	// �C�ӂ̃A�j���[�V�����Đ���Ԃł͏Փ˔��菈�������Ȃ�
	if (charactor->SearchAnimationTime(actor, 0.2f, 0.7f))
	{
		collision_time_flag = false;
	}
	else
	{
		collision_time_flag = true;
	}
	// �R���W�����t���O���ω�������
	if (old_collision_time_flag != collision_time_flag)
	{
		// �R���W�����̂����蔻��̃I���I�t��؂�ւ���
		actor->GetComponent<PlayerCollision>()->GetPlayerBodyCollision()->SetCollisionFlag(collision_time_flag);
	}
	// �O�t���[���̃R���W�����t���O�����B�R���W�����t���O���ω������^�C�~���O�𒲂ׂ�p
	charactor->SetOldCollisionTimeFlag(collision_time_flag);

	// �C�ӂ̃A�j���[�V�����Đ���Ԃł݈̂ړ����͏������󂯓����
	if (charactor->SearchAnimationTime(actor, 0.6f, 0.8f))
	{
		if (InputMove(elapsed_time, charactor->GetMoveSpeed()))
		{
			actor->GetComponent<PlayerCollision>()->GetPlayerBodyCollision()->SetCollisionFlag(collision_time_flag);
			TransitionMoveState();
		}
	}

	// �A�j���[�V�������I��������
	if (!model->IsPlayAnimation())
	{
		// �ҋ@��Ԃ֑J��
		TransitionIdleState();
	}
}

//-----------------------------------------
// �_���[�W��ԍX�V����
//-----------------------------------------
void Player::UpdateDamageState(float elapsed_time)
{
	std::shared_ptr<Actor> actor = GetActor();
	Model* model = actor->GetModel();
	// �_���[�W�A�j���[�V�������I�������ҋ@��Ԃ֑J��
	if (!model->IsPlayAnimation())
	{
		TransitionIdleState();
	}
}

//-----------------------------------------
// ���S��ԍX�V����
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
				static_cast<int>(MetaAI::Identity::Player),   // ���M��
				static_cast<int>(MetaAI::Identity::WorldMap),    // ��M��
				message);                                        // ���b�Z�[�W
		}
	}
}

//-----------------------------------------
// ������ԍX�V����
//-----------------------------------------
void Player::UpdateReviveState(float elapsed_time)
{
	std::shared_ptr<Actor> actor = GetActor();
	Model* model = actor->GetModel();
	// �����A�j���[�V�����I����ɑҋ@��Ԃ֑J��
	if (!model->IsPlayAnimation())
	{
		TransitionIdleState();
	}
}

//-----------------------------------------
// �X�e�B�b�N���͒l����ړ��x�N�g�����Z�o
//-----------------------------------------
DirectX::XMFLOAT3 Player::GetMoveVec() const
{
	// ���͏����擾
	GamePad& gamepad = Input::Instance().GetGamePad();
	float ax = gamepad.GetAxisLX();
	float ay = gamepad.GetAxisLY();

	// �J���������ƃX�e�B�b�N���͒l�ɂ���Đi�s�������v�Z����
	Camera& camera = Camera::Instance();
	const DirectX::XMFLOAT3& camera_right = camera.GetRight();
	const DirectX::XMFLOAT3 & camera_front = camera.GetFront();

	// �J�����E�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
	float camera_rightX = camera_right.x;
	float camera_rightZ = camera_right.z;
	float camera_right_length = sqrtf(camera_rightX * camera_rightX + camera_rightZ * camera_rightZ);
	if (camera_right_length > 0.0f)
	{
		// �P�ʃx�N�g����
		camera_rightX /= camera_right_length;
		camera_rightZ /= camera_right_length;
	}

	// �J�����O�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
	float camera_frontX = camera_front.x;
	float camera_frontZ = camera_front.z;
	float camera_front_length = sqrtf(camera_frontX * camera_frontX + camera_frontZ * camera_frontZ);
	if (camera_right_length > 0.0f)
	{
		// �P�ʃx�N�g����
		camera_frontX /= camera_front_length;
		camera_frontZ /= camera_front_length;
	}

	// �X�e�B�b�N�̐������͒l���J�����E�����ɔ��f���A
	// �X�e�B�b�N�̐������͒l���J�����O�����ɔ��f���A
	// �i�s�x�N�g�����v�Z
	DirectX::XMFLOAT3 vec;
	vec.x = (camera_rightX * ax) + (camera_frontX * ay);
	vec.z = (camera_rightZ * ax) + (camera_frontZ * ay);
	// Y�������ɂ͈ړ����Ȃ�
	vec.y = 0.0f;

	return vec;
}

//----------------------------------------------
// ����R���W�����̂����蔻��t���O��؂�ւ��鏈��
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
// ���R���W�����̂����蔻��t���O��؂�ւ��鏈��
//---------------------------------------------
void Player::SetOnOrOffFootCollisionFlag(bool flag, const char* node_name)
{
	CollisionManager& collision_manager = CollisionManager::Instance();
	std::shared_ptr<Actor> actor = GetActor();
	Model* model = actor->GetModel();

	collision_manager.GetCollisionSphereFromName(node_name)->SetAttackFlag(flag);
}

//-----------------------------------------
// �ړ����͏���
//-----------------------------------------
bool Player::InputMove(float elapsed_time, float move_speed)
{
	// �i�s�x�N�g�����擾
	DirectX::XMFLOAT3 move_vec = GetMoveVec();

	// �ړ�����
	movement->Move(move_vec, move_speed);

	// ���񏈗�
	movement->Turn(move_vec);

	return move_vec.x != 0.0f || move_vec.y != 0.0f || move_vec.z;
}

//-----------------------------------------
// �U�����͏���
//-----------------------------------------
Player::SubState Player::InputAttack()
{
	GamePad& gamepad = Input::Instance().GetGamePad();
	// ��U��
	if (gamepad.GetButtonDown() & GamePad::BTN_B)
	{
		// ���͗����ɒǉ�
		AddCommandList(GamePad::BTN_B);
		return SubState::Light_Attack;
	}
	// ���U��
	if (gamepad.GetButtonDown() & GamePad::BTN_Y)
	{
		// ���͗����ɒǉ�
		AddCommandList(GamePad::BTN_Y);
		return SubState::Heavy_Attack;
	}
	return SubState::None;
}

//-----------------------------------------
// �R�}���h�`�F�b�N
//-----------------------------------------
bool Player::CommandCheck(GamePadButton command)
{
	// ������������Δ��肵�Ȃ�
	if (input_history.size() == 0) return false;

	// �Ō�ɓ��͂��ꂽ�R�}���h�Ɠ���Ȃ�true
	if (input_history.front() == command) return true;

	return false;
}

//-----------------------------------------
// �R�}���h���X�g���N���A
//-----------------------------------------
void Player::CommandListClear(float elapsed_time)
{
	// �R�}���h�̗L�����Ԃ��I��������
	if (command_life_timer <= 0.0f)
	{
		// �R�}���h���X�g���N���A
		std::list<GamePadButton>::iterator iterat = input_history.begin();
		for (; iterat != input_history.end(); iterat = input_history.begin())
		{
			input_history.erase(iterat);
		}
	}

	// �R�}���h���Ԃ̗L�����Ԃ����炷
	command_life_timer -= elapsed_time;
}

//-----------------------------------------
// �R�}���h���X�g�ɃR�}���h��ǉ�
//-----------------------------------------
void Player::AddCommandList(GamePadButton command)
{
	// �R�}���h�̗L�����Ԃ�0.0�b�ȉ��Ȃ�L�����Ԃ�ݒ�
	if (command_life_timer <= 0.0f) command_life_timer = 0.5f;

	// �R�}���h������ǉ�
	input_history.push_front(command);
}

//-----------------------------------------
// �����͏���
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
// �J�����X�e�[�g�̍X�V
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
			// ���b�N�I�����[�h
			if (Input::Instance().GetGamePad().GetButton() & GamePad::BTN_LEFT_TRIGGER)
			{
				DirectX::XMVECTOR vec_position, vec_target, vec_vector;
				switch (old_lockon_state)
				{
				case	LockonState::NotLocked:
				{
					// ��ԋ߂������̃L�����N�^�[������
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
					// ���b�N�I���Ώۂ����݂��Ă��邩�`�F�b�N����
					// �Ώۂ�����΃��b�N�I�����p��������
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
					// �E�X�e�B�b�N�Ń��b�N�I���Ώۂ�ύX���鏈��
					GamePad& gamePad = Input::Instance().GetGamePad();
					float ax = gamePad.GetAxisRX();	// �����̂�
					// ���������͎g��Ȃ��ł���
					lockon_target_change_time -= 60.0f * elapsed_time;
					if (lockon_enemy &&
						lockon_target_change_time <= 0 &&
						ax * ax > 0.3f)
					{
						lockon_target_change_time = lockon_target_change_time_max;
						// ���b�N�I���ΏۂƎ������g�̈ʒu����x�N�g�����Z�o
						float dx = old_lockon_enemy_actor->GetPosition().x - position.x;
						float dz = old_lockon_enemy_actor->GetPosition().z - position.z;
						float l = sqrtf(dx * dx + dz * dz);
						dx /= l;
						dz /= l;
						// �O�ς�p���č��E������s���A�p�x�I�ɍł��߂��ΏۂɃ��b�N�I����ς���
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
				// ���b�N�I����ԂȂ烍�b�N�I���J�����ɕύX
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
			// ���S���p�̃J�������[�V����
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
			// �������p�̃J�������[�V����
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
