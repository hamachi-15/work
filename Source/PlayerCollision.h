#pragma once
#include "Collision.h"

class Effect;
class Actor;

//************************************
// 
// �v���C���[�̃R���W����
// 
//************************************
class PlayerCollision : public Component
{
public:
	// �R���X�g���N�^
	PlayerCollision() {}
	
	// �f�X�g���N�^
	~PlayerCollision();
	
	// ���O�擾
	std::string GetName() const override { return this->name; }

	// GUI�`��
	void OnGUI() override;
	
	// �J�n����
	void Start() override;

	// �X�V����
	void Update(float elapsed_time) override;

	// �R���W�����X�V����
	void UpdateCollision(std::shared_ptr<CollisionObject> collision,std::shared_ptr<Actor> actor, Model* model);

	// �Փˎ��̃��A�N�V��������
	void Reaction(int receiver, const Message& message);

	// �~���R���W�����擾
	std::shared_ptr<CollisionCylinder> GetPlayerBodyCollision() { return collision_cylinder; }

private:
	std::shared_ptr<Effect>							hit_effect = nullptr;
	float											hit_effect_scale = 5.0f;
	std::shared_ptr<CollisionCylinder>				collision_cylinder;
	std::vector<std::shared_ptr<CollisionSphere>>	collision_spheres;
	std::string										name = "";
};