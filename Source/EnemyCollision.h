#pragma once
#include "Collision.h"

enum class EnemyCategory;
class Effect;

//************************************
// 
// �v���C���[�̃R���W����
// 
//************************************
class EnemyCollision : public Component
{
public:
	// �R���X�g���N�^
	EnemyCollision(EnemyCategory enemy_category, int identity) : enemy_category(enemy_category), identity(identity){}

	// �f�X�g���N�^
	~EnemyCollision();

	// ���O�擾
	std::string GetName() const override { return this->name; }

	// GUI�`��
	void OnGUI() override;

	// �J�n����
	void Start() override;

	// �X�V����
	void Update(float elapsed_time) override;

	// �R���W�����̍X�V����
	void UpdateCollision(std::shared_ptr<CollisionObject> collision, std::shared_ptr<Actor> actor, Model* model);

	// �Փˎ��̃��A�N�V��������
	void Reaction(int receiver, const Message& message);

	// �~���R���W�����擾
	std::shared_ptr<CollisionCylinder> GetEnemyBodyCollision() { return collision_cylinder; }
private:
	std::shared_ptr<Effect>							hit_effect = nullptr;
	float											hit_effect_scale = 5.0f;
	std::shared_ptr<CollisionCylinder>				collision_cylinder;
	std::vector<std::shared_ptr<CollisionSphere>>	collision_spheres;
	std::string										name = "";
	bool											attack_falg = false;
	EnemyCategory									enemy_category; // �f�[�^�x�[�X�̃R���W�����f�[�^�����Ɏg��
	int												identity = 0;
};