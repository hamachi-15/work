#pragma once
#include <string>
#include "Actor.h"

//**********************************
// 
// �R���|�[�l���g�擾
// 
//**********************************
class Component
{
public:
	// �R���X�g���N�^
	Component() {}

	// �f�X�g���N�^
	virtual ~Component() {}

	// ���O�擾
	virtual std::string GetName() const = 0;

	// �J�n����
	virtual void Start() {}

	// �X�V����
	virtual void Update(float elapsed_time) {}

	// GUI�`��
	virtual void OnGUI() {}

	// �A�N�^�[�ݒ�
	void SetActor(std::shared_ptr<Actor> actor) { this->actor = actor; }

	// �A�N�^�[�擾
	std::shared_ptr<Actor> GetActor() { return actor.lock(); }
private:
	std::weak_ptr<Actor> actor;
};