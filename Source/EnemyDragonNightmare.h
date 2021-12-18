#pragma once
#include "Enemy.h"

//--------------------------------
// �i�C�g���A�h���S���N���X
//--------------------------------
class EnemyDragonNightmare : public Enemy
{
public:
	// �R���X�g���N�^
	EnemyDragonNightmare();

	// �f�X�g���N�^
	~EnemyDragonNightmare()override;

	// GUI�`��
	void OnGUI() override;

	// �J�n����
	void Start() override;

	// �X�V����
	void Update(float elapsed_time)override;

	// ������͈̓f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive()override;

	// ���b�Z�[�W����M�����Ƃ��̏���
	bool OnMessages(const Telegram& message)override;

	// �A�j���[�V�����Đ�
	void PlayAnimation(std::shared_ptr<AnimationData> animation);
};