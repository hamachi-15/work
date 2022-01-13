#include "Model.h"

#include "Graphics.h"
#include "SceneManager.h"
#include "Telegram.h"

#include "ActorManager.h"
#include "Charactor.h"
#include "EnemySlime.h"
#include "EnemyManager.h"

#include "NodeBase.h"
#include "BehaviorTree.h"
#include "BehaviorData.h"
#include "JudgmentOwner.h"
#include "SlimeJudgmentOwner.h"
#include "ActionOwner.h"
#include "SlimeActionOwner.h"

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
	DirectX::XMFLOAT3 origin = GetTerritoryOrigin();
	ImGui::InputFloat3("territory_origin", &origin.x);
	bool attack_flag = GetAttackFlag();
	ImGui::Checkbox("AttackFlag", &attack_flag);

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
	
	// ���G�͈͂̐ݒ�
	SetSearchRange(20.0f);

	// �U���͈͂̐ݒ�
	SetAttackRange(10.0f);

	// �ŏ��̓^�[�Q�b�g���W�����g�̍��W�ɐݒ�
	SetTargetPosition(actor->GetPosition());

	// �R���W�����̐ݒ�
	{
		Model* model = GetActor()->GetModel();
		CollisionParameter parameter;

		// �J�����O�p�̃R���W����
		parameter.name = "SlimeAABB";
		parameter.node_name = "Spine01";
		parameter.float3_radius = DirectX::XMFLOAT3(5.5f, 5.5f, 5.5f);
		parameter.collision_flg = true;
		parameter.actor_id = charactor->GetID();
		parameter.actor_type = CollisionActorType::Enemy;
		parameter.element = CollisionElement::Body;
		parameter.position_mask = CollisionPositionMask::Collision_Mask_Member_Position;
		charactor->SetCollision(actor, parameter, CollisionMeshType::AABB);

		// �̂̃R���W�����ݒ�
		parameter.name = actor->GetName();
		parameter.node_name = "";
		parameter.actor_id = charactor->GetID() + GetIdentity();
		parameter.radius = 3.5f;
		parameter.height = 6.5f;
		parameter.weight = 6.5f;
		parameter.collision_flg = true;
		parameter.position_mask = CollisionPositionMask::Collision_Mask_Actor_Position;
		charactor->SetCollision(actor, parameter, CollisionMeshType::Cylinder);

		// ���˂��̃R���W�����ݒ�
		charactor->GetNodePosition("BottomEyeCover", head_position, model);
		haed_collision_name = actor->GetName();
		haed_collision_name += "Head";
		parameter.name = haed_collision_name.c_str();
		parameter.node_name = "BottomEyeCover";
		parameter.position = head_position;
		parameter.radius = 4.0f;
		parameter.weight = 1.0f;
		parameter.height = 0.0f;
		parameter.collision_flg = false;
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

//-----------------------------------------
// �r�w�C�r�A�̃m�[�h�ݒ菈��
//-----------------------------------------
void EnemySlime::SetBehaviorNode()
{
	// ���݂̃V�[�����擾
	const char* name = SceneManager::Instance().GetCurrentScene()->GetName();

	// �V�[�������[���h�}�b�v���̃m�[�h�ݒ�
	if (strcmp(name, "SceneWorldMap") == 0)
	{
		ai_tree->AddNode("",	   "Root",	  0, BehaviorTree::SelectRule::Priority, NULL,					   NULL);
		ai_tree->AddNode("Root",   "Battle",  4, BehaviorTree::SelectRule::Priority, new BattleJudgment(this), NULL);
		ai_tree->AddNode("Root",   "Scount",  5, BehaviorTree::SelectRule::Priority, NULL,					   NULL);
		ai_tree->AddNode("Battle", "Pursuit", 2, BehaviorTree::SelectRule::Non,		 NULL,					   new PursuitAction(this));
		ai_tree->AddNode("Scount", "Wander",  1, BehaviorTree::SelectRule::Non,		 new WanderJudgment(this), new WanderAction(this));
		ai_tree->AddNode("Scount", "Idle",	  2, BehaviorTree::SelectRule::Non,		 NULL,					   new IdleAction(this));
	}
	else
	{
		ai_tree->AddNode("",	   "Root",		 0, BehaviorTree::SelectRule::Priority, NULL,								NULL);
		ai_tree->AddNode("Root",   "Death",		 1, BehaviorTree::SelectRule::Non,		new DeathJudgment(this),			new DeathAction(this));
		ai_tree->AddNode("Root",   "Damage",	 2, BehaviorTree::SelectRule::Non,		new DamageJudgment(this),			new DamageAction(this));
		ai_tree->AddNode("Root",   "Escape",	 3, BehaviorTree::SelectRule::Sequence, new EscapeJudgment(this),			NULL);
		ai_tree->AddNode("Root",   "Battle",	 4, BehaviorTree::SelectRule::Priority, new BattleJudgment(this),			NULL);
		ai_tree->AddNode("Root",   "Scount",	 5, BehaviorTree::SelectRule::Priority, NULL,								NULL);
		ai_tree->AddNode("Escape", "Leave",		 0, BehaviorTree::SelectRule::Non,		NULL,								new LeaveAction(this));
		ai_tree->AddNode("Escape", "Recover",    0, BehaviorTree::SelectRule::Non,		NULL,								new RecoverAction(this));
		ai_tree->AddNode("Battle", "Attack",	 1, BehaviorTree::SelectRule::Priority, new AttackJudgment(this),			NULL);
		ai_tree->AddNode("Battle", "Pursuit",	 2, BehaviorTree::SelectRule::Non,		NULL,								new PursuitAction(this));
		ai_tree->AddNode("Attack", "JumpAttack", 1, BehaviorTree::SelectRule::Non,		new  HeavyBodyAttackJudgment(this), new HeavyBodyAttackAction(this));
		ai_tree->AddNode("Attack", "BodyAttack", 2, BehaviorTree::SelectRule::Non,		NULL,								new BodyAttackAction(this));
		ai_tree->AddNode("Scount", "Wander",     1, BehaviorTree::SelectRule::Non,		new WanderJudgment(this),			new WanderAction(this));
		ai_tree->AddNode("Scount", "Idle",		 2, BehaviorTree::SelectRule::Non,		NULL,								new IdleAction(this));
}
}

//-----------------------------------------
// �X�V����
//-----------------------------------------
void EnemySlime::Update(float elapsed_time)
{
	// �r�w�C�r�A�X�V����
	BehaviorUpdate(elapsed_time);

	// ���͍X�V����
	GetMovement()->UpdateVelocity(elapsed_time);

	// ���G���ԍX�V����
	GetCharactor()->UpdateInvincibleTimer(elapsed_time);
}

//-----------------------------------------
// ������͈̓f�o�b�O�v���~�e�B�u�`��
//-----------------------------------------
void EnemySlime::DrawDebugPrimitive()
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

//-----------------------------------------
// �G�̔j��
//-----------------------------------------
void EnemySlime::Destroy()
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
	CollisionManager::Instance().UnregisterBox(CollisionManager::Instance().GetCollisionBoxFromName("SlimeAABB"));
	
	// �G�}�l�[�W���[����폜
	EnemyManager::Instance().EnemyRemove(GetActor()->GetComponent<EnemySlime>());
	
	// �A�N�^�[�}�l�[�W���[����폜
	ActorManager::Instance().Remove(GetActor());
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