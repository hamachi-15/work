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

	// ������͈̓f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive() override;
private:
	// �A���ːi�̃^�[�Q�b�g���W
	DirectX::XMFLOAT3 lunges_target_position_data[5] =
	{
		{-305.0f, 5.8f, 205.0f},
		{-207.0f, 8.8f, 261.0f},
		{-381.0f, 4.5f, 212.0f},
		{-113.0f, 2.5f, 108.0f},
		{-346.0f, 4.8f, 237.0f},
	};
};