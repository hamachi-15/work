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

	// �r�w�C�r�A�̃m�[�h�ݒ菈��
	void SetBehaviorNode() override;
	
	// �X�V����
	void Update(float elapsed_time) override;

	// �G�̔j��
	void Destroy()override;

	// ���b�Z�[�W����M�����Ƃ��̏���
	bool OnMessages(const Telegram& message)override;

private:
	std::string haed_collision_name;
	DirectX::XMFLOAT3 head_position;
};