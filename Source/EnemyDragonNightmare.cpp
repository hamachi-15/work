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
		break;
	case MessageType::Message_GetHit_Attack:
		//�_���[�W�t���O���I����
		OnDamaged();
		// �Փ˂����ʒu��ݒ�
		SetHitPosition(message.message_box.hit_position);
		break;
	case MessageType::Message_Give_Attack_Right:
		// �U���t���O���I����
		SetAttackFlag(true);
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

	// �R���W�����̐ݒ�
	{
		Model* model = GetActor()->GetModel();
		CollisionParameter parameter;
		// �J�����O�p�̃R���W����
		parameter.name = "NightmareDragonAABB";
		parameter.node_name = "Spine02";
		parameter.float3_radius = DirectX::XMFLOAT3(40.5f, 25.5f, 60.5f);
		parameter.collision_flg = true;
		parameter.actor_id = charactor->GetID();
		parameter.element = CollisionElement::Body;
		parameter.position_mask = CollisionPositionMask::Collision_Mask_Member_Position;
		charactor->SetCollision(actor, parameter, CollisionMeshType::AABB);

		// �E�r�R���W����
		std::string name = actor->GetName();
		name += "RightWrist";
		parameter.name = name.c_str();
		parameter.node_name = "R_Wrist";
		parameter.actor_id = charactor->GetID() + GetIdentity();
		parameter.radius = 4.0f;
		parameter.weight = 1.0f;
		parameter.collision_flg = false;
		parameter.actor_type = CollisionActorType::Enemy;
		parameter.element = CollisionElement::Weppon;
		charactor->SetCollision(actor, parameter, CollisionMeshType::Sphere);

		// ���R���W����
		name.clear();
		name = actor->GetName();
 		name += "Head";
		parameter.name = name.c_str();
		parameter.node_name = "UpperHead02";
		parameter.radius = 6.5f;
		charactor->SetCollision(actor, parameter, CollisionMeshType::Sphere);
	
		// ���̃R���W����
		name.clear();
		name = actor->GetName();
		name += "mouth";
		parameter.name = name.c_str();
		parameter.node_name = "Jaw02";
		parameter.radius = 6.5f;
		charactor->SetCollision(actor, parameter, CollisionMeshType::Sphere);

		// �̂̃R���W�����ݒ�
		parameter.name = actor->GetName();
		parameter.node_name = "Root";
		parameter.radius = 20.5f;
		parameter.height = 34.5f;
		parameter.weight = 6.5f;
		parameter.local_position = DirectX::XMFLOAT3(0, 14, 0);
		parameter.collision_flg = true;
		parameter.element = CollisionElement::Body;
		parameter.position_mask = CollisionPositionMask::Collision_Mask_Castam_poition;
		charactor->SetCollision(actor, parameter, CollisionMeshType::Cylinder);
		// �̂̃R���W�����ݒ�
		name.clear();
		name = actor->GetName();
		name += "Body";
		parameter.name = name.c_str();
		parameter.node_name = "Root";
		parameter.radius = 20.5f;
		parameter.height = 34.5f;
		parameter.weight = 6.5f;
		parameter.local_position = DirectX::XMFLOAT3(0, 14, 0);
		parameter.collision_flg = false;
		parameter.element = CollisionElement::Weppon;
		parameter.position_mask = CollisionPositionMask::Collision_Mask_Castam_poition;
		charactor->SetCollision(actor, parameter, CollisionMeshType::Cylinder);

	}

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
		ai_tree->AddNode("",					  "Root",						0,	BehaviorTree::SelectRule::Priority,	 NULL,								NULL);
		ai_tree->AddNode("Root",				  "Death",						1,	BehaviorTree::SelectRule::Non,		 new DeathJudgment(this),			new DeathAction(this));
		ai_tree->AddNode("Root",				  "Damage",						2,	BehaviorTree::SelectRule::Non,		 new DamageJudgment(this),			new DamageAction(this));
		ai_tree->AddNode("Root",				  "Battle",						3,	BehaviorTree::SelectRule::Priority,	 new BattleJudgment(this),			NULL);
		ai_tree->AddNode("Root",				  "Scount",						4,	BehaviorTree::SelectRule::Priority,	 NULL,								NULL);
		ai_tree->AddNode("Battle",				  "Attack",						1,	BehaviorTree::SelectRule::On_Off_Ramdom,	 new AttackJudgment(this),			NULL);
		ai_tree->AddNode("Battle",				  "Pursuit",					2,	BehaviorTree::SelectRule::Non,		 NULL,								new PursuitAction(this));
		ai_tree->AddNode("Scount",				  "Idle",						1,	BehaviorTree::SelectRule::Non,		 NULL,								new IdleAction(this));
		ai_tree->AddNode("Scount",				  "Scream",						2, BehaviorTree::SelectRule::Non,		 NULL, new ScreamAction(this));
		ai_tree->AddNode("Attack",				  "BasicAttack",				1,	BehaviorTree::SelectRule::Non,		 new BasicAttackJudgment(this),		new BasicAttackAction(this));
		ai_tree->AddNode("Attack",				  "ClawAttack",					2,	BehaviorTree::SelectRule::Non,		 new ClawAttackJudgment(this),		new ClawAttackAction(this));
		ai_tree->AddNode("Attack",				  "HornAttack",					3,	BehaviorTree::SelectRule::Non,		 new ClawAttackJudgment(this),		new HornAttackAction(this));
		ai_tree->AddNode("Attack",				  "BodyPressAttack",			4, BehaviorTree::SelectRule::Random, new BodyPressAttackJudgment(this), NULL);
		ai_tree->AddNode("Attack",				  "LungesAttack",				5,	BehaviorTree::SelectRule::Priority,		 NULL, NULL);
		ai_tree->AddNode("BodyPressAttack",		  "OnceBodyPressAttack",		1, BehaviorTree::SelectRule::Non, NULL, new BodyPressAttackAction(this));
		ai_tree->AddNode("BodyPressAttack",		  "DoubleBodyPressAttack",		1, BehaviorTree::SelectRule::Sequence, NULL, NULL);
		ai_tree->AddNode("LungesAttack",		  "TreeTimesLungesAttack", 1, BehaviorTree::SelectRule::Sequence, NULL, NULL);
		ai_tree->AddNode("LungesAttack",		  "SixTimesLungesAttack", 2, BehaviorTree::SelectRule::Sequence, NULL, NULL);
		// 2�A���{�f�B�v���X
		ai_tree->AddNode("DoubleBodyPressAttack", "FarstBodyPressAttack",		0, BehaviorTree::SelectRule::Non, NULL, new BodyPressAttackAction(this));
		ai_tree->AddNode("DoubleBodyPressAttack", "SecondBodyPressAttack",		0, BehaviorTree::SelectRule::Non, NULL, new BodyPressAttackAction(this));
		ai_tree->AddNode("DoubleBodyPressAttack", "Scream",						0, BehaviorTree::SelectRule::Non, NULL, new ScreamAction(this));
		// 3�A���ːi
		ai_tree->AddNode("TreeTimesLungesAttack", "MoveToTargetSequence",		0, BehaviorTree::SelectRule::Non, NULL, new MoveToTargetAction(this, lunges_target_position_data[0]));
		ai_tree->AddNode("TreeTimesLungesAttack", "FarstLungesAttackSequence",	0, BehaviorTree::SelectRule::Non, NULL, new LungesAttackAction(this ,lunges_target_position_data[1]));
		ai_tree->AddNode("TreeTimesLungesAttack", "SecondLungesAttackSequence",	0, BehaviorTree::SelectRule::Non, NULL, new LungesAttackAction(this, lunges_target_position_data[2]));
		ai_tree->AddNode("TreeTimesLungesAttack", "ThirdLungesAttackSequence",	0, BehaviorTree::SelectRule::Non, NULL, new LungesAttackAction(this, lunges_target_position_data[3]));
		// 5�A���ːi
		ai_tree->AddNode("SixTimesLungesAttack",  "MoveToTargetSequence",		0, BehaviorTree::SelectRule::Non, NULL, new MoveToTargetAction(this, lunges_target_position_data[0]));
		ai_tree->AddNode("SixTimesLungesAttack",  "FarstLungesAttackSequence",	0, BehaviorTree::SelectRule::Non, NULL, new LungesAttackAction(this, lunges_target_position_data[1]));
		ai_tree->AddNode("SixTimesLungesAttack",  "SecondLungesAttackSequence",	0, BehaviorTree::SelectRule::Non, NULL, new LungesAttackAction(this, lunges_target_position_data[2]));
		ai_tree->AddNode("SixTimesLungesAttack",  "ThirdLungesAttackSequence",	0, BehaviorTree::SelectRule::Non, NULL, new LungesAttackAction(this, lunges_target_position_data[3]));
		ai_tree->AddNode("SixTimesLungesAttack",  "FourthLungesAttackSequence",	0, BehaviorTree::SelectRule::Non, NULL, new LungesAttackAction(this, lunges_target_position_data[4]));
		ai_tree->AddNode("SixTimesLungesAttack",  "FifthLungesAttackSequence",	0, BehaviorTree::SelectRule::Non, NULL, new LungesAttackAction(this, lunges_target_position_data[0]));
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

	// �R���W�����폜
	// ���R���W�����폜
	std::vector<std::shared_ptr<CollisionSphere>> list = CollisionManager::Instance().GetCollisionSphereFromID(GetCharactor()->GetID() + GetIdentity());
	for (std::shared_ptr<CollisionSphere> sphere : list)
	{
		CollisionManager::Instance().UnregisterSphere(sphere);
	}

	// �~���R���W�����폜
	CollisionManager::Instance().UnregisterCylinder(CollisionManager::Instance().GetCollisionCylinderFromName(actor->GetName()));

	// �����̃R���W�����폜
	CollisionManager::Instance().UnregisterBox(CollisionManager::Instance().GetCollisionBoxFromName("NightmareDragonAABB"));

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

	// ���͍X�V����
	GetMovement()->UpdateVelocity(elapsed_time);

	// ���G���ԍX�V����
	GetCharactor()->UpdateInvincibleTimer(elapsed_time);
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


