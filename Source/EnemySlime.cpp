#include "Graphics.h"
#include "Charactor.h"
#include "Telegram.h"
#include "EnemySlime.h"
#include "EnemyManager.h"
#include "ActorManager.h"
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
	std::string str = "";
	if (active_node != nullptr)
	{
		str = active_node->GetName();
	}
	ImGui::Text(u8"Behavior�@%s", str.c_str());

	DirectX::XMFLOAT3 origin = GetTerritoryOrigin();
	ImGui::InputFloat3("territory_origin", &origin.x);
	int c = (int)GetActor().use_count();
	ImGui::InputInt("actor_counter", &c);
	int health = GetCharactor()->GetHealth();
	ImGui::InputInt("Health", &health);
	int id = GetCharactor()->GetID();
	ImGui::InputInt("ID", &id);
	bool attack_flag = GetAttackFlag();
	ImGui::Checkbox("AttackFlag", &attack_flag);
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

	// �e���g���[�͈͂̐ݒ�
	SetTerritoryRange(50.0f);

	// ���G�͈͂̐ݒ�
	SetSearchRange(20.0f);

	// �U���͈͂̐ݒ�
	SetAttackRange(10.0f);

	// �e���g���[�͈͂̐ݒ�
	DirectX::XMFLOAT3 position = actor->GetPosition();
	SetTerritoryOrigin(position);

	// �ŏ��̓^�[�Q�b�g���W�����g�̍��W�ɐݒ�
	SetTargetPosition(actor->GetPosition());

	// �R���W�����̐ݒ�
	{
		Model* model = GetActor()->GetModel();
		// �̂̃R���W�����ݒ�
		CollisionParameter parameter;
		parameter.name = actor->GetName();
		parameter.node_name = "";
		parameter.actor_id = charactor->GetID() + GetIdentity();
		parameter.position = { 0, 0, 0 };
		parameter.radius = 6.5f;
		parameter.height = 10.0f;
		parameter.weight = 6.5f;
		parameter.collision_flg = true;
		parameter.actor_type = CollisionActorType::Enemy;
		parameter.element = CollisionElement::Body;
		parameter.mask = CollisionPositionMask::Collision_Mask_Actor_Position;
		charactor->SetCollision(actor, parameter, CollisionMeshType::Cylinder);

		// ���˂��̃R���W�����ݒ�
		charactor->GetNodePosition("BottomEyeCover", head_position, model);
		haed_collision_name = actor->GetName();
		haed_collision_name += "Head";
		parameter.name = haed_collision_name.c_str();
		parameter.node_name = "BottomEyeCover";
		parameter.position = head_position;
		parameter.radius = 5.0f;
		parameter.weight = 1.0f;
		parameter.height = 0.0f;
		parameter.collision_flg = false;
		parameter.element = CollisionElement::Weppon;
		parameter.mask = CollisionPositionMask::Collision_Mask_Member_Position;
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
	ai_tree->AddNode("Escape", "Leave", 0, BehaviorTree::SelectRule::Non, NULL, new LeaveAction(this));
	ai_tree->AddNode("Escape", "Recover", 0, BehaviorTree::SelectRule::Non, NULL, new RecoverAction(this));
	ai_tree->AddNode("Battle", "Attack", 1, BehaviorTree::SelectRule::Priority, new AttackJudgment(this), NULL);
	ai_tree->AddNode("Battle", "Pursuit", 2, BehaviorTree::SelectRule::Non, NULL, new PursuitAction(this));
	ai_tree->AddNode("Attack", "JumpAttack", 1, BehaviorTree::SelectRule::Non, new JumpAttackJudgment(this), new JumpAttackAction(this));
	ai_tree->AddNode("Attack", "BodyAttack", 2, BehaviorTree::SelectRule::Non, NULL, new BodyAttackAction(this));
	ai_tree->AddNode("Scount", "Wander", 1, BehaviorTree::SelectRule::Non, new WanderJudgment(this), new WanderAction(this));
	ai_tree->AddNode("Scount", "Idle", 2, BehaviorTree::SelectRule::Non, NULL, new IdleAction(this));
}

//-----------------------------------------
// �X�V����
//-----------------------------------------
void EnemySlime::Update(float elapsed_time)
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
	std::vector<std::shared_ptr<CollisionSphere>> list = CollisionManager::Instance().GetCollisionSphereFromID(GetCharactor()->GetID() + GetIdentity());
	for (std::shared_ptr<CollisionSphere> sphere : list)
	{
		CollisionManager::Instance().UnregisterSphere(sphere);
	}
	CollisionManager::Instance().UnregisterCylinder(CollisionManager::Instance().GetCollisionCylinderFromName(GetName()));

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

//-----------------------------------------
// �A�j���[�V�����Đ�
//-----------------------------------------
void EnemySlime::PlayAnimation(std::shared_ptr<AnimationData> animation)
{
	// ���f���擾
	Model* model = GetActor()->GetModel();
	// �A�j���[�V�����Đ�
	model->PlayAnimation(animation->number, animation->roop_flag, animation->blend);
}