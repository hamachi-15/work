#pragma once
#include "Enemy.h"

class EnemyLAT : public Enemy
{
public:
	// �R���X�g���N�^
	EnemyLAT();

	// �f�X�g���N�^
	~EnemyLAT() override;

	// GUI�`��
	void OnGUI() override;

	// �J�n����
	void Start() override;

	// �X�V����
	void Update(float elapsed_time) override;

	// ������͈̓f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive()override;

	// ���b�Z�[�W����M�����Ƃ��̏���
	bool OnMessages(const Telegram& message)override;

	// �A�j���[�V�����Đ�
	void PlayAnimation(std::shared_ptr<AnimationData> animation);
};