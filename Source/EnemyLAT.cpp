#include "EnemyLAT.h"
#include "Graphics.h"
#include "Charactor.h"
#include "EnemyManager.h"
#include "BehaviorTree.h"
#include "BehaviorData.h"
#include "NodeBase.h"
#include "JudgmentOwner.h"
#include "ActionOwner.h"

//-----------------------------------------
// �R���X�g���N�^
//-----------------------------------------
EnemyLAT::EnemyLAT()
{
}

//-----------------------------------------
// �f�X�g���N�^
//-----------------------------------------
EnemyLAT::~EnemyLAT()
{
}

//-----------------------------------------
// GUI�`��
//-----------------------------------------
void EnemyLAT::OnGUI()
{
	DirectX::XMFLOAT3 origin = GetTerritoryOrigin();
	ImGui::InputFloat3("territory_origin", &origin.x);
	bool attack_flag = GetAttackFlag();
	ImGui::Checkbox("AttackFlag", &attack_flag);

	// �r�w�C�r�A�֘A���
	if (ImGui::CollapsingHeader("BehaviorTree"))
	{
		ImGui::TextColored(ImVec4(1, 0, 1, 1), u8"-------�A�N�e�B�u�ɂȂ��Ă���m�[�h------");
		std::string child_str = "";
		std::string parent_str = "";
		if (active_node != nullptr)
		{
			parent_str = active_node->GetParent()->GetName();
			child_str = active_node->GetName();
		}
		ImGui::Text(u8"ActiveParentNode�@%s", parent_str.c_str());
		ImGui::Text(u8"ActiveChildNode�@%s", child_str.c_str());

		ImGui::TextColored(ImVec4(1, 0, 1, 1), u8"-------�m�[�h�c���[-------");
		ai_tree->DrawNodeGUI();
	}
}

//-----------------------------------------
// �J�n����
//-----------------------------------------
void EnemyLAT::Start()
{
	// �A�N�^�[�̎擾
	std::shared_ptr<Actor> actor = GetActor();

	// �L�����N�^�[�擾
	std::shared_ptr<Charactor> charactor = actor->GetComponent<Charactor>();

	// ���O�ݒ�
	SetName("LAT");

	// ���[�u�����g�R���|�[�l���g�̐ݒ�
	SetMovement(actor->GetComponent<Movement>());

	// �L�����N�^�[�R���|�[�l���g�̐ݒ�
	SetCharactor(charactor);

	// �}�l�[�W���[�ɓo�^
	EnemyManager::Instance().EnemyRegister(actor->GetComponent<EnemyLAT>());

	// ���G�͈͂̐ݒ�
	SetSearchRange(10.0f);

	// �U���͈͂̐ݒ�
	SetAttackRange(5.0f);

	// �e���g���[�͈͂̐ݒ�
	DirectX::XMFLOAT3 position = actor->GetPosition();
	SetTerritoryOrigin(position);


	// �ŏ��̓^�[�Q�b�g���W�����g�̍��W�ɐݒ�
	SetTargetPosition(actor->GetPosition());

	// �R���W�����ݒ�
	{
		CollisionParameter parameter;
		// ���f���擾
		Model* model = actor->GetModel();

		// �̂̃R���W�����ݒ�
		parameter.name = GetName();
		parameter.actor_id = charactor->GetID() + GetIdentity();
		parameter.position = { 0.0f, 0.0f, 0.0f };
		parameter.radius = 3.0f;
		parameter.weight = 1.0f;
		parameter.height = 5.0f;
		parameter.collision_flg = true;
		parameter.actor_type = CollisionActorType::Enemy;
		parameter.element = CollisionElement::Body;
		parameter.mask = CollisionPositionMask::Collision_Mask_Actor_Position;
		charactor->SetCollision(actor, parameter, CollisionMeshType::Cylinder);

		// �K���̃R���W�����ݒ�
		DirectX::XMFLOAT3 tail_position;
		charactor->GetNodePosition("LAT:tai102_M_BK", tail_position, model);
		parameter.name = "LATTail";
		parameter.node_name = "LAT:tai102_M_BK";
		parameter.actor_id = charactor->GetID();
		parameter.position = tail_position;
		parameter.radius = 3.0f;
		parameter.weight = 1.0f;
		parameter.height = 0.0f;
		parameter.collision_flg = false;
		parameter.actor_type = CollisionActorType::Enemy;
		parameter.element = CollisionElement::Weppon;
		parameter.mask = CollisionPositionMask::Collision_Mask_Member_Position;
		charactor->SetCollision(actor, parameter, CollisionMeshType::Sphere);
	}

	// �r�w�C�r�A�c���[�̐ݒ�
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
	ai_tree->AddNode("Battle", "Attack", 1, BehaviorTree::SelectRule::Random, new AttackJudgment(this), NULL);
	ai_tree->AddNode("Battle", "Pursuit", 2, BehaviorTree::SelectRule::Non, NULL, new PursuitAction(this));
	//ai_tree->AddNode("Attack", "TailAttack", 0, BehaviorTree::SelectRule::Non, new SkillShotJudgment(this), new SkillShotAction(this));
	ai_tree->AddNode("Scount", "Wander", 1, BehaviorTree::SelectRule::Non, new WanderJudgment(this), new WanderAction(this));
	ai_tree->AddNode("Scount", "Idle", 2, BehaviorTree::SelectRule::Non, NULL, new IdleAction(this));

}

//-----------------------------------------
// �X�V����
//-----------------------------------------
void EnemyLAT::Update(float elapsed_time)
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
void EnemyLAT::DrawDebugPrimitive()
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
// ���b�Z�[�W����M�����Ƃ��̏���
//-----------------------------------------
bool EnemyLAT::OnMessages(const Telegram& message)
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
	case MessageType::Message_Hit_Boddy:
		break;
	}
	return false;
}

//-----------------------------------------
// �A�j���[�V�����Đ�
//-----------------------------------------
void EnemyLAT::PlayAnimation(std::shared_ptr<AnimationData> animation)
{
	// ���f���擾
	Model* model = GetActor()->GetModel();
	// �A�j���[�V�����Đ�
	model->PlayAnimation(animation->number, animation->roop_flag, animation->blend);
}
