#include "DragonJudgmentOwner.h"
// ����̔ėp�֐����܂Ƃ߂����
#include "JudgmentUniversal.h"
// �A�N�^�[�֌W
#include "Charactor.h"
#include "ActorManager.h"
#include "Enemy.h"
#include "Telegram.h"
// AI�n
#include "MetaAI.h"
// �Z�o�n
//#include "Mathf.h"
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
	
	// �v���C���[�ƃh���S���̃A�N�^�[���擾
	std::shared_ptr<Actor> player = ActorManager::Instance().GetActor("Player");
	std::shared_ptr<Actor> enemy = owner->GetActor();

	// �G�ƃv���C���[�̍��W�擾
	DirectX::XMFLOAT3 player_position = player->GetPosition();
	DirectX::XMFLOAT3 enemy_position = enemy->GetPosition();
	DirectX::XMFLOAT3 enemy_angle = enemy->GetAngle();

	// �v���C���[���h���S���̑O���ɂ��邩����
	return JudgmentUniversal::JudgementTargetInFront(enemy_position, enemy_angle, player_position);
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

	// �v���C���[�ƃh���S���̃A�N�^�[���擾
	std::shared_ptr<Actor> player = ActorManager::Instance().GetActor("Player");
	std::shared_ptr<Actor> enemy = owner->GetActor();

	// �G�ƃv���C���[�̍��W�擾
	DirectX::XMFLOAT3 player_position = player->GetPosition();
	DirectX::XMFLOAT3 enemy_position = enemy->GetPosition();
	DirectX::XMFLOAT3 enemy_angle = enemy->GetAngle();

	// �v���C���[���h���S���̑O���ɂ��邩����
	return JudgmentUniversal::JudgementTargetInFront(enemy_position, enemy_angle, player_position);
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

	// �v���C���[�ƃh���S���̃A�N�^�[���擾
	std::shared_ptr<Actor> player = ActorManager::Instance().GetActor("Player");
	std::shared_ptr<Actor> enemy = owner->GetActor();

	// �G�ƃv���C���[�̍��W�擾
	DirectX::XMFLOAT3 player_position = player->GetPosition();
	DirectX::XMFLOAT3 enemy_position = enemy->GetPosition();
	DirectX::XMFLOAT3 enemy_angle = enemy->GetAngle();

	// �v���C���[���h���S���̑O���ɂ��邩����
	return JudgmentUniversal::JudgementTargetInFront(enemy->GetPosition(), enemy->GetAngle(), player->GetPosition());
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
	//if (!owner->GetAngerFlag()) return false;

	// �v���C���[�ƃh���S���̃A�N�^�[���擾
	std::shared_ptr<Actor> player = ActorManager::Instance().GetActor("Player");
	std::shared_ptr<Actor> enemy = owner->GetActor();

	// �G�ƃv���C���[�̍��W�擾
	DirectX::XMFLOAT3 player_position = player->GetPosition();
	DirectX::XMFLOAT3 enemy_position = enemy->GetPosition();
	DirectX::XMFLOAT3 enemy_angle = enemy->GetAngle();
	
	// �v���C���[���h���S���̑O���ɂ��邩����
	return JudgmentUniversal::JudgementTargetInFront(enemy_position, enemy_angle, player_position);
}

//*****************************************
// 
// �����킹�m�[�h�Ɉڍs�ł��邩����
// 
//*****************************************
//-----------------------------------------
//����
//-----------------------------------------
bool AligningJudgment::Judgment()
{
	// �v���C���[�ƃh���S���̃A�N�^�[���擾
	std::shared_ptr<Actor> player = ActorManager::Instance().GetActor("Player");
	std::shared_ptr<Actor> enemy = owner->GetActor();

	// �v���C���[���h���S���̑O���ɂ��邩����
	if (!JudgmentUniversal::JudgementTargetInFront(enemy->GetPosition(), enemy->GetAngle(), player->GetPosition()))
	{
		// �O���ɂ��Ȃ����true
		return true;
	}

	// �O���ɂ����false
	return false;
}
