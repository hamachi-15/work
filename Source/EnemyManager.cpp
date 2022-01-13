 #include "Graphics.h"

#include "Mathf.h"

// �V�F�[�_�[�n�C���N���[�h
#include "ShaderManager.h"
#include "PhongVarianceShadowMap.h"

// �f�[�^�x�[�X�n�C���N���[�h
#include "Script.h"
#include "EnemyTerritoryManager.h"

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
	int appearance_data_count = GameDataBase::Instance().GetEnemyOccurCount();
	for (int appearance_position_index = 0; appearance_position_index < appearance_data_count; ++appearance_position_index)
	{
		std::shared_ptr<EnemyAppearancePosition> appearance_data = GameDataBase::Instance().GetEnemyAppearanceData(appearance_position_index);
		DirectX::XMFLOAT3 appearance_position = { appearance_data->position_x,appearance_data->position_y, appearance_data->position_z };
		std::shared_ptr<EnemyTerritory> territory = std::make_shared<EnemyTerritory>();
		territory->SetTerritoryOrigin(appearance_position);
		territory->SetTerritoryRange(appearance_data->radius);
		territory->SetTerritoryTag(appearance_data->tag);
		EnemyTerritoryManager::Instance().RegisterTerritory(territory);
	}
}

//-----------------------------------------------
// �G��o�^
//-----------------------------------------------
void EnemyManager::EnemyRegister(std::shared_ptr<Enemy> enemy)
{
	// ID��ݒ�
	enemy->GetCharactor()->SetID(static_cast<int>(MetaAI::Identity::Enemy));
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
	int world_map_data_max_count = GameDataBase::Instance().GetWorldMapDataCount();
	for (int i = 0; i < world_map_data_max_count; ++i)
	{
		// ���[���h�}�b�v�f�[�^���擾
		std::shared_ptr<WorldMapData> world_map_data = GameDataBase::Instance().GetWorldMapData(i);
		// ���[���h�}�b�v�f�[�^��ID����G�f�[�^���擾
		std::shared_ptr<EnemyData> enemy_data = GameDataBase::Instance().GetEnemyDataFromID(world_map_data->enemy_id);
		// ���[���h�}�b�v�f�[�^�̃^�O����G�̏o���ʒu�f�[�^���擾
		std::shared_ptr<EnemyAppearancePosition> appearance_data = GameDataBase::Instance().GetEnemyAppearanceData(world_map_data->tag);
		// �A�N�^�[�쐬
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		// �G�X�e�[�^�X�ݒ�
		SetEnemyStatus(actor, enemy_data, appearance_data);
	}
}

//-----------------------------------------------
// �X�N���v�g����G�𐶐�
//-----------------------------------------------
void EnemyManager::CreateEnemies(int id)
{
	// �G�f�[�^���X�g�ƓG�̏o���ʒu�f�[�^���X�g���擾
	std::vector<std::shared_ptr<EnemyData>> data_list = GameDataBase::Instance().GetEnemyDataList();
	std::vector<std::shared_ptr<EnemyAppearancePosition>> appearance_potition_list = GameDataBase::Instance().GetEnemyAppearanceDataList();
	int index = 0;
	for (std::shared_ptr<EnemyData> data : data_list)
	{
		// �f�[�^�x�[�X��ID�ƈ�v������G�𐶐�����
		if (data->id == id)
		{
			// �A�N�^�[�쐬
			std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
			// �o���ʒu�̃f�[�^�擾
			std::shared_ptr<EnemyAppearancePosition> appearance_data = appearance_potition_list.at(index);
			// �X�e�[�^�X�ݒ�
			SetEnemyStatus(actor, data, appearance_data);
		}
		++index;
	}
}

//-----------------------------------------
// �X�N���v�g����G�����擾���ēG�𐶐�����
//-----------------------------------------
bool EnemyManager::CreateEnemyScriptData()
{
	Script* script = new Script("./Data/Script/SendBattleSceneScript.txt");
	char	strwork[256] = {};

	while (1)
	{
		if (!script->SearchTop())
			break;
		if (script == nullptr)
		{
			return false;
		}
		//	��������t�@�C��������o��
		script->GetParamString(strwork);

		//	�I���R�}���h
		if (lstrcmpA(strwork, "END") == 0)
		{
			break;
		}

		if (lstrcmpA(strwork, "EnemyID") == 0)
		{
			// �X�N���v�g�̓GID����G�𐶐�
			CreateEnemies(script->GetParamInt());
		}
	}
	delete script;
	return true;
}

//-----------------------------------------
// �G�̃X�e�[�^�X��ݒ肷�鏈��
//-----------------------------------------
void EnemyManager::SetEnemyStatus(std::shared_ptr<Actor> actor, std::shared_ptr<EnemyData> enemy_data, std::shared_ptr<EnemyAppearancePosition> appearance_data)
{
	// ���O�̐ݒ�
	std::string name = std::string(enemy_data->name) + std::to_string(appearance_data->id);
	actor->SetName(name.c_str());

	// �G�f�[�^ID�̐ݒ�
	actor->SetEnemyDataID(enemy_data->id);
	
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
	DirectX::XMFLOAT3 appearance_position = { appearance_data->position_x,appearance_data->position_y, appearance_data->position_z };
	GetAppearancePosition(actor, { appearance_position.x, appearance_position.y, appearance_position.z }, appearance_data->radius);
	
	// �X�P�[���̐ݒ�
	actor->SetScale({ enemy_data->scale_x, enemy_data->scale_y, enemy_data->scale_z });

	// �A���O���̐ݒ�
	actor->SetAngle({ enemy_data->angle_x, enemy_data->angle_y, enemy_data->angle_z });

	// �V�F�[�_�[�̐ݒ�
	actor->SetShaderType(ShaderManager::ShaderType::Lambert);

	// �e�G�̃R���|�[�l���g�ǉ�
	AddComponent(actor, enemy_data, appearance_data->tag);
}

void EnemyManager::SetEnemyStatus(std::shared_ptr<Actor> actor, std::shared_ptr<EnemyData> enemy_data, int& string_id, DirectX::XMFLOAT3& appearance_position)
{
	// ���O�̐ݒ�
	std::string name = std::string(enemy_data->name) + std::to_string(string_id);
	actor->SetName(name.c_str());

	// �G�f�[�^ID�̐ݒ�
	actor->SetEnemyDataID(enemy_data->id);

	// ���f���̃Z�b�g�A�b�v
	actor->SetUpModel(enemy_data->model_path, enemy_data->animation_node_name);

	// �o���ʒu�̐ݒ�
	GetAppearancePosition(actor, { appearance_position.x, appearance_position.y, appearance_position.z }, 2);

	// �X�P�[���̐ݒ�
	actor->SetScale({ enemy_data->scale_x, enemy_data->scale_y, enemy_data->scale_z });

	// �A���O���̐ݒ�
	actor->SetAngle({ enemy_data->angle_x, enemy_data->angle_y, enemy_data->angle_z });

	// �V�F�[�_�[�̐ݒ�
	actor->SetShaderType(ShaderManager::ShaderType::Lambert);

	// �e�G�̃R���|�[�l���g�ǉ�
	std::shared_ptr<Enemy> enemy;
	actor->AddComponent<Movement>();
	std::shared_ptr<Charactor> charactor = actor->AddComponent<Charactor>();
	
	// �ő�HP�ݒ�
	charactor->SetMaxHealth(enemy_data->hp);

	// HP�ݒ�
	charactor->SetHealth(enemy_data->hp);

	// �G�̎�ނ��Ƃ̃R���|�[�l���g��ǉ�
	// TODO �G�����I������Ή�����
	switch (enemy_data->category)
	{
	case EnemyCategory::LAT:
		enemy = actor->AddComponent<EnemyLAT>();
		break;
	case EnemyCategory::Slime:
		enemy = actor->AddComponent<EnemySlime>();
		break;
	case EnemyCategory::PLT:
		enemy = actor->AddComponent<EnemyPLT>();
		break;
	case EnemyCategory::NightmareDragon:
		enemy = actor->AddComponent<EnemyDragonNightmare>();
		break;
	case EnemyCategory::SoulEaterDragon:
		enemy = actor->AddComponent<EnemyDragonSoulEater>();
		break;
	case EnemyCategory::DragonUsurper:
		break;
	}

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
void EnemyManager::AddComponent(std::shared_ptr<Actor> actor, std::shared_ptr<EnemyData> enemy_data, EnemyTerritoryTag& tag)
{
	std::shared_ptr<Enemy> enemy;
	actor->AddComponent<Movement>();
	std::shared_ptr<Charactor> charactor = actor->AddComponent<Charactor>();
	charactor->SetMaxHealth(enemy_data->hp);
	charactor->SetHealth(enemy_data->hp);

	// �G�̎�ނ��Ƃ̃R���|�[�l���g��ǉ�
	switch (enemy_data->category)
	{
	case EnemyCategory::Slime:
		enemy = actor->AddComponent<EnemySlime>();
		break;
	case EnemyCategory::LAT:
		enemy = actor->AddComponent<EnemyLAT>();
		break;
	case EnemyCategory::PLT:
		enemy = actor->AddComponent<EnemyPLT>();
		break;
	case EnemyCategory::NightmareDragon:
		//break;
	case EnemyCategory::SoulEaterDragon:
		//break;
	case EnemyCategory::DragonUsurper:
		enemy = actor->AddComponent<EnemyDragonNightmare>();
		break;
	}
	std::shared_ptr<EnemyTerritory> territory = EnemyTerritoryManager::Instance().GetTerritory(tag);
	enemy->SetTerritoryOrigin(territory->GetTerritoryOrigin());
	enemy->SetTerritoryRange(territory->GetTerritoryRange());
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
