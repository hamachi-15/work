#pragma once
#include "Enemy.h"

//********************************
// 
// �~���[�^���g�N���X
// 
//********************************
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
	// �r�w�C�r�A�̃m�[�h�ݒ菈��
	void SetBehaviorNode() override;
	// �G�̔j������
	void Destroy() override;
	// �X�V����
	void Update(float elapsed_time) override;
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
