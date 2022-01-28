#pragma once
#include "Component.h"
#include "CollisionDataHeader.h"

class Telegram;

//**************************************
//
// �L�����N�^�[�N���X
//
//**************************************
class Charactor : public Component
{
public:
	// �R���X�g���N�^
	Charactor(){}

	// �f�X�g���N�^
	~Charactor() override {}

	// ���O�擾
	std::string GetName() const override { return "Charactor"; }

	// �J�n����
	void Start()override;

	// �X�V����
	void Update(float elapsed_time)override;

	// GUI�`��
	void OnGUI()override;

	// �U���q�b�g�t���O�ݒ�
	void SetHitAttackFlag(bool flag) { hit_attack_flag = flag; }

	// �U���q�b�g�t���O�擾
	bool GetHitAttackFlag() const { return hit_attack_flag; }

	// �_���[�W�t���O�ݒ�
	void SetDamageFlag(bool flag) { damage_flag = flag; }

	// �_���[�W�t���O�擾
	bool GetDamageFlag() const { return damage_flag; }

	// �q�b�g�X�g�b�v�t���O�̐ݒ�
	void SetHitStopFlag(bool hitstop_flag) { this->hitstop_flag = hitstop_flag; }

	// �q�b�g�X�g�b�v�t���O�̎擾
	bool GetHitStopFlag() const { return hitstop_flag; }

	// 1�t���[���O�̓����蔻����s�����Ԃ̃t���O�擾
	void SetOldCollisionTimeFlag(bool flag) { this->old_collision_time_flag = flag; }

	//  1�t���[���O�̓����蔻����s�����Ԃ̃t���O�ݒ�
	bool GetOldCollisionTimeFlag() const { return old_collision_time_flag; }

	// ���S�t���O�ݒ�
	void SetDeathFlag(bool flag) { this->death_flag = flag; }

	// ���S�t���O�擾
	bool GetDeathFlag() const { return death_flag; }

	// �̗͂�ݒ�
	void SetHealth(int health) { this->health = health; }

	// �̗͂��擾
	int GetHealth() const { return health; }

	// �ő�̗͂�ݒ�
	void SetMaxHealth(int max_health) { this->max_health = max_health; }

	// �ő�̗͂��擾
	int GetMaxHealth() const { return max_health; }

	// �ړ����x�ݒ�
	void SetMoveSpeed(float speed) { this->move_speed = speed; }

	// �ړ����x�擾
	float GetMoveSpeed() const { return this->move_speed; }

	// �ړ����x�ݒ�
	void SetTurnSpeed(float speed) { this->turn_speed = speed; }

	// ���莞�̔{���ݒ�
	void SetRunSpeedScale(float speed_scale) { this->run_speed_scale = speed_scale; }

	// ���莞�̔{���擾
	float GetRunSpeedScale() { return this->run_speed_scale; }

	// �ړ����x�擾
	float GetTurnSpeed() const { return this->turn_speed; }

	// �q�b�g�X�g�b�v�^�C�}�[�ݒ�
	void SetHitStopTimer(float hitstop_timer) { this->hitstop_timer = hitstop_timer; }

	// �q�b�g�X�g�b�v�^�C�}�[�擾
	float GetHitStopTimer() const { return hitstop_timer; }

	// �m�[�h���W���擾
	//void GetNodePosition(const char* node_name, DirectX::XMFLOAT3& node_position, Model* model);

	// ���ʔԍ��擾
	int GetID() const { return id; }

	// ���ʔԍ��ݒ�
	void SetID(int id) { this->id = id; }

	// �_���[�W��^����
	bool ApplyDamage(int damage, float invincible_time);

	// �R���W�����̓o�^
	void SetCollision(std::shared_ptr<Actor> actor, CollisionParameter& collision_status, CollisionMeshType type);

	// ���b�Z�[�W����M�����Ƃ��̏���
	virtual bool OnMessages(const Telegram& message) { return false; }

	// �A�j���[�V�����͈̔͋�Ԕ���
	bool SearchAnimationTime(std::shared_ptr<Actor> actor, float start_time, float end_time);

	// �q�b�g�X�g�b�v�^�C�}�[�̍X�V����
	void UpdateHitStop(float elapsed_time);

	// ���G���ԍX�V
	void UpdateInvincibleTimer(float elapsed_time);

	// �f�o�b�O�pGUI�`��
	virtual	void DrawDebugGUI() {}

	// �f�o�b�O�v���~�e�B�u�`��
	virtual	void DrawDebugPrimitive() {}

	// ���n�������ɌĂ΂��
	virtual void OnLanding() {}

	// �_���[�W���󂯂����ɌĂ΂��
	void OnDamaged() { damage_flag = true; }

	// ���S�������ɌĂ΂��
	void OnDead() { death_flag = true; }
	bool GetDead() const { return death_flag; }
private:
	int		id = 0;							// �A�N�^�[�̎��ʔԍ�
	bool	hit_attack_flag = false;	// �U�����q�b�g�������̃t���O
	bool	old_collision_time_flag = false;
	
	int		max_health = 5;
	int		health = 5;
	float	invincible_timer = 0.0f;
	float	move_speed = 30.0f;
	float	run_speed_scale = 0.0f;
	float	turn_speed = DirectX::XMConvertToRadians(360);
	bool	damage_flag = false;		// �_���[�W���󂯂Ă��邩�̃t���O
	bool	death_flag = false;			// ���S��Ԃ��̃t���O
	float	hitstop_timer = 0.0f;		// �q�b�g�X�g�b�v�^�C�}�[
	bool	hitstop_flag = false;		// �q�b�g�X�g�b�v���s�����̃t���O

	int level;							// ���x��
	int exp;							// ���݂̌o���l
	int next_exp;						// ���̃��x���܂ł̌o���l
};
