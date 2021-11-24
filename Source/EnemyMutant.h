#pragma once
#include "Enemy.h"

class EnemyMutant : public Enemy
{
public:
	EnemyMutant();
	~EnemyMutant() override;
	// ���O�擾
	const char* GetName() const override { return "Mutant"; }
	void OnGUI() override;
	// �J�n����
	void Start() override;
	// �X�V����
	void Update(float elapsed_time) override;
	void PlayAnimation(std::shared_ptr<AnimationData> animation) override;
	void DrawDebugPrimitive() override;
	// ���b�Z�[�W����M�����Ƃ��̏���
	bool OnMessages(const Telegram& message)override { return true; };

private:
	enum Animation
	{
		Anim_Idle1,
		Anim_Idle2,
		Anim_Idle3,
		Anim_Walking,
		Anim_Running,
		Anim_Punch,
		Anim_Swiping,
		Anim_JumpAttack,
		Anim_Dath
	};
};