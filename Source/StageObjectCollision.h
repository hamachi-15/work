#pragma once
#include "Collision.h"

class Actor;
enum class ActorType;

class StageObjectCollision : public Component
{
public:
	// �R���X�g���N�^
	StageObjectCollision(ActorType category) : category(category){}
	
	// �f�X�g���N�^
	~StageObjectCollision() {}

	// ���O�擾
	std::string GetName() const { return "StageObjectCollision"; }

	// GUI�`��
	void OnGUI() override;

	// �J�n����
	void Start() override;

	// �X�V����
	void Update(float elapsed_time) override;

	// �~���R���W�����擾
	std::shared_ptr<CollisionCylinder> GetPlayerBodyCollision() { return collision_cylinder; }
private:
	std::shared_ptr<CollisionCylinder>				collision_cylinder;
	ActorType category;
};