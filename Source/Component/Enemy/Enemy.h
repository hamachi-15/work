#pragma once
#include "Component.h"
#include "Movement.h"

enum class EnemyTerritoryTag;
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
	std::string GetName() const override { return name.c_str(); }
	
	// ���O�ݒ�
	void SetName(const char* name) { this->name = name; }
	
	// GUI�`��
	void OnGUI() override {};
	
	// �J�n����
	void Start() override {};
	
	// �X�V����
	void Update(float elapsed_time) override {};
	
	// �r�w�C�r�A�X�V����
	void BehaviorUpdate(float elapsed_time);

	// ������͈̓f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive();
	
	// �G�̔j������
	virtual void Destroy() = 0;

	// �G�l�~�[�f�[�^ID�ݒ�
	void SetEnemyDataID(int id) { this->id = id; }

	// �G�l�~�[�f�[�^ID�擾
	int GetEnemyDataID() const { return id; }

	// �A�C�f���e�B�e�B�ݒ�
	void SetIdentity(int identity) { this->identity = identity; }
	
	// �A�C�f���e�B�e�B�擾
	int GetIdentity() { return this->identity; }
	
	// �r�w�C�r�A�̃m�[�h�ݒ菈��
	virtual void SetBehaviorNode() = 0;

	// ���[�u�����g�̐ݒ�
	void SetMovement(std::shared_ptr<Movement> movement) { this->movement = movement; }
	
	// ���[�u�����g�̎擾
	std::shared_ptr<Movement> GetMovement() const { return movement; }
	
	// �L�����N�^�[�̐ݒ�
	void SetCharactor(std::shared_ptr<Charactor> charactor) { this->charactor = charactor; }
	
	// �L�����N�^�[�̎擾
	std::shared_ptr<Charactor> GetCharactor() const { return charactor; }

	// �͈͓��Ƀv���C���[�����Ȃ����T��
	bool SearchPlayer();
	
	// �ړI�n�_�ֈړ�
	void MoveToTarget(float speed_rate);

	// �w��̕����ֈړ�
	void MoveToDirection(DirectX::XMFLOAT3& direction, float speed_rate);

	// �ڕW�̃x�N�g���֌���
	float TurnToTarget(DirectX::XMFLOAT3& direction);

	// �^�[�Q�b�g�ʒu�������_���ݒ�
	void SetRandomTargetPosition();
	
	// �^�[�Q�b�g�ʒu��ݒ�
	void SetTargetPosition(const DirectX::XMFLOAT3& target_position) { this->target_position = target_position; }
	
	// �^�[�Q�b�g�ʒu���擾
	DirectX::XMFLOAT3 GetTargetPosition() const { return target_position; }
	
	// �v���C���[�̍U���R���W�����̈ʒu�ݒ�
	void SetHitPosition(const DirectX::XMFLOAT3& hit_position) { this->hit_position = hit_position; }

	// �v���C���[�̍U���R���W�����̈ʒu�擾
	const DirectX::XMFLOAT3& GetHitPosition() const { return hit_position; }

	// �e���g���[�^�O�ݒ�
	void SetBelongingToTerritory(EnemyTerritoryTag teritory_tag) { this->teritory_tag = teritory_tag; }

	// �e���g���[�^�O�擾
	const EnemyTerritoryTag& GetBelongingToTerritory() const { return teritory_tag; }
	
	// �U���͈͐ݒ�
	void SetAttackRange(const float& attack_range) { this->attack_range = attack_range; }

	// �U���͈͎擾
	float GetAttackRange() const { return attack_range; }

	// ���G�͈͐ݒ�
	void SetSearchRange(const float search_range) { this->search_range = search_range; }
	
	// �{���ԃt���O�ݒ�
	void SetAngerFlag(bool anger_flag) { this->anger_flag = anger_flag; }

	// �{���ԃt���O�擾
	const bool& GetAngerFlag() const { return anger_flag; }

	// �U�����擾
	bool GetRightOfAttack() const { return rightofattack; }
	
	// �U�����ݒ�
	void SetRightOfAttack(bool flag) { rightofattack = flag; }

	float GetRunTimer() const { return run_timer; }

	void SetRunTimer(const float& run_timer) { this->run_timer = run_timer; }

	// �A�j���[�V�����Đ�
	virtual void PlayAnimation(const char* animation_name);

	// ���b�Z�[�W����M�����Ƃ��̏���
	virtual bool OnMessages(const Telegram& message) = 0;

	// �r�w�C�r�A��GUI�`��
	void DrawBehaviorGUI();

private:
	int							id;
	int							identity;			// ���Ԗڂ̓G��
	bool						anger_flag = false;				 // �{���ԃt���O
	bool						rightofattack = false;
	std::shared_ptr<Movement>	movement;
	std::shared_ptr<Charactor>	charactor;
	EnemyTerritoryTag			teritory_tag;

	DirectX::XMFLOAT4 primitive_color_blue = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	DirectX::XMFLOAT4 primitive_color_purple = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	DirectX::XMFLOAT4 primitive_color_red = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMFLOAT4 primitive_color_green = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
protected:
	DirectX::XMFLOAT3	target_position = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3	hit_position = { 0.0f, 0.0f, 0.0f };	// �v���C���[�̍U���R���W�����̈ʒu(���g�ƏՓ˂����Ƃ��Ɏg��)
	std::string			name;
	BehaviorTree*		ai_tree = nullptr;
	BehaviorData*		behavior_data = nullptr;
	NodeBase*			active_node = nullptr;
	NodeBase*			old_active_node = nullptr;
	float				run_timer = 0.0f;
	float				search_range = 5.0f;	 // ���G�͈�
	float				attack_range = 1.5f;	 // �U���Ɉڂ�͈�
};
