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
	// �R���X�g���N�^
	Player(){}

	// �f�X�g���N�^
	~Player()override{}

	// ���O�擾
	std::string GetName() const override { return "Player"; }

	// GUI�`��
	void OnGUI() override;

	// �J�n����
	void Start() override;

	// �X�V����
	void Update(float elapsed_time) override;

	// ���b�Z�[�W����M�����Ƃ��̏���
	bool OnMessages(const Telegram& message);

private: // �v���C���[�̏�ԏ���
	// �ҋ@��Ԃ֑J��
	void TransitionIdleState();

	// �ړ���Ԃ֑J��
	void TransitionMoveState();
	
	// ������Ԃ֑J��
	void TransitionMoveWalkState();

	// �����j���O��Ԃ֑J��
	void TransitionMoveRunState();

	// �U����Ԃ֑J��
	void TransitionAttackState(SubState sub_state);

	// ���U����ԑJ��
	void TransitionHeavyAttackState();

	// �_�b�V�����U��1�i�ڍX�V����
	void TransitionDushHeavyAttackState();

	// ���U��1�i�ڍX�V����
	void TransitionSlashAttackState();

	// ���U��2�i�ڍX�V����
	void TransitionSideSlashAttackState();

	// ���U��3�i�ڍX�V����
	void TransitionSmartSlashAttackState();

	// ��U����ԑJ��
	void TransitionLightAttackState();

	// �_�b�V����U��1�i�ڍX�V����
	void TransitionDushLightAttackState();

	// ��U��1�i�ڍX�V����
	void TransitionSlashLightAttackState();

	// �E�L�b�N�U���X�V����
	void TransitionRightKickAttackState();

	// ���L�b�N�U���X�V����
	void TransitionLeftKickAttackState();

	// �����Ԃ֑J��
	void TransitionAvoidState();

	// �_���[�W��Ԃ֑J��
	void TransitionDamageState();

	// ���S��Ԃ֑J��
	void TransitionDeathState();

	// ������Ԃ֑J��
	void TransitionReviveState();

	// �ҋ@��ԍX�V����
	void UpdateIdleState(float elapsed_time);

	// �ړ���ԍX�V����
	void UpdateMoveState(float elapsed_time);

	// ������ԍX�V����
	void UpdateMoveWalkState(float elapsed_time);

	// �����j���O��ԍX�V����
	void UpdateMoveRunState(float elapsed_time);

	// �U����ԍX�V����
	void UpdateAttackState(float elapsed_time);

	// ���U����ԍX�V����
	void UpdateHeavyAttackState(float elapsed_time);
	
	// �_�b�V�����U��1�i�ڍX�V����
	void UpdateDushHeavyAttackState(float elapsed_time);
	
	// ���U��1�i�ڍX�V����
	void UpdateSlashAttackState(float elapsed_time);

	// ���U��2�i�ڍX�V����
	void UpdateSideSlashAttackState(float elapsed_time);

	// ���U��3�i�ڍX�V����
	void UpdateSmartSlashAttackState(float elapsed_time);

	// ��U����ԍX�V����
	void UpdateLightAttackState(float elpased_time);

	// �_�b�V����U��1�i�ڍX�V����
	void UpdateDushLightAttackState(float elapsed_time);

	// ��U��1�i�ڍX�V����
	void UpdateSlashLightAttackState(float elpased_time);
		
	// �E�L�b�N�U���X�V����
	void UpdateRightKickAttackState(float elapsed_time);
	
	// ���L�b�N�U���X�V����
	void UpdateLeftKickAttackState(float elapsed_time);

	// �����ԍX�V����
	void UpdateAvoidState(float elapsed_time);

	// �_���[�W��ԍX�V����
	void UpdateDamageState(float elapsed_time);

	// ���S��ԍX�V����
	void UpdateDeathState(float elapsed_time);

	// ������ԍX�V����
	void UpdateReviveState(float elapsed_time);

private:
	// �X�e�B�b�N���͒l����ړ��x�N�g�����Z�o
	DirectX::XMFLOAT3 GetMoveVec() const;

	// ����R���W�����̂����蔻��t���O��؂�ւ��鏈��
	void SetOnOrOffWepponCollisionFlag(bool flag);

	// ���R���W�����̂����蔻��t���O��؂�ւ��鏈��
	void SetOnOrOffFootCollisionFlag(bool flag, const char* node_name);

	// �ړ����͏���
	bool InputMove(float elapsed_time, float move_speed);

	// �U�����͏���
	SubState InputAttack();

	// �R�}���h�`�F�b�N
	bool CommandCheck(GamePadButton command);

	// �R�}���h���X�g���N���A
	void CommandListClear(float elapsed_time);
	
	// �R�}���h���X�g�ɃR�}���h��ǉ�
	void AddCommandList(GamePadButton command);

	// �����͏���
	bool InputAvoid();

	// �J�����X�e�[�g�̍X�V
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

	// ���b�N�I���X�e�[�g
	enum class LockonState
	{
		NotLocked,
		Locked,
	};

	enum class AttackConbo
	{
		None,
		Heavy_Attack_Conbo1,		// ���U��1�i�ڍU��
		Dush_Heavy_Attack,			// �_�b�V�����U��1�i�ڍU��
		Dush_Light_Attack,			// �؂�グ��
		Heavy_Attack_Conbo2,		// ������E�֓ガ����
		Light_Attack_Conbo1,		// �؂�グ��BHardSlash����q����U��
		Light_Attack_Conbo2,
		Light_Attack_Conbo3,
		Conbo_End_Attack,			// ���U����U���̃R���{�ŏI�U��
	};

	// ����̂����蔻��̃��[�J�����W���i�[����p
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
	// �J����
	LockonState					lockon_state = LockonState::NotLocked;
	float						lockon_target_change_time = 0;
	float						lockon_target_change_time_max = 8;
	std::shared_ptr<Enemy>		lockon_enemy;
	DirectX::XMFLOAT3			lock_direction;

	float timer = 0.0f;
};