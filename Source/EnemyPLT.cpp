// ���f��
#include "Model.h"

// �`��
#include "Graphics.h"
#include "DebugRenderer.h"

// AI
#include "NodeBase.h"
#include "BehaviorData.h"
#include "BehaviorTree.h"
#include "JudgmentOwner.h"
#include "PLTJudgmentOwner.h"
#include "ActionOwner.h"
#include "PLTActionOwner.h"

// �R���|�[�l���g
#include "Charactor.h"
#include "EnemyPLT.h"

// �}�l�[�W���[
#include "SceneManager.h"
#include "ActorManager.h"
#include "EnemyManager.h"
#include "EnemyTerritoryManager.h"
#include "CollisionManager.h"

// �R���W����
#include "EnemyCollision.h"

//**********************************
// 
// �v���b�g�N���X
// 
//**********************************
//-----------------------------------------
// �R���X�g���N�^
//-----------------------------------------
EnemyPLT::EnemyPLT()
{
}

//-----------------------------------------
// �f�X�g���N�^
//-----------------------------------------
EnemyPLT::~EnemyPLT()
{
}

//-----------------------------------------
// �j������
//-----------------------------------------
void EnemyPLT::Destroy()
{
	// �A�N�^�[�̎擾
	std::shared_ptr<Actor> actor = GetActor();

	// �G�}�l�[�W���[����폜
	EnemyManager::Instance().EnemyRemove(GetActor()->GetComponent<EnemyPLT>());

	// �A�N�^�[�}�l�[�W���[����폜
	ActorManager::Instance().Remove(GetActor());
}

//-----------------------------------------
// GUI�`��
//-----------------------------------------
void EnemyPLT::OnGUI()
{
	// �r�w�C�r�A�֘A���
	DrawBehaviorGUI();
}

//-----------------------------------------
// ���b�Z�[�W����M�����Ƃ��̏���
//-----------------------------------------
bool EnemyPLT::OnMessages(const Telegram& message)
{
	switch (message.message_box.message)
	{
	case MessageType::Message_Hit_Attack:

		break;
	case MessageType::Message_GetHit_Attack:
		// �Փ˂����ʒu��ݒ�
		SetHitPosition(message.message_box.hit_position);
		break;
	case MessageType::Message_Hit_Boddy:
		break;
	case MessageType::Message_Give_Attack_Right:
	{
		std::shared_ptr<Charactor> charactor = GetActor()->GetComponent<Charactor>();
		// �U���q�b�g�t���O�𗧂Ă�
		charactor->SetHitAttackFlag(true);
	}
	break;
	}
	return false;
}

//-----------------------------------------
// �J�n����
//-----------------------------------------
void EnemyPLT::Start()
{
	// �A�N�^�[�̎擾
	std::shared_ptr<Actor> actor = GetActor();

	// ���O�ݒ�
	SetName("PLT");

	// ���[�u�����g�R���|�[�l���g�̐ݒ�
	SetMovement(actor->GetComponent<Movement>());

	// �L�����N�^�[�R���|�[�l���g�̐ݒ�
	SetCharactor(actor->GetComponent<Charactor>());

	// �}�l�[�W���[�ɓo�^
	EnemyManager::Instance().EnemyRegister(actor->GetComponent<EnemyPLT>());

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
void EnemyPLT::SetBehaviorNode()
{
	// �m�[�h�ݒ�
	ai_tree->AddNode("", "Root", 0, BehaviorTree::SelectRule::Priority, NULL, NULL);
	ai_tree->AddNode("Root", "Scount", 2, BehaviorTree::SelectRule::Priority, NULL, NULL);
	ai_tree->AddNode("Scount", "Wander", 1, BehaviorTree::SelectRule::Non, new WanderJudgment(this), new WanderAction(this));
	ai_tree->AddNode("Scount", "Idle", 2, BehaviorTree::SelectRule::Non, NULL, new IdleAction(this));

}

//-----------------------------------------
// �X�V����
//-----------------------------------------
void EnemyPLT::Update(float elapsed_time)
{
	// �r�w�C�r�A�X�V����
	BehaviorUpdate(elapsed_time);
}