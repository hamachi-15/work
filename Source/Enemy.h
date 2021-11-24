#pragma once
#include "Component.h"
#include "Movement.h"
//#include "Charactor.h"
#include "AnimationData.h"

class BehaviorTree;
class BehaviorData;
class NodeBase;
class Telegram;
class Charactor;
class Enemy : public Component
{
public:
	// �R���X�g���N�^
	Enemy();
	// �f�X�g���N�^
	~Enemy() override;

	// ���O�擾
	const char* GetName() const override { return name.c_str(); }
	
	// ���O�ݒ�
	void SetName(const char* name) { this->name = name; }
	
	// GUI�`��
	void OnGUI() override {};
	
	// �J�n����
	void Start() override {};
	
	// �X�V����
	void Update(float elapsed_time) override {};
	
	// �A�C�f���e�B�e�B�ݒ�
	void SetIdentity(int identity) { this->identity = identity; }
	
	// �A�C�f���e�B�e�B�擾
	int GetIdentity() { return this->identity; }
	
	// ������͈̓f�o�b�O�v���~�e�B�u�`��
	virtual void DrawDebugPrimitive(){}
	
	// �G�̔j��
	virtual void Destroy() {}
	
	// ���[�u�����g�̐ݒ�
	void SetMovement(std::shared_ptr<Movement> movement) { this->movement = movement; }
	
	// ���[�u�����g�̎擾
	std::shared_ptr<Movement> GetMovement() const { return movement; }
	
	// ���[�u�����g�̐ݒ�
	void SetCharactor(std::shared_ptr<Charactor> charactor) { this->charactor = charactor; }
	
	// ���[�u�����g�̎擾
	std::shared_ptr<Charactor> GetCharactor() const { return charactor; }

	// �͈͓��Ƀv���C���[�����Ȃ����T��
	bool SearchPlayer();
	
	// �ړI�n�_�ֈړ�
	void MoveToTarget(float elapsed_time, float speed_rate);
	
	// �^�[�Q�b�g�ʒu�������_���ݒ�
	void SetRandomTargetPosition();
	
	// �^�[�Q�b�g�ʒu��ݒ�
	void SetTargetPosition(const DirectX::XMFLOAT3& target_position) { this->target_position = target_position; }
	
	// �^�[�Q�b�g�ʒu���擾
	DirectX::XMFLOAT3 GetTargetPosition() const { return target_position; }
	
	// �e���g���[�̌��_�ݒ�
	void SetTerritoryOrigin(const DirectX::XMFLOAT3& territory_origin) { this->territory_origin = territory_origin; }
	
	// �e���g���[�̌��_�擾
	DirectX::XMFLOAT3 GetTerritoryOrigin() const { return territory_origin; }
	
	// �v���C���[�̍U���R���W�����̈ʒu�ݒ�
	void SetHitPosition(const DirectX::XMFLOAT3& hit_position) { this->hit_position = hit_position; }

	// �v���C���[�̍U���R���W�����̈ʒu�擾
	const DirectX::XMFLOAT3& GetHitPosition() const { return hit_position; }

	// �e���g���[�͈͐ݒ�
	void SetTerritoryRange(float territory_range) { this->territory_range = territory_range; }

	// �e���g���[�͈͎擾
	float GetTerritoryRange() const { return territory_range; }

	// �U���͈͐ݒ�
	void SetAttackRange(const float& attack_range) { this->attack_range = attack_range; }

	// �U���͈͎擾
	float GetAttackRange() const { return attack_range; }


	// ���G�͈͐ݒ�
	void SetSearchRange(const float search_range) { this->search_range = search_range; }

	// �U���t���O�ݒ�
	void SetAttackFlag(bool attack_flag) { this->attack_flag = attack_flag; }
	
	// �U���t���O�擾
	bool GetAttackFlag() const { return attack_flag; }
	
	float GetRunTimer() const { return run_timer; }

	void SetRunTimer(const float& run_timer) { this->run_timer = run_timer; }

	// �A�j���[�V�����Đ�
	virtual void PlayAnimation(std::shared_ptr<AnimationData> animation) {}

	// �_���[�W����
	void OnDamaged();

	// ���S����
	void OnDead();

	// ���b�Z�[�W����M�����Ƃ��̏���
	virtual bool OnMessages(const Telegram& message) = 0;

private:
	int					identity;			// ���Ԗڂ̓G��
	bool				attack_flag = false;		// �U���t���O
	std::shared_ptr<Movement>  movement;
	std::shared_ptr<Charactor> charactor;
protected:
	DirectX::XMFLOAT3	target_position = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3	territory_origin = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3	hit_position = { 0.0f, 0.0f, 0.0f };	// �v���C���[�̍U���R���W�����̈ʒu(���g�ƏՓ˂����Ƃ��Ɏg��)
	std::string			name;
	BehaviorTree*		ai_tree = nullptr;
	BehaviorData*		behavior_data = nullptr;
	NodeBase*			active_node = nullptr;
	NodeBase*			old_active_node = nullptr;
	float				run_timer = 0.0f;
	float				territory_range = 10.0f;
	float				search_range = 5.0f;
	float				attack_range = 1.5f;
};
