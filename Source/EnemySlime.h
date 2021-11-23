#pragma once
#include "Enemy.h"
#include "Collision.h"
#include "AnimationData.h"

class EnemySlime : public Enemy
{
public:
	EnemySlime();
	~EnemySlime() override;
	// GUI�`��
	void OnGUI() override;

	// �J�n����
	void Start() override;

	// �X�V����
	void Update(float elapsed_time) override;

	// ������͈̓f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive()override;

	// �G�̔j��
	void Destroy()override;

	// ���b�Z�[�W����M�����Ƃ��̏���
	bool OnMessages(const Telegram& message)override;

	// �A�j���[�V�����Đ�
	void PlayAnimation(std::shared_ptr<AnimationData> animation);
private:
	std::string haed_collision_name;
	DirectX::XMFLOAT3 head_position;
};