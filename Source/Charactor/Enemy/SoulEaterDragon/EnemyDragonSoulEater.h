#pragma once
#include "Enemy.h"

//*********************************
// 
// �\�E���C�[�^�[�h���S���N���X
// 
//*********************************
class EnemyDragonSoulEater : public Enemy
{
public:
	// �R���X�g���N�^
	EnemyDragonSoulEater();

	// �f�X�g���N�^
	~EnemyDragonSoulEater()override;

	// GUI�`��
	void OnGUI() override;

	// ���b�Z�[�W����M�����Ƃ��̏���
	bool OnMessages(const Telegram& message)override;

	// �J�n����
	void Start() override;

	// �r�w�C�r�A�̃m�[�h�ݒ菈��
	void SetBehaviorNode() override;

	// �G�̔j������
	void Destroy() override;

	// �X�V����
	void Update(float elapsed_time)override;
};