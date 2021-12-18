#include "EnemyDragonNightmare.h"
#include "Graphics.h"
#include "SceneManager.h"

// ���b�Z�[�W�֘A�C���N���[�h
#include "Telegram.h"

// �A�N�^�[�֘A�C���N���[�h
#include "Charactor.h"
#include "ActorManager.h"
#include "EnemyManager.h"

// AI�֘A�C���N���[�h
#include "NodeBase.h"
#include "BehaviorTree.h"
#include "BehaviorData.h"
#include "JudgmentOwner.h"
#include "NightmareDragonJudgmentOwner.h"
#include "ActionOwner.h"
#include "NightmareDragonActionOwner.h"

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
// �G�̔j������
//--------------------------------------
void EnemyDragonNightmare::Destroy()
{
	// �A�N�^�[�̎擾
	std::shared_ptr<Actor> actor = GetActor();

	// �R���W�����폜
	std::vector<std::shared_ptr<CollisionSphere>> list = CollisionManager::Instance().GetCollisionSphereFromID(GetCharactor()->GetID() + GetIdentity());
	for (std::shared_ptr<CollisionSphere> sphere : list)
	{
		CollisionManager::Instance().UnregisterSphere(sphere);
	}
	CollisionManager::Instance().UnregisterCylinder(CollisionManager::Instance().GetCollisionCylinderFromName(actor->GetName()));

	// �G�}�l�[�W���[����폜
	EnemyManager::Instance().EnemyRemove(GetActor()->GetComponent<EnemyDragonNightmare>());

	// �A�N�^�[�}�l�[�W���[����폜
	ActorManager::Instance().Remove(GetActor());
}

//--------------------------------------
// GUI�`��
//--------------------------------------
void EnemyDragonNightmare::OnGUI()
{
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
	const char* name = SceneManager::Instance().GetCurrentScene()->GetName();

	// �V�[�������[���h�}�b�v���̃m�[�h�ݒ�
	if (strcmp(name, "SceneWorldMap") == 0)
	{
		ai_tree->AddNode("", "Root", 0, BehaviorTree::SelectRule::Priority, NULL, NULL);
		ai_tree->AddNode("Root", "Scount", 5, BehaviorTree::SelectRule::Priority, NULL, NULL);
		ai_tree->AddNode("Scount", "Sleep", 2, BehaviorTree::SelectRule::Non, NULL, new SleepAction(this));

	} // �V�[�����o�g���V�[���̎��̃m�[�h�ݒ�
	else //if (strcmp(name, "SceneBattle") == 0)
	{
		ai_tree->AddNode("", "Root", 0, BehaviorTree::SelectRule::Priority, NULL, NULL);
		ai_tree->AddNode("Root", "Death", 1, BehaviorTree::SelectRule::Non, new DeathJudgment(this), new DeathAction(this));
		ai_tree->AddNode("Root", "Damage", 2, BehaviorTree::SelectRule::Non, new DamageJudgment(this), new DamageAction(this));
		ai_tree->AddNode("Root", "Battle", 4, BehaviorTree::SelectRule::Priority, new BattleJudgment(this), NULL);
		ai_tree->AddNode("Root", "Scount", 5, BehaviorTree::SelectRule::Priority, NULL, NULL);
		ai_tree->AddNode("Battle", "Attack", 1, BehaviorTree::SelectRule::Priority, new AttackJudgment(this), NULL);
		ai_tree->AddNode("Battle", "Pursuit", 2, BehaviorTree::SelectRule::Non, NULL, new PursuitAction(this));
		//ai_tree->AddNode("Attack", "JumpAttack", 1, BehaviorTree::SelectRule::Non, new JumpAttackJudgment(this), new JumpAttackAction(this));
		//ai_tree->AddNode("Attack", "BodyAttack", 2, BehaviorTree::SelectRule::Non, NULL, new BodyAttackAction(this));
		ai_tree->AddNode("Scount", "Wander", 1, BehaviorTree::SelectRule::Non, new WanderJudgment(this), new WanderAction(this));
		ai_tree->AddNode("Scount", "Idle", 2, BehaviorTree::SelectRule::Non, NULL, new IdleAction(this));
	}
}

//--------------------------------------
// �X�V����
//--------------------------------------
void EnemyDragonNightmare::Update(float elapsed_time)
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

//--------------------------------------
// ������͈̓f�o�b�O�v���~�e�B�u�`��
//--------------------------------------
void EnemyDragonNightmare::DrawDebugPrimitive()
{
	//DebugRenderer* renderer = Graphics::Instance().GetDebugRenderer();
	//std::shared_ptr<Actor> actor = GetActor();
	//DirectX::XMFLOAT3 position = actor->GetPosition();
	//float territory_range = GetTerritoryRange();
	//DirectX::XMFLOAT3 territory_origin = GetTerritoryOrigin();
	//territory_origin.y = actor->GetPosition().y;
	//// �꒣��͈͂��f�o�b�O�~���`��
	//renderer->DrawCylinder(territory_origin, territory_range, 1.0f, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

	//// ���G�͈͂��f�o�b�O�~���`��
	//renderer->DrawCylinder(position, search_range, 1.0f, DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));

	//// �U���͈͂��f�o�b�O�~���`��
	//renderer->DrawCylinder(position, GetAttackRange(), 1.0f, DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f));

	//// �^�[�Q�b�g���W�̋��`��
	//renderer->DrawSphere(target_position, 0.5f, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
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
// �A�j���[�V�����Đ�
//--------------------------------------
void EnemyDragonNightmare::PlayAnimation(std::shared_ptr<AnimationData> animation)
{
	// ���f���擾
	Model* model = GetActor()->GetModel();
	// �A�j���[�V�����Đ�
	model->PlayAnimation(animation->number, animation->roop_flag, animation->blend);
}

