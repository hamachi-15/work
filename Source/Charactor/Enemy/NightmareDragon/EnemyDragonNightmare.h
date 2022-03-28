#pragma once
#include "Enemy.h"

//********************************
// 
// �i�C�g���A�h���S���N���X
// 
//********************************
class EnemyDragonNightmare : public Enemy
{
public:
	// �R���X�g���N�^
	EnemyDragonNightmare();

	// �f�X�g���N�^
	~EnemyDragonNightmare() override;

	// GUI�`��
	void OnGUI() override;

	// ���b�Z�[�W����M�����Ƃ��̏���
	bool OnMessages(const Telegram& message) override;

	// �J�n����
	void Start() override;

	// �r�w�C�r�A�̃m�[�h�ݒ菈��
	void SetBehaviorNode() override;

	// �G�̔j������
	void Destroy() override;

	// �X�V����
	void Update(float elapsed_time) override;
};