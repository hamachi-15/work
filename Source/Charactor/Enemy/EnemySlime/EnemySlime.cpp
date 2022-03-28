#include "Model.h"
// �`��
#include "Graphics.h"
// ���b�Z�[�W
#include "Telegram.h"
// �R���|�[�l���g
#include "EnemySlime.h"
#include "Charactor.h"

// �}�l�[�W���[
#include "SceneManager.h"
#include "ActorManager.h"
#include "EnemyManager.h"
#include "EnemyTerritoryManager.h"
#include "CollisionManager.h"

// AI
#include "NodeBase.h"
#include "BehaviorTree.h"
#include "BehaviorData.h"
#include "JudgmentOwner.h"
#include "SlimeJudgmentOwner.h"
#include "ActionOwner.h"
#include "SlimeActionOwner.h"

// �R���W����
#include "EnemyCollision.h"

//-----------------------------------------
// �R���X�g���N�^
//-----------------------------------------
EnemySlime::EnemySlime()
{
}

//-----------------------------------------
// �f�X�g���N�^
//-----------------------------------------
EnemySlime::~EnemySlime()
{
}

//-----------------------------------------
// GUI�`��
//-----------------------------------------
void EnemySlime::OnGUI()
{
	// �r�w�C�r�A�֘A���
	DrawBehaviorGUI();
}

//-----------------------------------------
// �J�n����
//-----------------------------------------
void EnemySlime::Start()
{
	// �A�N�^�[�̎擾
	std::shared_ptr<Actor> actor = GetActor();

	// �L�����N�^�[�R���|�[�l���g�擾
	std::shared_ptr<Charactor> charactor = actor->GetComponent<Charactor>();

	// ���O�ݒ�
	SetName("Slime");

	// ���[�u�����g�R���|�[�l���g�̐ݒ�
	SetMovement(actor->GetComponent<Movement>());

	// �L�����N�^�[�R���|�[�l���g�̐ݒ�
	SetCharactor(charactor);

	// �}�l�[�W���[�ɓo�^
	EnemyManager::Instance().EnemyRegister(actor->GetComponent<EnemySlime>());
	
	// �ŏ��̓^�[�Q�b�g���W�����g�̍��W�ɐݒ�
	SetTargetPosition(actor->GetPosition());

	// �r�w�C�r�A�c���[�ݒ�
	behavior_data = new BehaviorData();
	ai_tree = new BehaviorTree();

	// �m�[�h�ݒ�
	SetBehaviorNode();
}

//-----------------------------------------
// �r�w�C�r�A�̃m�[�h�ݒ菈��
//-----------------------------------------
void EnemySlime::SetBehaviorNode()
{
	// �m�[�h�ݒ�
	ai_tree->AddNode("",	   "Root",	  0, BehaviorTree::SelectRule::Priority, NULL,					   NULL);
	ai_tree->AddNode("Root",   "Scount",  5, BehaviorTree::SelectRule::Priority, NULL,					   NULL);
	ai_tree->AddNode("Scount", "Wander",  1, BehaviorTree::SelectRule::Non,		 new WanderJudgment(this), new WanderAction(this));
	ai_tree->AddNode("Scount", "Idle",	  2, BehaviorTree::SelectRule::Non,		 NULL,					   new IdleAction(this));
}

//-----------------------------------------
// �X�V����
//-----------------------------------------
void EnemySlime::Update(float elapsed_time)
{
	// �r�w�C�r�A�X�V����
	BehaviorUpdate(elapsed_time);
}

//-----------------------------------------
// �G�̔j��
//-----------------------------------------
void EnemySlime::Destroy()
{
	// �A�N�^�[�̎擾
	std::shared_ptr<Actor> actor = GetActor();
		
	// �G�}�l�[�W���[����폜
	EnemyManager::Instance().EnemyRemove(actor->GetComponent<EnemySlime>());
	
	// �A�N�^�[�}�l�[�W���[����폜
	ActorManager::Instance().Remove(actor);
}

//-----------------------------------------
// ���b�Z�[�W����M�����Ƃ��̏���
//-----------------------------------------
bool EnemySlime::OnMessages(const Telegram& message)
{
	switch (message.message_box.message)
	{
	case MessageType::Message_Hit_Attack:
		break;
	case MessageType::Message_GetHit_Attack:
		// �Փ˂����ʒu��ݒ�
		SetHitPosition(message.message_box.hit_position);
		break;
	case MessageType::Message_Give_Attack_Right:
	{
		std::shared_ptr<Charactor> charactor = GetActor()->GetComponent<Charactor>();
		// �U���q�b�g�t���O�𗧂Ă�
		charactor->SetHitAttackFlag(true);
		break;
	}
	case MessageType::Message_Hit_Boddy:
		break;
	}
	return false;
}