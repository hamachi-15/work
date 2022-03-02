 #include "Graphics.h"

#include "Mathf.h"

// �V�F�[�_�[�n�C���N���[�h
#include "ShaderManager.h"
#include "PhongVarianceShadowMap.h"

// �f�[�^�x�[�X�n�C���N���[�h
#include "GameDatabase.h"
#include "EnemyCategory.h"

// �}�l�[�W���[�C���N���[�h
#include "EnemyTerritoryManager.h"
#include "SceneManager.h"
#include "UIManager.h"
#include "CollisionManager.h"

// �A�N�^�[�n�C���N���[�h
#include "Charactor.h"
#include "Movement.h"
#include "ActorManager.h"
#include "EnemyManager.h"
#include "EnemySlime.h"
#include "EnemyPLT.h"
#include "EnemyLAT.h"
#include "EnemyDragonNightmare.h"
#include "EnemyDragonSoulEater.h"

#include "BossHealthUI.h"

#include "ActorType.h"
#include "CollisionMeshType.h"
#include "EnemyCollision.h"
#include "CullingCollision.h"

//-----------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------
EnemyManager::EnemyManager()
{
}

//-----------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------
EnemyManager::~EnemyManager()
{
}

//-----------------------------------------------
// �꒣��̐ݒ�
//-----------------------------------------------
void EnemyManager::CreateTerritory()
{
	int appearance_data_count = GameDataBase::Instance().GetEnemyTerritoryCount();
	for (int appearance_position_index = 0; appearance_position_index <= GameDataBase::Buttle_Territory_index; ++appearance_position_index)
	{
		// �o���ʒu�̃f�[�^���擾
		std::shared_ptr<EnemyTerritoryPosition> appearance_data = GameDataBase::Instance().GetEnemyTerritoryData(appearance_position_index);
		DirectX::XMFLOAT3 appearance_position = { appearance_data->position_x,appearance_data->position_y, appearance_data->position_z };
		// �e���g���[���쐬
		std::shared_ptr<EnemyTerritory> territory = std::make_shared<EnemyTerritory>();
		// �o���ʒu�̃f�[�^����e���g���[��ݒ�
		territory->SetTerritoryOrigin(appearance_position);
		territory->SetTerritoryRange(appearance_data->radius);
		territory->SetTerritoryTag(appearance_data->tag);
		// �e���g���[�}�l�[�W���[�ɐݒ�
		EnemyTerritoryManager::Instance().RegisterTerritory(territory);

		// �e���g���[�̓G�����j���ꂽ���Ǘ�����t���O��ǉ�
		defeat_territory[appearance_data->tag] = false;
	}
}

//-----------------------------------------------
// ���j�t���O���N���A
//-----------------------------------------------
void EnemyManager::CrearDefeat()
{
	// ���j�t���O���N���A
	defeat_territory.clear();

	// �꒣��̐ݒ�
	CreateTerritory();
}

//-----------------------------------------------
// �G��o�^
//-----------------------------------------------
void EnemyManager::EnemyRegister(std::shared_ptr<Enemy> enemy)
{
	// ID��ݒ�
	enemy->SetIdentity(identity);
	//�ݒ肵����C���N�������g����
	identity++;
	// �G���X�g�ɒǉ�
	enemies.emplace_back(enemy);
}

//-----------------------------------------------
// �G��j��
//-----------------------------------------------
void EnemyManager::EnemyRemove(std::shared_ptr<Enemy> enemy)
{
	std::vector<std::shared_ptr<Enemy>>::iterator iterate_enemy = std::find(enemies.begin(), enemies.end(), enemy);
	enemies.erase(iterate_enemy);
}

//-----------------------------------------------
// �S�Ă̓G��j��
//-----------------------------------------------
void EnemyManager::AllRemove()
{
	std::vector<std::shared_ptr<Enemy>>::iterator iterate_enemy = enemies.begin();
	for (; iterate_enemy != enemies.end(); iterate_enemy = enemies.begin())
	{
		enemies.erase(iterate_enemy);
	}
	enemies.clear();
}

//-----------------------------------------------
// �f�o�b�O�v���~�e�B�u�`��
//-----------------------------------------------
void EnemyManager::DrawDebugPrimitive()
{
	for (std::shared_ptr<Enemy> enemy : enemies)
	{
		enemy->DrawDebugPrimitive();
	}
}

//-----------------------------------------------
// �G�𐶐�
//-----------------------------------------------
void EnemyManager::CreateEnemies()
{
	std::vector<std::shared_ptr<WorldMapData>> world_map_data = GameDataBase::Instance().GetWorldMapDataList();
	
	int world_map_data_max_count = world_map_data.size();
	for (int i = 0; i < world_map_data_max_count; ++i)
	{
		// ���[���h�}�b�v�f�[�^���擾
		std::shared_ptr<WorldMapData> world_map_data = GameDataBase::Instance().GetWorldMapData(i);
		
		// ���[���h�}�b�v�f�[�^�̃^�O����e���g���[�f�[�^���擾
		std::shared_ptr<EnemyTerritoryPosition> territory_data = GameDataBase::Instance().GetEnemyTerritoryData(world_map_data->tag);
		
		// �G�����j����Ă���Δ�΂�
		if (defeat_territory[world_map_data->tag]) continue;

		// ���[���h�}�b�v�f�[�^��ID����G�f�[�^���擾
		std::shared_ptr<EnemyData> enemy_data = GameDataBase::Instance().GetEnemyDataFromID(world_map_data->enemy_id);
		
		// �A�N�^�[�쐬
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		
		// �G�X�e�[�^�X�ݒ�
		SetEnemyStatus(actor, enemy_data, territory_data);
		
		// �e�G�̃R���|�[�l���g�ǉ�
		AddComponent(actor, enemy_data, world_map_data->tag);
	}
}

//-----------------------------------------------
// �G���J�E���g�f�[�^����G�𐶐�
//-----------------------------------------------
void EnemyManager::CreateEnemies(int id, EnemyTerritoryTag territory_tag)
{
	// �G�f�[�^���X�g�ƓG�̏o���ʒu�f�[�^���X�g���擾
	std::vector<std::shared_ptr<EnemyData>> enemy_data_list = GameDataBase::Instance().GetEnemyDataList();
	std::vector<std::shared_ptr<EnemyTerritoryPosition>> territory_potition_list = GameDataBase::Instance().GetEnemyTerritoryDataList();
	
	// �e���g���[�f�[�^�̃C���f�b�N�X���o�g���V�[���̂Ƃ���ɐݒ�
	int territory_index = GameDataBase::Buttle_Territory_index;

	// �e���g���[�f�[�^�擾
	std::shared_ptr<EnemyTerritoryPosition> territory_data = territory_potition_list.at(territory_index);

	// �o���ʒu�f�[�^�̃C���f�b�N�X���o�g���V�[���̂Ƃ���ɐݒ�
	int appearance_index = GameDataBase::Buttle_Appearance_Position_Index;

	// �G�f�[�^���Q�Ƃ��Ă�����
	for (std::shared_ptr<EnemyData> data : enemy_data_list)
	{
		// �f�[�^�x�[�X��ID�ƈ�v������G�𐶐�����
		if (data->id == id)
		{
			// �A�N�^�[�쐬
			std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
			// �o���ʒu�̃f�[�^�擾
			std::shared_ptr<EnemyTerritoryPosition> appearance_data = territory_potition_list.at(appearance_index);
			// �X�e�[�^�X�ݒ�
			SetEnemyStatus(actor, data, territory_data, { appearance_data->position_x, appearance_data->position_y, appearance_data->position_z });
			// �e�G�̃R���|�[�l���g�ǉ�
			AddComponent(actor, data, territory_tag);
			// �C���f�b�N�X�̃C���f���g
			++appearance_index;
		}
	}
}

//-----------------------------------------
// �G���J�E���g�f�[�^����G�����擾���ēG�𐶐�����
//-----------------------------------------
bool EnemyManager::CreateEnemyEncountData()
{
	appearance_ramdam_flag = false;
	std::vector<EncountEnemyTerritory> encount_enemies = GameDataBase::Instance().GetEncountEnemyList();
	for(EncountEnemyTerritory encount_enemy : encount_enemies)
	{
		// �X�N���v�g�̓GID����G�𐶐�
		CreateEnemies(encount_enemy.id, encount_enemy.tag);
	}
	appearance_ramdam_flag = true;
	return true;
}

//-----------------------------------------
// �G�̃X�e�[�^�X��ݒ肷�鏈��
//-----------------------------------------
void EnemyManager::SetEnemyStatus(std::shared_ptr<Actor> actor, std::shared_ptr<EnemyData> enemy_data, std::shared_ptr<EnemyTerritoryPosition> territory_data, DirectX::XMFLOAT3 appearance_position)
{
	//�ݒ肵����C���N�������g����
	identity++;
	// ���O�̐ݒ�
	std::string name = std::string(enemy_data->name) + std::to_string(identity);
	actor->SetName(name.c_str());
	
	// ���f���̃Z�b�g�A�b�v
	if (strcmp(enemy_data->animation_node_name, "NULL") == 0)
	{
		actor->SetUpModel(enemy_data->model_path, nullptr);
	}
	else
	{
		actor->SetUpModel(enemy_data->model_path, enemy_data->animation_node_name);
	}

	// �o���ʒu�̐ݒ�
	DirectX::XMFLOAT3 territory_position = { territory_data->position_x, territory_data->position_y, territory_data->position_z };
	// �o���ʒu�̃����_���t���O����������
	if (appearance_ramdam_flag)
	{	// �o���ʒu��꒣��͈͓��̃����_���Ȉʒu�ɐݒ�
		GetAppearancePosition(actor, { territory_position.x, territory_position.y, territory_position.z }, territory_data->radius);
	}
	else
	{	// �o���ʒu���w��̈ʒu�ɐݒ�
		actor->SetPosition(appearance_position);
	}

	// �X�P�[���̐ݒ�
	actor->SetScale({ enemy_data->scale_x, enemy_data->scale_y, enemy_data->scale_z });

	// �A���O���̐ݒ�
	actor->SetAngle({ enemy_data->angle_x, enemy_data->angle_y, enemy_data->angle_z });

	// �V�F�[�_�[�̐ݒ�
	actor->SetShaderType(ShaderManager::ShaderType::Phong);
}

//-----------------------------------------------
// �o���ʒu�����߂鏈��
//-----------------------------------------------
void EnemyManager::GetAppearancePosition(std::shared_ptr<Actor> actor, DirectX::XMFLOAT3 appearance_origin, float appearance_range)
{
	float theta = Mathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
	float range = Mathf::RandomRange(0.0f, appearance_range);

	DirectX::XMFLOAT3 appearance_position;
	appearance_position.x = appearance_origin.x + sinf(theta) * range;
	appearance_position.y = appearance_origin.y;
	appearance_position.z = appearance_origin.z + cosf(theta) * range;
	actor->SetPosition(appearance_position);
}

//-----------------------------------------------
// �X���C���̃R���|�[�l���g��ǉ����鏈��
//-----------------------------------------------
void EnemyManager::AddComponent(std::shared_ptr<Actor> actor, std::shared_ptr<EnemyData> enemy_data, EnemyTerritoryTag& territory_tag)
{
	std::shared_ptr<Charactor> charactor = actor->AddComponent<Charactor>(static_cast<int>(MetaAI::Identity::Enemy));
	charactor->SetMaxHealth(enemy_data->hp);
	charactor->SetHealth(enemy_data->hp);

	std::shared_ptr<Enemy> enemy;
	// ���[�u�����g�R���|�[�l���g�ǉ�
	actor->AddComponent<Movement>();

	// �G�̎�ނ��Ƃ̃R���|�[�l���g��ǉ�
	switch (enemy_data->category)
	{
	case EnemyCategory::Slime:
		enemy = actor->AddComponent<EnemySlime>();
		actor->AddComponent<EnemyCollision>(EnemyCategory::Slime, identity);

		// �J�����O�p�R���W������ǉ�
		CollisionManager::Instance().RegisterCulling(
			std::make_shared<CullingCollision>(EnemyCategory::Slime, actor));
		break;
	case EnemyCategory::LAT:
		enemy = actor->AddComponent<EnemyLAT>();
		actor->AddComponent<EnemyCollision>(EnemyCategory::LAT, identity);

		// �J�����O�p�R���W������ǉ�
		CollisionManager::Instance().RegisterCulling(
			std::make_shared<CullingCollision>(EnemyCategory::LAT, actor));
		break;
	case EnemyCategory::PLT:
		enemy = actor->AddComponent<EnemyPLT>();
		actor->AddComponent<EnemyCollision>(EnemyCategory::PLT, identity);

		// �J�����O�p�R���W������ǉ�
		CollisionManager::Instance().RegisterCulling(
			std::make_shared<CullingCollision>(EnemyCategory::PLT, actor));
		break;
	case EnemyCategory::SoulEaterDragon:
	case EnemyCategory::DragonUsurper:
		if (IsBattleScene()) {
			std::shared_ptr<BossHealthUI> ui = actor->AddComponent<BossHealthUI>();
			UIManager::Instance().RegisterUI(ui);
		}
		enemy = actor->AddComponent<EnemyDragonSoulEater>();
		actor->AddComponent<EnemyCollision>(EnemyCategory::SoulEaterDragon, identity);
		// �J�����O�p�R���W������ǉ�
		//CollisionManager::Instance().RegisterCulling(
		//	std::make_shared<CullingCollision>(EnemyCategory::NightmareDragon, actor));
		break;
	case EnemyCategory::NightmareDragon:
		if (IsBattleScene()) {
			std::shared_ptr<BossHealthUI> ui = actor->AddComponent<BossHealthUI>();
			UIManager::Instance().RegisterUI(ui);
		}
		enemy = actor->AddComponent<EnemyDragonNightmare>();
		actor->AddComponent<EnemyCollision>(EnemyCategory::NightmareDragon, identity);
		// �J�����O�p�R���W������ǉ�
		CollisionManager::Instance().RegisterCulling(
			std::make_shared<CullingCollision>(EnemyCategory::NightmareDragon, actor));
		break;
	}
	// �A�C�f���e�B�e�B�ݒ�
	enemy->SetIdentity(identity);

	// �����e���g���[�ݒ�
	enemy->SetBelongingToTerritory(territory_tag);

	// �G�f�[�^ID�̐ݒ�
	enemy->SetEnemyDataID(enemy_data->id);

}

//-----------------------------------------------
// ID�œG���擾
//-----------------------------------------------
std::shared_ptr<Enemy> EnemyManager::GetEnemyFromID(int id)
{
	int enemy_count = GetEnemyCount();
	for (int i = 0; i < enemy_count; ++i)
	{
		std::shared_ptr<Enemy> enemy = enemies.at(i);
		// id�ƃA�C�f���e�B�e�B�������̓G���擾����
		if (enemy->GetCharactor()->GetID() + enemy->GetIdentity() ==  id)
		{
			return enemy;
		}
	}
	return nullptr;
}
