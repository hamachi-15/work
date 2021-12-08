#pragma once
#include "UI.h"

class Charactor;

//--------------------------------------
// �v���C���[HP�̐ԃQ�[�W�N���X
//--------------------------------------
class PlayerUIRedHealth : public UI
{
public:
	// �R���X�g���N�^
	PlayerUIRedHealth(const UIParameter& parameter) : UI(parameter){}
	// �f�X�g���N�^
	~PlayerUIRedHealth() override {}

	// �X�V����
	void Update(float elapsed_time) override;
	
	// �L�����N�^�R���|�[�l���g�ݒ�
	void SetCharactor(std::shared_ptr<Charactor> charactor) { this->charactor = charactor; }
private:
	float speed = 100.0f; // �Q�[�W���������鑬�x
	std::shared_ptr<Charactor> charactor;
};