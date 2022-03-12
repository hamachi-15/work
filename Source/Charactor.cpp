#include "ImGuiRenderer.h"
#include "Charactor.h"
#include "Model.h"
#include "Actor.h"
#include "CollisionManager.h"
#include "Telegram.h"
//-----------------------------------------
// �_���[�W��^����
//-----------------------------------------
bool Charactor::ApplyDamage(int damage, float invincible_time)
{
	// �_���[�W��0�̏ꍇ�A���N��Ԃ�ύX����
	if (damage == 0) return false;

	// ���S���Ă���ꍇ�͌��N��Ԃ�ύX���Ȃ�
	if (health <= 0) return false;

	// ���G���Ԓ��̓_���[�W��^���Ȃ�
	if (invincible_timer > 0.0f) return false;

	// ���G���Ԑݒ�
	invincible_timer = invincible_time;

	// �_���[�W����
	health -= damage;

	// ���S�ʒm
	if (health <= 0)
	{
		death_flag = true;
	}
	// �_���[�W�ʒm
	else
	{
		damage_flag = true;
	}

	return true;
}

void Charactor::Start()
{
}

void Charactor::Update(float elapsed_time)
{
	// ���G���ԍX�V����
	UpdateInvincibleTimer(elapsed_time);

	// �q�b�g�X�g�b�v�̍X�V����
	UpdateHitStop(elapsed_time);
}

void Charactor::OnGUI()
{
	ImGui::InputInt("Health", &health);
	ImGui::InputInt("ID", &id);
	ImGui::Checkbox("AttackFlag", &hit_attack_flag);
	ImGui::Checkbox("DamageFlag", &damage_flag);

}

//-----------------------------------------
// ���G���ԍX�V
//-----------------------------------------
void Charactor::UpdateInvincibleTimer(float elapsed_time)
{
	// ���G���Ԃ�����Ό��������Ă���
	if (invincible_timer > 0.0f)
	{
		invincible_timer -= elapsed_time;
	}
}

//-----------------------------------------
// �A�j���[�V�����͈̔͋�Ԕ���
//-----------------------------------------
bool Charactor::SearchAnimationTime(std::shared_ptr<Actor> actor, float start_time, float end_time)
{
	Model* model = actor->GetModel();
	// �A�j���[�V�������Ԃ����������ׂ�
	float animation_time = model->GetCurrentAnimationSeconds();
	return animation_time >= start_time && animation_time <= end_time;
}

//-----------------------------------------
// �q�b�g�X�g�b�v�̍X�V����
//-----------------------------------------
void Charactor::UpdateHitStop(float elapsed_time)
{
	// �q�b�g�X�g�b�v�t���O�������Ă��Ȃ���Ώ����͂��Ȃ�
	if (!hitstop_flag) return;

	// �q�b�g�X�g�b�v�t���[����0�ȉ��Ȃ�t���O��false�ɂ���
	if (hitstop_frame <= 0.0f)
	{
		// �t���O��OFF��
		hitstop_flag = false;
		// 0.0f�ɏ�����
		hitstop_frame = 0.0f;
		return;
	}
	// �q�b�g�X�g�b�v�t���[���̃f�N�������g
	hitstop_frame--;
}
