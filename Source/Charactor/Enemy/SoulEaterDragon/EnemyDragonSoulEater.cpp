
#include "EnemyDragonSoulEater.h"
// �R���|�[�l���g
#include "Charactor.h"
// �}�l�[�W���[
#include "SceneManager.h"
#include "ActorManager.h"
#include "EnemyManager.h"
#include "CollisionManager.h"
#include "EnemyTerritoryManager.h"
#include "AudioManager.h"
// AI
#include "BehaviorTree.h"
#include "BehaviorData.h"
#include "ActionOwner.h"
#include "DragonActionOwner.h"
#include "JudgmentOwner.h"
#include "DragonJudgmentOwner.h"
// �`��
#include "Graphics.h"
// �e���g���[�֘A
#include "EnemyTerritory.h"
//*********************************
// 
// �\�E���C�[�^�[�h���S���N���X
// 
//*********************************
//------------------------------------
// �R���X�g���N�^
//------------------------------------
EnemyDragonSoulEater::EnemyDragonSoulEater()
{
}

//------------------------------------
// �f�X�g���N�^
//------------------------------------
EnemyDragonSoulEater::~EnemyDragonSoulEater()
{
}

//------------------------------------
// GUI�`��
//------------------------------------
void EnemyDragonSoulEater::OnGUI()
{
	// �r�w�C�r�A�֘A���
	DrawBehaviorGUI();
}

//------------------------------------
// ���b�Z�[�W����M�����Ƃ��̏���
//------------------------------------
bool EnemyDragonSoulEater::OnMessages(const Telegram& message)
{
	switch (message.message_box.message)
	{
	case MessageType::Message_Hit_Attack:
	{
		std::shared_ptr<Charactor> charactor = GetActor()->GetComponent<Charactor>();
		// ���g�̍U���q�b�g�t���O�𗧂Ă�
		charactor->SetHitAttackFlag(true);
		// �q�b�gSE�Đ�
		AudioManager::Instance().PlaySoundEffect(SEType::PlayerGetHit);
	}
	break;
	case MessageType::Message_GetHit_Attack:
		// �Փ˂����ʒu��ݒ�
		SetHitPosition(message.message_box.hit_position);
		break;
	case MessageType::Message_Give_Attack_Right:
		// �U���q�b�g�t���O�𗧂Ă�
		SetRightOfAttack(true);
	break;
	case MessageType::Message_Hit_Boddy:
		break;
	}
	return false;
}

//------------------------------------
// �J�n����
//------------------------------------
void EnemyDragonSoulEater::Start()
{
	// �A�N�^�[�̎擾
	std::shared_ptr<Actor> actor = GetActor();

	// �L�����N�^�[�R���|�[�l���g�擾
	std::shared_ptr<Charactor> charactor = actor->GetComponent<Charactor>();

	// ���O�ݒ�
	SetName("SoulEaterDragon");

	// ���G�͈͂̐ݒ�
	SetSearchRange(50.0f);

	// �U���͈͂̐ݒ�
	SetAttackRange(30.0f);

	// ���[�u�����g�R���|�[�l���g�̐ݒ�
	SetMovement(actor->GetComponent<Movement>());

	// �L�����N�^�[�R���|�[�l���g�̐ݒ�
	SetCharactor(charactor);

	// �}�l�[�W���[�ɓo�^
	EnemyManager::Instance().EnemyRegister(actor->GetComponent<EnemyDragonSoulEater>());

	// �r�w�C�r�A�c���[������
	behavior_data = new BehaviorData();
	ai_tree = new BehaviorTree();

	// �r�w�C�r�A�c���[�̃m�[�h�ݒ�
	SetBehaviorNode();
}

//------------------------------------
// �r�w�C�r�A�̃m�[�h�ݒ菈��
//------------------------------------
void EnemyDragonSoulEater::SetBehaviorNode()
{
	// ���݂̃V�[�����擾
	std::string scene_name = SceneManager::Instance().GetCurrentScene()->GetName();

	// �V�[�������[���h�}�b�v���̃m�[�h�ݒ�
	if (scene_name == "SceneBattle")
	{	// �V�[�����o�g���V�[���̎��̃m�[�h�ݒ�
		ai_tree->AddNode("", "Root", 0, BehaviorTree::SelectRule::Priority, NULL, NULL);
		ai_tree->AddNode("Root", "Death", 1, BehaviorTree::SelectRule::Non, new DeathJudgment(this), new DeathAction(this));
		ai_tree->AddNode("Root", "Battle", 3, BehaviorTree::SelectRule::Priority, NULL/*new BattleJudgment(this)*/, NULL);
		ai_tree->AddNode("Root", "Scount", 4, BehaviorTree::SelectRule::Priority, NULL, NULL);
		ai_tree->AddNode("Battle", "Attack", 1, BehaviorTree::SelectRule::On_Off_Ramdom, new AttackJudgment(this), NULL);
		ai_tree->AddNode("Battle", "OutRange", 2, BehaviorTree::SelectRule::On_Off_Ramdom, NULL, NULL);
		ai_tree->AddNode("Scount", "Idle", 1, BehaviorTree::SelectRule::Non, NULL, new IdleAction(this));
		ai_tree->AddNode("OutRange", "Pursuit", 0, BehaviorTree::SelectRule::Non, NULL, new PursuitAction(this));
		ai_tree->AddNode("OutRange", "FireBallShoot", 0, BehaviorTree::SelectRule::Sequence,NULL, NULL);
		ai_tree->AddNode("Attack", "BasicAttack", 0, BehaviorTree::SelectRule::Sequence, new BasicAttackJudgment(this), NULL);
		ai_tree->AddNode("Attack", "TailAttack", 0, BehaviorTree::SelectRule::Non, NULL, new TailAttackAction(this));
		ai_tree->AddNode("Attack", "FireBallShoot", 0, BehaviorTree::SelectRule::Sequence, NULL, NULL);

		ai_tree->AddNode("FireBallShoot", "TurnSequence", 0, BehaviorTree::SelectRule::Non, NULL, new TurnToTargetAction(this));
		ai_tree->AddNode("FireBallShoot", "FireBallShootSequence", 0, BehaviorTree::SelectRule::Non, NULL, new FireBollAttackAction(this));
		
		ai_tree->AddNode("BasicAttack", "TurnSequence", 0, BehaviorTree::SelectRule::Non, NULL, new TurnToTargetAction(this));
		ai_tree->AddNode("BasicAttack", "BasicAttackSequence", 0, BehaviorTree::SelectRule::Non, NULL, new BasicAttackAction(this));
	}
	else
	{
		ai_tree->AddNode("", "Root", 0, BehaviorTree::SelectRule::Priority, NULL, NULL);
		ai_tree->AddNode("Root", "Sleep", 1, BehaviorTree::SelectRule::Non, NULL, new SleepAction(this));
	}
}

//------------------------------------
// �G�̔j������
//------------------------------------
void EnemyDragonSoulEater::Destroy()
{
	// �A�N�^�[�̎擾
	std::shared_ptr<Actor> actor = GetActor();

	// �G�}�l�[�W���[����폜
	EnemyManager::Instance().EnemyRemove(GetActor()->GetComponent<EnemyDragonSoulEater>());

	// �A�N�^�[�}�l�[�W���[����폜
	ActorManager::Instance().Remove(GetActor());
}


//------------------------------------
// �X�V����
//------------------------------------
void EnemyDragonSoulEater::Update(float elapsed_time)
{
	// �r�w�C�r�A�X�V����
	BehaviorUpdate(elapsed_time);	
}