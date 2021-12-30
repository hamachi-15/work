#pragma once
#include "Enemy.h"

//**********************************
// 
// �v���b�g�N���X
// 
//**********************************
class EnemyPLT : public Enemy
{
public:
	// �R���X�g���N�^
	EnemyPLT();

	// �f�X�g���N�^
	~EnemyPLT() override;

	// �j������
	void Destroy() override;

	// GUI�`��
	void OnGUI() override;

	// ���b�Z�[�W����M�����Ƃ��̏���
	bool OnMessages(const Telegram& message) override;

	// �J�n����
	void Start() override;
	
	// �r�w�C�r�A�̃m�[�h�ݒ菈��
	void SetBehaviorNode() override;

	// �X�V����
	void Update(float elapsed_time) override;

	// ������͈̓f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive() override;

private:
	std::string right_hand_collision_name;
};