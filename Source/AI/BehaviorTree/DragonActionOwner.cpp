// �f�[�^�x�[�X
#include "GameDatabase.h"
#include "AttackCategory.h"
#include "ActorType.h"

// AI
#include "DragonActionOwner.h"

// �R���|�[�l���g
#include "Actor.h"
#include "Enemy.h"
#include "Charactor.h"

// �}�l�[�W���[
#include "ActorManager.h"
#include "EnemyManager.h"
#include "FireBallManager.h"
#include "CollisionManager.h"
#include "EnemyTerritoryManager.h"

// �Z�p�֐�
#include "Mathf.h"

// �ėp�֐�
#include "Universal.h"
//*****************************
// 
// �����킹�A�N�V����
// 
//*****************************
// -----------------------------
// ���s�O����
// -----------------------------
void TurnToTargetAction::Start()
{
	// �A�j���[�V�����Đ�
	std::string animation_name = owner->GetName() + "Walk";
	owner->PlayAnimation(animation_name.c_str());

	larp_timer = 0.0f;
	// �v���C���[�ւ̕����x�N�g���Z�o
	// �v���C���[���W�擾
	DirectX::XMFLOAT3 player_position = ActorManager::Instance().GetActor("Player")->GetPosition();
	turn_direction = Mathf::ReturnNormalizeFloatSubtract(player_position, owner->GetActor()->GetPosition());
	// Y���͍l�����Ȃ�
	turn_direction.y = 0.0f;
}

// -----------------------------
// ���s����
// -----------------------------
ActionBase::State TurnToTargetAction::Run(float elapsed_time)
{

	float run_timer = owner->GetRunTimer();
	if (run_timer <= 0.0f)
	{
		owner->SetRunTimer(1.0f);
		run_timer = owner->GetRunTimer();
	}
	else
	{
		// �^�C�}�[����
		larp_timer += elapsed_time;
		run_timer = owner->GetRunTimer() - elapsed_time;
	}
	// �v���C���[�ւ̃x�N�g���Z�o
	// �v���C���[���W�擾
	DirectX::XMFLOAT3 player_position = ActorManager::Instance().GetActor("Player")->GetPosition();
	turn_direction = Mathf::ReturnNormalizeFloatSubtract(player_position, owner->GetActor()->GetPosition());
	// Y���͍l�����Ȃ�
	turn_direction.y = 0.0f;

	// �U���������
	// �O�����Z�o
	DirectX::XMFLOAT3 angle = owner->GetActor()->GetAngle();
	front_direction.x = sinf(angle.y);
	front_direction.z = cosf(angle.y);
	// ���`���
	front_direction.x = Mathf::Lerp(front_direction.x, turn_direction.x, larp_timer);
	front_direction.z = Mathf::Lerp(front_direction.z, turn_direction.z, larp_timer);
	
	// �Z�o���������x�N�g���Ƀ^�[������
	owner->TurnToTarget(turn_direction);

	// �^�C�}�[�X�V
	owner->SetRunTimer(run_timer);

	// �����^�C�}�[��0�ȉ��ɂȂ��
	if (run_timer <= 0)
	{
		// �����^�C�}�[�����Z�b�g���Ċ�������
		owner->SetRunTimer(0.0f);
		return ActionBase::State::Complete;
	}
	return ActionBase::State::Run;
}

//***************************************
// 
// ���K�s��
// 
//***************************************
// -----------------------------
// ���s�O����
//------------------------------
void ScreamAction::Start()
{
	// �p������^�C�}�[�ݒ�
	owner->SetRunTimer(3.0f);

	// �A�j���[�V�������ݒ�
	std::string animation_name = owner->GetName();
	animation_name += "Scream";

	// �A�j���[�V�����Đ�
	owner->PlayAnimation(animation_name.c_str());
}

//------------------------------
// ���s����
//------------------------------
ActionBase::State ScreamAction::Run(float elapsed_time)
{
	// �^�C�}�[�擾
	float run_timer = owner->GetRunTimer();

	// �^�C�}�[����
	run_timer -= elapsed_time;

	// �A�j���[�V�����Đ����I�������犮����Ԃ�
	if (!owner->GetActor()->GetModel()->IsPlayAnimation() && run_timer <= 0.0f)
	{
		owner->SetRunTimer(0.0f);
		return ActionBase::State::Complete;
	}

	// �^�C�}�[�ݒ�
	owner->SetRunTimer(run_timer);

	// �p��
	return ActionBase::State::Run;
}

//*****************************
// 
// �K�[�h�A�N�V����
// 
//*****************************
// -----------------------------
// ���s�O����
// -----------------------------
void DefendAction::Start()
{
	// �A�j���[�V�����Đ�
	owner->PlayAnimation("NightmareDragonDefend");
}

// -----------------------------
// ���s����
// -----------------------------
ActionBase::State DefendAction::Run(float elapsed_time)
{
	owner->SetRunTimer(3.0f);

	float run_timer = owner->GetRunTimer();
	if (run_timer > 0.0f)
	{
		run_timer -= elapsed_time;
	}
	else
	{
		owner->SetRunTimer(0.0f);
		return State::Complete;
	}

	return State::Run;
}

//*****************************
// 
// ���݂��U��
// 
//*****************************
// -----------------------------
// ���s�O����
// -----------------------------
void BasicAttackAction::Start()
{
	// �A�j���[�V�����Đ�
	owner->PlayAnimation("NightmareDragonBasicAttack");

	// �����蔻����s�����Ԃ̃f�[�^���擾
	collision_time_data = GameDataBase::Instance().GetAttackCollitionTimeData(AttackCategory::BitingAttack, ActorType::NightmareDragon);
}

// -----------------------------
// ���s����
// -----------------------------
ActionBase::State BasicAttackAction::Run(float elapsed_time)
{
	// �A�N�^�[�擾
	std::shared_ptr<Actor> actor = owner->GetActor();

	// �U���̓����蔻�菈��
	std::string collision_name = actor->GetName();
	collision_name += "Mouth";
	AttackCollision(actor, collision_name.c_str(), collision_time_data);

	// �A�j���[�V�����Đ����I�������犮����Ԃ�
	if (!owner->GetActor()->GetModel()->IsPlayAnimation())
	{
		owner->SetRunTimer(0.0f);
		owner->SetRightOfAttack(false);
		std::shared_ptr<Charactor> charactor = actor->GetComponent<Charactor>();
		charactor->SetHitAttackFlag(false);
		return ActionBase::State::Complete;
	}
	return ActionBase::State::Run;
}

//*****************************
// 
// �܍U��
// 
//*****************************
// -----------------------------
// ���s�O����
// -----------------------------
void ClawAttackAction::Start()
{
	// �A�j���[�V�����Đ�
	owner->PlayAnimation("NightmareDragonClawAttack");
	// �����蔻����s�����Ԃ̃f�[�^���擾
	collision_time_data = GameDataBase::Instance().GetAttackCollitionTimeData(AttackCategory::ClawAttack, ActorType::NightmareDragon);
}

// -----------------------------
// ���s����
// -----------------------------
ActionBase::State ClawAttackAction::Run(float elapsed_time)
{
	// �A�N�^�[�擾
	std::shared_ptr<Actor> actor = owner->GetActor();
	
	// �U���̓����蔻�菈��
	std::string collision_name = actor->GetName();
	collision_name += "RightWrist";
	AttackCollision(actor, collision_name.c_str(), collision_time_data);

	// �A�j���[�V�����Đ����I�������犮����Ԃ�
	if (!owner->GetActor()->GetModel()->IsPlayAnimation())
	{
		owner->SetRunTimer(0.0f);
		owner->SetRightOfAttack(false);
		std::shared_ptr<Charactor> charactor = actor->GetComponent<Charactor>();
		charactor->SetHitAttackFlag(false);
		return ActionBase::State::Complete;
	}
	return ActionBase::State::Run;
}

//*****************************
// 
// �p�U��
// 
//*****************************
// -----------------------------
// ���s�O����
// -----------------------------
void HornAttackAction::Start()
{
	// �A�j���[�V�����Đ�
	owner->PlayAnimation("NightmareDragonHornAttack");

	// �����蔻����s�����Ԃ̃f�[�^���擾
	collision_time_data = GameDataBase::Instance().GetAttackCollitionTimeData(AttackCategory::HornAttack, ActorType::NightmareDragon);
}

// -----------------------------
// ���s����
// -----------------------------
ActionBase::State HornAttackAction::Run(float elapsed_time)
{
	// �A�N�^�[�擾
	std::shared_ptr<Actor> actor = owner->GetActor();

	// �U���̓����蔻�菈��
	std::string collision_name = actor->GetName();
	collision_name += "Head";
	AttackCollision(actor, collision_name.c_str(), collision_time_data);

	// �A�j���[�V�����Đ����I�������犮����Ԃ�
	if (!owner->GetActor()->GetModel()->IsPlayAnimation())
	{
		owner->SetRunTimer(0.0f);
		owner->SetRightOfAttack(false);
		std::shared_ptr<Charactor> charactor = actor->GetComponent<Charactor>();
		charactor->SetHitAttackFlag(false);
		return ActionBase::State::Complete;
	}
	return ActionBase::State::Run;
}

//*****************************
// 
// �W�����v�U��
// 
//*****************************
// -----------------------------
// ���s�O����
// -----------------------------
void BodyPressAttackAction::Start()
{
	// �p������^�C�}�[�ݒ�
	owner->SetRunTimer(2.0f);

	// �A�j���[�V�����Đ�
	owner->PlayAnimation("NightmareDragonJump");

	// �����蔻����s�����Ԃ̃f�[�^���擾
	collision_time_data = GameDataBase::Instance().GetAttackCollitionTimeData(AttackCategory::BodyPressAttack, ActorType::NightmareDragon);
}

// -----------------------------
// ���s����
// -----------------------------
ActionBase::State BodyPressAttackAction::Run(float elapsed_time)
{
	// �A�N�^�[�擾
	std::shared_ptr<Actor> actor = owner->GetActor();

	// �U���̓����蔻�菈��
	std::string collision_name = actor->GetName();
	//collision_name += "Body";
	AttackCollision(actor, collision_name.c_str(), collision_time_data, CollisionMeshType::Cylinder);

	// �^�C�}�[�擾
	float run_timer = owner->GetRunTimer();

	// �^�C�}�[����
	run_timer -= elapsed_time;

	// �A�j���[�V�����Đ����I�������犮����Ԃ�
	if (!actor->GetModel()->IsPlayAnimation() && run_timer <= 0.0f)
	{
		owner->SetRunTimer(0.0f);
		owner->SetRightOfAttack(false);
		std::shared_ptr<Charactor> charactor = actor->GetComponent<Charactor>();
		charactor->SetHitAttackFlag(false);
		return ActionBase::State::Complete;
	}

	// �^�C�}�[�ݒ�
	owner->SetRunTimer(run_timer);

	return ActionBase::State::Run;
}

//*****************************
// 
// �ːi�U��
// 
//*****************************
// -----------------------------
// ���s�O����
// -----------------------------
void LungesAttackAction::Start()
{
	std::shared_ptr<Actor> owner_actor = owner->GetActor();
	// �A�j���[�V�����Đ�
	owner->PlayAnimation("NightmareDragonRun");

	// �U���̓����蔻�菈��
	std::string collision_name = owner_actor->GetName();

	// �R���W�����}�l�[�W���[�擾
	CollisionManager& collision_manager = CollisionManager::Instance();
	std::shared_ptr<CollisionCylinder> collision = collision_manager.GetCollisionCylinderFromName(collision_name);
	collision->SetAttackFlag(true);

	// �v���C���[�ւ̕����x�N�g���Z�o
	// �v���C���[���W�擾
	DirectX::XMFLOAT3 player_position = ActorManager::Instance().GetActor("Player")->GetPosition();
	target_direction = Mathf::ReturnNormalizeFloatSubtract(player_position, owner->GetActor()->GetPosition());
	// Y���͍l�����Ȃ�
	target_direction.y = 0.0f;
}

// -----------------------------
// ���s����
// -----------------------------
ActionBase::State LungesAttackAction::Run(float elapsed_time)
{
	// �A�N�^�[�擾
	std::shared_ptr<Actor> actor = owner->GetActor();

	float run_timer = owner->GetRunTimer();
	if (run_timer <= 0.0f)
	{
		owner->SetRunTimer(Mathf::RandomRange(2.0f, 3.0f));
		run_timer = owner->GetRunTimer();
	}
	else
	{
		// �^�C�}�[����
		run_timer = owner->GetRunTimer() - elapsed_time;
	}

	// �^�C�}�[�X�V
	owner->SetRunTimer(run_timer);

	// �ړI�n�֒��������̔��菈��
	if (JedgmentToTargetPosition(owner->GetTargetPosition(), actor->GetPosition(), actor->GetName()))
	{
		// �U���̓����蔻�菈��
		std::string collision_name = actor->GetName();
		// �R���W�����}�l�[�W���[�擾
		CollisionManager& collision_manager = CollisionManager::Instance();
		std::shared_ptr<CollisionCylinder> collision = collision_manager.GetCollisionCylinderFromName(collision_name);
		collision->SetAttackFlag(false);
		owner->SetRightOfAttack(false);
		
		std::shared_ptr<Charactor> charactor = actor->GetComponent<Charactor>();
		charactor->SetHitAttackFlag(false);
		return ActionBase::State::Complete;
	}
	std::shared_ptr<EnemyTerritory> territory = EnemyTerritoryManager::Instance().GetTerritory(owner->GetBelongingToTerritory());
	DirectX::XMFLOAT3 territory_origine = territory->GetTerritoryOrigin();
	float territory_range = territory->GetTerritoryRange();
	float length;
	DirectX::XMStoreFloat(&length, DirectX::XMVector3Length(Mathf::ReturnVectorSubtract(owner->GetActor()->GetPosition(), territory_origine)));
	
	// �^�C�����I��������
	if (run_timer <= 0.0f || length >= territory_range)
	{
		// �^�C�}�[�ݒ�
		owner->SetRunTimer(0.0f);
		// �U���̓����蔻�菈��
		std::string collision_name = actor->GetName();
		CollisionManager& collision_manager = CollisionManager::Instance();
		std::shared_ptr<CollisionCylinder> collision = collision_manager.GetCollisionCylinderFromName(collision_name);
		collision->SetAttackFlag(false);
		owner->SetRightOfAttack(false);
		std::shared_ptr<Charactor> charactor = actor->GetComponent<Charactor>();
		charactor->SetHitAttackFlag(false);
		return ActionBase::State::Complete;
	}

	// �ړI�n�_�ֈړ�
	owner->MoveToDirection(target_direction, 2.5f);

	return ActionBase::State::Run;
}
//*****************************
// 
// �΋��U��
// 
//*****************************
//-----------------------------
// ���s�O����
//-----------------------------
void FireBollAttackAction::Start()
{
	std::shared_ptr<Actor> owner_actor = owner->GetActor();
	// �A�j���[�V�����Đ�
	owner->PlayAnimation("SoulEaterDragonFireBallShoot");

	// �U���̓����蔻�菈��
	std::string collision_name = owner_actor->GetName();
}

//-----------------------------
// ���s����
//-----------------------------
ActionBase::State FireBollAttackAction::Run(float elapsed_time)
{
	std::shared_ptr<Actor> actor = owner->GetActor();
	Model* model = actor->GetModel();

	// ����̃A�j���[�V�������ԂɂȂ�΃u���X�𔭎˂���
	float time = model->GetCurrentAnimationSeconds();

	// �΋����˃t���O�������Ă��Ȃ����
	if (!flag)
	{
		if (time >= 0.5f && time <= 0.6f)
		{
			// �i�s�x�N�g���擾
			DirectX::XMFLOAT3 angle = actor->GetAngle();
			direction.x = sinf(angle.y);
			direction.z = cosf(angle.y);

			// �΋��𔭎�
			FireBallManager::Instance().Create(actor, direction);
			flag = true;
		}
	}
	// �A�j���[�V�����Đ����I�������犮����Ԃ�
	if (!model->IsPlayAnimation())
	{
		owner->SetRunTimer(0.0f);
		owner->SetRightOfAttack(false);
		flag = false;
		return ActionBase::State::Complete;
	}

	return ActionBase::State::Run;
}

//*****************************
// 
// �K���U��
// 
//*****************************
//-----------------------------
// ���s�O����
//-----------------------------
void TailAttackAction::Start()
{
	std::shared_ptr<Actor> owner_actor = owner->GetActor();
	// �A�j���[�V�����Đ�
	owner->PlayAnimation("SoulEaterDragonTailAttack");

	// �����蔻����s�����Ԃ̃f�[�^���擾
	collision_time_data = GameDataBase::Instance().GetAttackCollitionTimeData(AttackCategory::TailAttack, ActorType::SoulEaterDragon);
}

//-----------------------------
// ���s����
//-----------------------------
ActionBase::State TailAttackAction::Run(float elapsed_time)
{
	// �A�N�^�[�擾
	std::shared_ptr<Actor> actor = owner->GetActor();

	// �U���̓����蔻�菈��
	std::string collision_name = actor->GetName();
	collision_name += "Tail";
	AttackCollision(actor, collision_name.c_str(), collision_time_data);

	// �A�j���[�V�����Đ����I�������犮����Ԃ�
	if (!owner->GetActor()->GetModel()->IsPlayAnimation())
	{
		owner->SetRunTimer(0.0f);
		owner->SetRightOfAttack(false);
		std::shared_ptr<Charactor> charactor = actor->GetComponent<Charactor>();
		charactor->SetHitAttackFlag(false);
		return ActionBase::State::Complete;
	}
	return ActionBase::State::Run;
}