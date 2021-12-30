#include "Model.h"

#include "EnemyMutant.h"
#include "EnemyManager.h"
#include "Graphics.h"
#include "BehaviorTree.h"
#include "BehaviorData.h"
#include "NodeBase.h"
#include "JudgmentOwner.h"
#include "BossJudgmenetOwner.h"
#include "ActionOwner.h"
#include "BossActionOwner.h"
#include "Charactor.h"
//---------------------------------------
// �R���X�g���N�^
//---------------------------------------
EnemyMutant::EnemyMutant()
{
	// ���O�ݒ�
	SetName("Mutant");
}

//---------------------------------------
// �f�X�g���N�^
//---------------------------------------
EnemyMutant::~EnemyMutant()
{
}

//---------------------------------------
// GUI�`��
//---------------------------------------
void EnemyMutant::OnGUI()
{
	// �r�w�C�r�A�֘A���
	DrawBehaviorGUI();
}

//-----------------------------------------
// �G�̔j������
//-----------------------------------------
void EnemyMutant::Destroy()
{
}

//---------------------------------------
// �J�n����
//---------------------------------------
void EnemyMutant::Start()
{
	// �A�N�^�[�̎擾
	std::shared_ptr<Actor> actor = GetActor();
	std::shared_ptr<Charactor> charactor = actor->GetComponent<Charactor>();


	// ���[�u�����g�R���|�[�l���g�̐ݒ�
	SetMovement(actor->GetComponent<Movement>());

	// �L�����N�^�[�R���|�[�l���g�̐ݒ�
	SetCharactor(charactor);
	// �}�l�[�W���[�ɓo�^
	EnemyManager::Instance().EnemyRegister(actor->GetComponent<EnemyMutant>());

	// ���G�͈͂̐ݒ�
	SetSearchRange(40.0f);
	// �U���͈͂̐ݒ�
	SetAttackRange(5.0f);
	// �e���g���[�͈͂̐ݒ�
	SetTerritoryOrigin(actor->GetPosition());
	// �ŏ��̓^�[�Q�b�g���W�����g�̍��W�ɐݒ�
	SetTargetPosition(actor->GetPosition());

	{
		Model* model = actor->GetModel();
		CollisionParameter parameter;
		parameter.name = GetName();
		parameter.node_name = "";
		parameter.actor_id = charactor->GetID();
		parameter.position = { 0, 0, 0 };
		parameter.radius = 4.5f;
		parameter.height = 7.0f;
		parameter.weight = 6.5f;
		parameter.collision_flg = true;
		parameter.element = CollisionElement::Body;
		parameter.position_mask = CollisionPositionMask::Collision_Mask_Actor_Position;
		charactor->SetCollision(actor, parameter, CollisionMeshType::Cylinder);


		DirectX::XMFLOAT3 position;
		charactor->GetNodePosition("Mutant:RightHandIndex1", position, model);
		parameter.name = "MutantRightHand";
		parameter.node_name = "Mutant:RightHandIndex1";
		parameter.actor_id = charactor->GetID();
		parameter.position = position;
		parameter.radius = 3.0f;
		parameter.weight = 2.0f;
		parameter.collision_flg = true;
		parameter.element = CollisionElement::Weppon;
		parameter.position_mask = CollisionPositionMask::Collision_Mask_Member_Position;
		charactor->SetCollision(actor, parameter, CollisionMeshType::Sphere);

		// ����̃R���W����
		charactor->GetNodePosition("Mutant:LeftHand", position, model);
		parameter.name = "MutantLeftHand";
		parameter.node_name = "Mutant:LeftHand";
		parameter.position = position;
		parameter.collision_flg = true;
		charactor->SetCollision(actor, parameter, CollisionMeshType::Sphere);

	}

	// �r�w�C�r�A�c���[�ݒ�
	behavior_data = new BehaviorData();
	ai_tree = new BehaviorTree();

	ai_tree->AddNode("", "Root", 0, BehaviorTree::SelectRule::Priority, NULL, NULL);
	ai_tree->AddNode("Root", "Death", 1, BehaviorTree::SelectRule::Non, new DeathJudgment(this), new DeathAction(this));
	ai_tree->AddNode("Root", "Damage", 2, BehaviorTree::SelectRule::Non, new DamageJudgment(this), new DamageAction(this));
	ai_tree->AddNode("Root", "Escape", 3, BehaviorTree::SelectRule::Sequence, new EscapeJudgment(this), NULL);
	ai_tree->AddNode("Root", "Battle", 4, BehaviorTree::SelectRule::Priority, new BattleJudgment(this), NULL);
	ai_tree->AddNode("Root", "Scount", 5, BehaviorTree::SelectRule::Priority, NULL, NULL);
	//	ai_tree->AddNode("Escape", "Recover", 0, BehaviorTree::SelectRule::Non, NULL, new RecoverAction(this));
	ai_tree->AddNode("Battle", "Attack", 1, BehaviorTree::SelectRule::Random, new AttackJudgment(this), NULL);
	ai_tree->AddNode("Battle", "Pursuit", 2, BehaviorTree::SelectRule::Non, NULL, new PursuitAction(this));
	ai_tree->AddNode("Attack", "Punch", 0, BehaviorTree::SelectRule::Non, new PunchAttackJudgment(this), new PunchAttackAction(this));
	ai_tree->AddNode("Attack", "Swiping", 0, BehaviorTree::SelectRule::Non, new SwipingAttackJudgmenet(this), new SwipingAttackAction(this));
	//ai_tree->AddNode("Attack", "JumpAttack", 0, BehaviorTree::SelectRule::Non, new JumpAttackJudgmenet(this), new JumpAttackAction(this));
	ai_tree->AddNode("Scount", "Wander", 1, BehaviorTree::SelectRule::Non, new WanderJudgment(this), new WanderAction(this));
	ai_tree->AddNode("Scount", "Idle", 2, BehaviorTree::SelectRule::Non, NULL, new IdleAction(this));
}

//-----------------------------------------
// �r�w�C�r�A�̃m�[�h�ݒ菈��
//-----------------------------------------
void EnemyMutant::SetBehaviorNode()
{
}

//---------------------------------------
// �X�V����
//---------------------------------------
void EnemyMutant::Update(float elapsed_time)
{
	// �r�w�C�r�A�c���[�X�V����
	if (active_node == nullptr)
	{
		active_node = ai_tree->ActiveNodeInference(this, behavior_data);
	}
	if (active_node != nullptr && active_node != old_active_node)
	{
		ai_tree->Start(active_node);
	}
	if (active_node != nullptr)
	{
		active_node = ai_tree->Run(this, active_node, behavior_data, elapsed_time);
	}
	old_active_node = active_node;
	// ���͍X�V����
	GetMovement()->UpdateVelocity(elapsed_time);

	// ���G���ԍX�V����
	GetCharactor()->UpdateInvincibleTimer(elapsed_time);
}

void EnemyMutant::DrawDebugPrimitive()
{
	DebugRenderer* renderer = Graphics::Instance().GetDebugRenderer();
	std::shared_ptr<Actor> actor = GetActor();
	DirectX::XMFLOAT3 position = actor->GetPosition();
	float territory_range = GetTerritoryRange();
	DirectX::XMFLOAT3 territory_origin = GetTerritoryOrigin();
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