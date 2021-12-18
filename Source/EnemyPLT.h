#pragma once
#include "Enemy.h"

class EnemyPLT : public Enemy
{
public:
	// �R���X�g���N�^
	EnemyPLT();

	// �f�X�g���N�^
	~EnemyPLT() override;

	// GUI�`��
	void OnGUI() override;

	// �J�n����
	void Start() override;
	
	// �r�w�C�r�A�̃m�[�h�ݒ菈��
	void SetBehaviorNode() override;

	// �j������
	void Destroy() override;

	// �X�V����
	void Update(float elapsed_time) override;

	// ������͈̓f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive()override;

	// ���b�Z�[�W����M�����Ƃ��̏���
	bool OnMessages(const Telegram& message)override;

	// �A�j���[�V�����Đ�����
	void PlayAnimation(std::shared_ptr<AnimationData> animation);
private:
	std::string right_hand_collision_name;
};