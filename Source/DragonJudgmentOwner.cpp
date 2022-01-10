#include "DragonJudgmentOwner.h"
#include "ActorManager.h"
#include "Enemy.h"
#include "Charactor.h"
#include "Telegram.h"
#include "MetaAI.h"
#include "Mathf.h"
//*****************************************
// 
// �܍U���m�[�h�Ɉڍs�ł��邩����
// 
//*****************************************
// -----------------------------
// ����
// -----------------------------
bool ClawAttackJudgment::Judgment()
{
	// �{���ԂȂ�false��Ԃ�
	if (owner->GetAngerFlag()) return false;
	
	
	return true;
}

//*****************************************
// 
// ���݂��U���m�[�h�Ɉڍs�ł��邩����
// 
//*****************************************
// -----------------------------
// ����
// -----------------------------
bool BasicAttackJudgment::Judgment()
{
	// �{���ԂȂ�false��Ԃ�
	if (owner->GetAngerFlag()) return false;

	return false;
}

//*****************************************
// 
// �p�U���m�[�h�Ɉڍs�ł��邩����
// 
//*****************************************
// -----------------------------
// ����
// -----------------------------
bool HornAttackJudgment::Judgment()
{
	// �{���Ԃ���Ȃ����false��Ԃ�
	if (!owner->GetAngerFlag()) return false;
	return false;
}

//*****************************************
// 
// �{�f�B�v���X�U���m�[�h�Ɉڍs�ł��邩����
// 
//*****************************************
// -----------------------------
// ����
// -----------------------------
bool BodyPressAttackJudgment::Judgment()
{
	// �{���Ԃ���Ȃ����false��Ԃ�
	if (!owner->GetAngerFlag()) return false;

	std::shared_ptr<Actor> player = ActorManager::Instance().GetActor("Player");
	std::shared_ptr<Actor> enemy = owner->GetActor();

	// �G�ƃv���C���[�̍��W�擾
	DirectX::XMFLOAT3 player_position = player->GetPosition();
	DirectX::XMFLOAT3 enemy_position = enemy->GetPosition();

	// �G�ƃv���C���[�̃x�N�g���Z�o
	DirectX::XMVECTOR vector = Mathf::ReturnVectorSubtract(player_position, enemy_position);

	// �G�̊p�x����O�������Z�o
	DirectX::XMVECTOR front = Mathf::ReturnVectorFront(enemy->GetAngle());

	// �G�̑O�����ƃx�N�g��������ώZ�o
	float dot = Mathf::ReturnFloatDot(front, vector);

	// ���ς����̐��Ȃ�false��Ԃ�
	if (dot < 0) return false;

	return true;
}
