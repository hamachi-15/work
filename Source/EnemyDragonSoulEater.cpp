#include "SceneManager.h"

#include "Charactor.h"
#include "EnemyManager.h"
#include "EnemyDragonSoulEater.h"

#include "BehaviorTree.h"
#include "BehaviorData.h"
#include "ActionOwner.h"
#include "JudgmentOwner.h"
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

	// ���[�u�����g�R���|�[�l���g�̐ݒ�
	SetMovement(actor->GetComponent<Movement>());

	// �L�����N�^�[�R���|�[�l���g�̐ݒ�
	SetCharactor(charactor);

	// �}�l�[�W���[�ɓo�^
	EnemyManager::Instance().EnemyRegister(actor->GetComponent<EnemyDragonSoulEater>());
	
	// �R���W�����̐ݒ�
	{
		Model* model = GetActor()->GetModel();
		// �̂̃R���W�����ݒ�
		CollisionParameter parameter;
		parameter.name = actor->GetName();
		parameter.node_name = "";
		parameter.actor_id = charactor->GetID() + GetIdentity();
		parameter.position = { 0, 0, 0 };
		parameter.radius = 20.5f;
		parameter.height = 26.5f;
		parameter.weight = 6.5f;
		parameter.collision_flg = true;
		parameter.actor_type = CollisionActorType::Enemy;
		parameter.element = CollisionElement::Body;
		parameter.position_mask = CollisionPositionMask::Collision_Mask_Actor_Position;
		charactor->SetCollision(actor, parameter, CollisionMeshType::Cylinder);

		// �E�r�R���W����
		std::string name = parameter.name;
		name += "RightWrist";
		parameter.name = name.c_str();
		parameter.node_name = "R_Wrist";
		parameter.radius = 4.0f;
		parameter.height = 0.0f;
		parameter.weight = 1.0f;
		parameter.collision_flg = false;
		parameter.actor_type = CollisionActorType::Enemy;
		parameter.element = CollisionElement::Weppon;
		parameter.position_mask = CollisionPositionMask::Collision_Mask_Member_Position;
		charactor->SetCollision(actor, parameter, CollisionMeshType::Sphere);

		// ���R���W����
		name.clear();
		name = actor->GetName();
		name += "Head";
		parameter.name = name.c_str();
		parameter.node_name = "UpperHead02";
		parameter.radius = 6.5f;
		parameter.height = 0.0f;
		parameter.weight = 1.0f;
		parameter.collision_flg = false;
		parameter.actor_type = CollisionActorType::Enemy;
		parameter.element = CollisionElement::Weppon;
		parameter.position_mask = CollisionPositionMask::Collision_Mask_Member_Position;
		charactor->SetCollision(actor, parameter, CollisionMeshType::Sphere);
	
		// ���R���W����
		name.clear();
		name = actor->GetName();
		name += "Tail";
		parameter.name = name.c_str();
		parameter.node_name = "TailEnd";
		parameter.radius = 3.5f;
		parameter.height = 0.0f;
		parameter.weight = 1.0f;
		parameter.collision_flg = false;
		parameter.actor_type = CollisionActorType::Enemy;
		parameter.element = CollisionElement::Weppon;
		parameter.position_mask = CollisionPositionMask::Collision_Mask_Member_Position;
		charactor->SetCollision(actor, parameter, CollisionMeshType::Sphere);
	}

	// �r�w�C�r�A�c���[�ݒ�
	behavior_data = new BehaviorData();
	ai_tree = new BehaviorTree();

	// �m�[�h�ݒ�
	SetBehaviorNode();
}

//------------------------------------
// �r�w�C�r�A�̃m�[�h�ݒ菈��
//------------------------------------
void EnemyDragonSoulEater::SetBehaviorNode()
{
	// ���݂̃V�[�����擾
	const char* name = SceneManager::Instance().GetCurrentScene()->GetName();

	// �V�[�������[���h�}�b�v���̃m�[�h�ݒ�
	if (strcmp(name, "SceneWorldMap") == 0)
	{
		ai_tree->AddNode("", "Root", 0, BehaviorTree::SelectRule::Priority, NULL, NULL);
		ai_tree->AddNode("Root", "Sleep", 1, BehaviorTree::SelectRule::Non, NULL, new SleepAction(this));

	} // �V�[�����o�g���V�[���̎��̃m�[�h�ݒ�
	else //if (strcmp(name, "SceneBattle") == 0)
	{
		ai_tree->AddNode("", "Root", 0, BehaviorTree::SelectRule::Priority, NULL, NULL);
		ai_tree->AddNode("Root", "Death", 1, BehaviorTree::SelectRule::Non, new DeathJudgment(this), new DeathAction(this));
		ai_tree->AddNode("Root", "Damage", 2, BehaviorTree::SelectRule::Non, new DamageJudgment(this), new DamageAction(this));
		ai_tree->AddNode("Root", "Battle", 3, BehaviorTree::SelectRule::Priority, new BattleJudgment(this), NULL);
		ai_tree->AddNode("Root", "Scount", 4, BehaviorTree::SelectRule::Priority, NULL, NULL);
		ai_tree->AddNode("Battle", "Attack", 1, BehaviorTree::SelectRule::Priority, new AttackJudgment(this), NULL);
		ai_tree->AddNode("Battle", "Pursuit", 2, BehaviorTree::SelectRule::Non, NULL, new PursuitAction(this));
		//ai_tree->AddNode("Attack", "BasicAttack", 1, BehaviorTree::SelectRule::Non, new BasicAttackJudgment(this), new BasicAttackAction(this));
		//ai_tree->AddNode("Attack", "ClawAttack", 2, BehaviorTree::SelectRule::Non, new ClawAttackJudgment(this), new ClawAttackAction(this));
		//ai_tree->AddNode("Attack", "HornAttack", 3, BehaviorTree::SelectRule::Non, new ClawAttackJudgment(this), new HornAttackAction(this));
		//ai_tree->AddNode("Attack", "JumpAttack", 4, BehaviorTree::SelectRule::Non, new JumpAttackJudgment(this), new JumpAttackAction(this));
		ai_tree->AddNode("Scount", "Wander", 1, BehaviorTree::SelectRule::Non, new WanderJudgment(this), new WanderAction(this));
		ai_tree->AddNode("Scount", "Idle", 2, BehaviorTree::SelectRule::Non, NULL, new IdleAction(this));
	}
}

//------------------------------------
// �G�̔j������
//------------------------------------
void EnemyDragonSoulEater::Destroy()
{
}

//------------------------------------
// �X�V����
//------------------------------------
void EnemyDragonSoulEater::Update(float elapsed_time)
{
}

//------------------------------------
// ������͈̓f�o�b�O�v���~�e�B�u�`��
//------------------------------------
void EnemyDragonSoulEater::DrawDebugPrimitive()
{
}
// ���b�Z�[�W����M�����Ƃ��̏���
bool EnemyDragonSoulEater::OnMessages(const Telegram& message)
{
	return false;
}
