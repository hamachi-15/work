#include "EnemyDragonNightmare.h"
#include "Graphics.h"
#include "SceneManager.h"

// ���b�Z�[�W�֘A�C���N���[�h
#include "Telegram.h"

#include "Model.h"

// �A�N�^�[�֘A�C���N���[�h
#include "Charactor.h"

// �}�l�[�W���[�C���N���[�h
#include "ActorManager.h"
#include "EnemyManager.h"
#include "EnemyTerritoryManager.h"
#include "CollisionManager.h"
#include "AudioManager.h"

#include "EnemyCollision.h"

// AI�֘A�C���N���[�h
#include "NodeBase.h"
#include "BehaviorTree.h"
#include "BehaviorData.h"
#include "JudgmentOwner.h"
#include "DragonJudgmentOwner.h"
#include "ActionOwner.h"
#include "DragonActionOwner.h"

//********************************
// 
// �i�C�g���A�h���S���N���X
// 
//********************************
//--------------------------------------
// �R���X�g���N�^
//--------------------------------------
EnemyDragonNightmare::EnemyDragonNightmare()
{
}

//--------------------------------------
// �f�X�g���N�^
//--------------------------------------
EnemyDragonNightmare::~EnemyDragonNightmare()
{
}

//--------------------------------------
// GUI�`��
//--------------------------------------
void EnemyDragonNightmare::OnGUI()
{
	// �r�w�C�r�A�֘A���
	DrawBehaviorGUI();
}

//--------------------------------------
// ���b�Z�[�W����M�����Ƃ��̏���
//--------------------------------------
bool EnemyDragonNightmare::OnMessages(const Telegram& message)
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
	{
		// �U������^����
		SetRightOfAttack(true);
	}
	break;
	case MessageType::Message_Hit_Boddy:
		break;
	}
	return false;
}

//--------------------------------------
// �J�n����
//--------------------------------------
void EnemyDragonNightmare::Start()
{
	// �A�N�^�[�̎擾
	std::shared_ptr<Actor> actor = GetActor();

	// �L�����N�^�[�R���|�[�l���g�擾
	std::shared_ptr<Charactor> charactor = actor->GetComponent<Charactor>();

	// ���O�ݒ�
	SetName("NightmareDragon");

	// ���G�͈͂̐ݒ�
	SetSearchRange(80.0f);

	// �U���͈͂̐ݒ�
	SetAttackRange(60.0f);

	// ���[�u�����g�R���|�[�l���g�̐ݒ�
	SetMovement(actor->GetComponent<Movement>());

	// �L�����N�^�[�R���|�[�l���g�̐ݒ�
	SetCharactor(charactor);

	// �}�l�[�W���[�ɓo�^
	EnemyManager::Instance().EnemyRegister(actor->GetComponent<EnemyDragonNightmare>());


	// �r�w�C�r�A�c���[�ݒ�
	behavior_data = new BehaviorData();
	ai_tree = new BehaviorTree();

	// �m�[�h�ݒ�
	SetBehaviorNode();
}

//--------------------------------------
// �r�w�C�r�A�̃m�[�h�ݒ菈��
//--------------------------------------
void EnemyDragonNightmare::SetBehaviorNode()
{
	// ���݂̃V�[�����擾
	std::string scene_name = SceneManager::Instance().GetCurrentScene()->GetName();

	// �V�[�������[���h�}�b�v���̃m�[�h�ݒ�
	if (scene_name == "SceneBattle") 
	{	// �V�[�����o�g���V�[���̎��̃m�[�h�ݒ�
		ai_tree->AddNode("",					"Root",						0, BehaviorTree::SelectRule::Priority,	NULL,								NULL);
		ai_tree->AddNode("Root",				"Death",					1, BehaviorTree::SelectRule::Non,		new DeathJudgment(this),			new DeathAction(this));
		//ai_tree->AddNode("Root",				"Damage",					2, BehaviorTree::SelectRule::Non,		new DamageJudgment(this),			new DamageAction(this));
		ai_tree->AddNode("Root",				"Battle",					3, BehaviorTree::SelectRule::Priority,	NULL/*new BattleJudgment(this)*/,	NULL);
		ai_tree->AddNode("Root",				"Scount",					4, BehaviorTree::SelectRule::Priority,	NULL,								NULL);
		ai_tree->AddNode("Battle",				"Attack",					1, BehaviorTree::SelectRule::On_Off_Ramdom,	new AttackJudgment(this),			NULL);
		ai_tree->AddNode("Battle",				"OutRange",					2, BehaviorTree::SelectRule::Priority,		NULL,								NULL);
		ai_tree->AddNode("Scount",				"Idle",						1, BehaviorTree::SelectRule::Non,		NULL,								new IdleAction(this));
		ai_tree->AddNode("OutRange",			"Pursuit",					0, BehaviorTree::SelectRule::Non,		NULL, new PursuitAction(this));
		//ai_tree->AddNode("OutRange",			"LungesAttack",				1, BehaviorTree::SelectRule::Sequence,	NULL, NULL);
		//ai_tree->AddNode("LungesAttack",		"TurnSequence",				0, BehaviorTree::SelectRule::Non,		NULL, new TurnToTargetAction(this));
		//ai_tree->AddNode("LungesAttack",		"LungesAttackSequence",		1, BehaviorTree::SelectRule::Non,		NULL, new LungesAttackAction(this));
		ai_tree->AddNode("Attack",				"BasicAttack",				0, BehaviorTree::SelectRule::Sequence,	new BasicAttackJudgment(this),		NULL);
		ai_tree->AddNode("Attack",				"BodyPressAttack",			0, BehaviorTree::SelectRule::Random,	new BodyPressAttackJudgment(this), NULL);
		ai_tree->AddNode("Attack",				"PlayerToTurn",				0, BehaviorTree::SelectRule::Non,		new TurnToTargetJudgment(this),	new TurnToTargetAction(this));
		ai_tree->AddNode("Attack",				"ClawAttack",				0, BehaviorTree::SelectRule::Sequence,	new ClawAttackJudgment(this),		NULL);
		ai_tree->AddNode("Attack",				"HornAttack",				0, BehaviorTree::SelectRule::Non,		new ClawAttackJudgment(this),		NULL);
		ai_tree->AddNode("BasicAttack",			"TurnSequence",				0, BehaviorTree::SelectRule::Non,		NULL,								new TurnToTargetAction(this));
		ai_tree->AddNode("BasicAttack",			"BasicAttackSequence",		0, BehaviorTree::SelectRule::Non,		NULL,								new BasicAttackAction(this));
		ai_tree->AddNode("ClawAttack",			"TurnSequence",				0, BehaviorTree::SelectRule::Non,		NULL,								new TurnToTargetAction(this));
		ai_tree->AddNode("ClawAttack",			"BasicAttackSequence",		0, BehaviorTree::SelectRule::Non,		NULL,								new ClawAttackAction(this));
		ai_tree->AddNode("HornAttack",			"TurnSequence",				0, BehaviorTree::SelectRule::Non,		NULL,								new TurnToTargetAction(this));
		ai_tree->AddNode("HornAttack",			"HornAttackSequence",		0, BehaviorTree::SelectRule::Non,		NULL,								new HornAttackAction(this));
		ai_tree->AddNode("BodyPressAttack",		"OnceBodyPressAttack",		1, BehaviorTree::SelectRule::Non,		NULL,								new BodyPressAttackAction(this));
		ai_tree->AddNode("BodyPressAttack",		"DoubleBodyPressAttack",	1, BehaviorTree::SelectRule::Sequence,	NULL,								NULL);
		// 2�A���{�f�B�v���X
		ai_tree->AddNode("DoubleBodyPressAttack", "FarstBodyPressAttack",		0, BehaviorTree::SelectRule::Non, NULL,									new BodyPressAttackAction(this));
		ai_tree->AddNode("DoubleBodyPressAttack", "SecondBodyPressAttack",		0, BehaviorTree::SelectRule::Non, NULL,									new BodyPressAttackAction(this));
		ai_tree->AddNode("DoubleBodyPressAttack", "Scream",						0, BehaviorTree::SelectRule::Non, NULL,									new ScreamAction(this));
	}
	else
	{
		ai_tree->AddNode("",	 "Root",   0, BehaviorTree::SelectRule::Priority,	NULL, NULL);
		ai_tree->AddNode("Root", "Sleep",  1, BehaviorTree::SelectRule::Non,		NULL, new SleepAction(this));
	}
}

//--------------------------------------
// �G�̔j������
//--------------------------------------
void EnemyDragonNightmare::Destroy()
{
	// �A�N�^�[�̎擾
	std::shared_ptr<Actor> actor = GetActor();

	// �G�}�l�[�W���[����폜
	EnemyManager::Instance().EnemyRemove(GetActor()->GetComponent<EnemyDragonNightmare>());

	// �A�N�^�[�}�l�[�W���[����폜
	ActorManager::Instance().Remove(GetActor());
}

//--------------------------------------
// �X�V����
//--------------------------------------
void EnemyDragonNightmare::Update(float elapsed_time)
{
	// �r�w�C�r�A�X�V����
	BehaviorUpdate(elapsed_time);
}

//--------------------------------------
// ������͈̓f�o�b�O�v���~�e�B�u�`��
//--------------------------------------
void EnemyDragonNightmare::DrawDebugPrimitive()
{
	DebugRenderer* renderer = Graphics::Instance().GetDebugRenderer();
	std::shared_ptr<Actor> actor = GetActor();
	DirectX::XMFLOAT3 position = actor->GetPosition();
	EnemyTerritoryTag teritory_tag = GetBelongingToTerritory();
	std::shared_ptr<EnemyTerritory> enemy_territory = EnemyTerritoryManager::Instance().GetTerritory(teritory_tag);
	float territory_range = enemy_territory->GetTerritoryRange();
	DirectX::XMFLOAT3 territory_origin = enemy_territory->GetTerritoryOrigin();
	territory_origin.y = actor->GetPosition().y;
	// �꒣��͈͂��f�o�b�O�~���`��
	renderer->DrawCylinder(territory_origin, territory_range, 1.0f, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

	// ���G�͈͂��f�o�b�O�~���`��
	renderer->DrawCylinder(position, search_range, 1.0f, DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));

	// �U���͈͂��f�o�b�O�~���`��
	renderer->DrawCylinder(position, GetAttackRange(), 1.0f, DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f));

	// �^�[�Q�b�g���W�̋��`��
	renderer->DrawSphere(target_position, 0.5f, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
}


