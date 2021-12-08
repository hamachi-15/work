#pragma once
#include "UI.h"
#include "Charactor.h"

//------------------------------
// �v���C���[HP�N���X
//------------------------------
class PlayerHealth : public UI
{
public:
	// �R���X�g���N�^
	PlayerHealth(const UIParameter& parameter) : UI(parameter) {}

	// �f�X�g���N�^
	~PlayerHealth() override;

	// �X�V����
	void Update(float elapsed_time);

	// �L�����N�^�R���|�[�l���g�ݒ�
	void SetCharactor(std::shared_ptr<Charactor> charactor) { this->charactor = charactor; }
private:
	std::shared_ptr<Charactor> charactor;
};