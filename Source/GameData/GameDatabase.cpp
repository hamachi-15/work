#include "GameDatabase.h"
#include <iostream>
#include <fstream>
// �`��
#include "Graphics.h"
#include "Texture.h"

// �R���|�[�l���g
#include "Actor.h"
// �}�l�[�W���[
#include "EnemyManager.h"

// �f�[�^�x�[�X
#include "StageObjectData.h"
#include "WorldMapObjectPositionData.h"
#include "ButtleMapObjectPositionData.h"
#include "EnemyData.h"
#include "EnemyTerritoryPosition.h"
#include "CollisionParameterData.h"
#include "CullingCollisionParameterData.h"
#include "AttackCollitionTime.h"
#include "AnimationData.h"
#include "WorldMapData.h"

//-----------------------------------
// �t�@�C���ǂݍ���
//-----------------------------------
char* LoadBuffer(const char* filepath)
{
	std::ifstream ifs(filepath, std::ios::binary);
	ifs.seekg(0, std::ios::end);
	unsigned long long buffer_size = ifs.tellg();
	ifs.seekg(0);
	char* data = new char[buffer_size];
	ifs.read(data, buffer_size);
	return data;
}

//-----------------------------------
// �R���X�g���N�^
//-----------------------------------
GameDataBase::GameDataBase()
{
	Graphics& graphics = Graphics::Instance();
	// �e�N�X�`���쐬
	timing_texture = std::make_unique<Texture>();
	timing_texture->Create(static_cast<u_int>(graphics.GetScreenWidth()), static_cast<u_int>(graphics.GetScreenHeight()), DXGI_FORMAT_R8G8B8A8_UNORM);
	
	// �o�C�i���t�@�C������X�e�[�W�I�u�W�F�N�g�f�[�^�\�z
	LoadStageObjectData();
	
	// �o�C�i���t�@�C�����烏�[���h�}�b�v�I�u�W�F�N�g�f�[�^�\�z
	LoadWorldMapObjectData();
	
	// �o�C�i���t�@�C������o�g���}�b�v�I�u�W�F�N�g�f�[�^�\�z
	LoadButtleMapObjectData();
	
	// �o�C�i���t�@�C������G�f�[�^�\�z
	LoadEnemyData();
	
	// �o�C�i���t�@�C������G�����ʒu�f�[�^�\�z
	LoadEnemySetPositionData();
	
	// �o�C�i���t�@�C������G�̃e���g���[�f�[�^�\�z
	LoadEnemyTerritoryData();
	
	// �o�C�i���t�@�C������R���W�����f�[�^�\�z
	LoadCollisionData();
	
	// �o�C�i���t�@�C������J�����O�R���W�����̃f�[�^�\�z
	LoadCullingCollisionData();
	
	// �o�C�i���t�@�C�����瓖���蔻����s�����Ԃ̃f�[�^�\�z
	LoadCollisionTimeData();
	
	// �o�C�i���t�@�C������A�j���[�V�����̃f�[�^�\�z
	LoadAnimationData();
}

//-----------------------------------
// �f�X�g���N�^
//-----------------------------------
GameDataBase::~GameDataBase()
{
	// �X�e�[�W�I�u�W�F�N�gd�f�[�^�̔j��
	DeleteLoadData(stage_object_load_data);
	
	// ���[���h�}�b�v�̃I�u�W�F�N�g�f�[�^�̔j��
	DeleteLoadData(world_map_object_load_data);
	
	// �o�g���}�b�v�̃I�u�W�F�N�g�f�[�^�̔j��
	DeleteLoadData(buttle_map_object_load_data);
	
	// �G�f�[�^�̔j��
	DeleteLoadData(enmy_load_data);
	
	// �e���g���[�f�[�^�̔j��
	DeleteLoadData(enemy_territory_load_data);
	
	// �G�̐����ʒu�f�[�^�̔j��
	DeleteLoadData(enemy_position_load_data);
	
	// �J�����O���s�����߂̓����蔻��f�[�^�̔j��
	DeleteLoadData(culling_parameter_load_data);
	
	// �����蔻����s���^�C�~���O�̃f�[�^�j��
	DeleteLoadData(collision_time_load_data);
	
	// �����蔻��p�����[�^�̃f�[�^�j��
	DeleteLoadData(collision_parameter_load_data);
	
	// �A�j���[�V�����f�[�^�j��
	DeleteLoadData(animation_load_data);
}

//------------------------------
// �ǂݍ��ރf�[�^�̔j��
//------------------------------
void GameDataBase::DeleteLoadData(LoadData& load_data)
{
	// �o�b�t�@�̔j��
	if (load_data.buffer != nullptr)
	{
		delete[] load_data.buffer;
		load_data.buffer = nullptr;
	}
	
	// �e�L�X�g�o�b�t�@�̔j��
	if (load_data.text_buffer != nullptr)
	{
		delete[] load_data.text_buffer;
		load_data.text_buffer = nullptr;
	}

	// �f�[�^�̐��̏�����
	load_data.data_count = 0;
	load_data.headder.data_count = 0;

	// ������̒�����������
	load_data.headder.string_length = 0;
}

//-------------------------------
// �ǂݍ��ރf�[�^���쐬����
//-------------------------------
void GameDataBase::CreateData(const char* filename, LoadData& load_data)
{
	// �t�@�C���ǂݍ���
	load_data.buffer = LoadBuffer(filename);

	// �f�[�^�w�b�_�ǂݍ���
	memcpy_s(&load_data.headder, sizeof(load_data.headder), load_data.buffer, sizeof(load_data.headder));

	// �o�g���}�b�v�̃I�u�W�F�N�g�f�[�^���ݒ�
	load_data.data_count = load_data.headder.data_count;

	// �e�L�X�g�o�b�t�@����
	load_data.text_buffer = new char[load_data.headder.string_length];

	// �e�L�X�g�o�b�t�@�ǂݍ���
	memcpy_s(load_data.text_buffer, load_data.headder.string_length, &load_data.buffer[sizeof(load_data.headder)], load_data.headder.string_length);
}

//--------------------------------------------------
// �X�e�[�W�I�u�W�F�N�g�f�[�^�ǂݍ���
//--------------------------------------------------
void GameDataBase::LoadStageObjectData()
{
	// �o�C�i���t�@�C������ǂݍ��ރf�[�^���쐬
	CreateData("Data/GameData/StageObjectData.dat", stage_object_load_data);

	// �}�b�v�̃I�u�W�F�N�g�f�[�^����
	for (int i = 0; i < stage_object_load_data.data_count; ++i)
	{
		stage_object_data.emplace_back(std::make_shared<StageObjectData>());
	}

	// �o�g���}�b�v�̃I�u�W�F�N�g�f�[�^�\�z
	for (int i = 0; i < stage_object_load_data.data_count; ++i)
	{
		StageObjectDataReader* data = &((StageObjectDataReader*)&stage_object_load_data.buffer[sizeof(stage_object_load_data.headder) + stage_object_load_data.headder.string_length])[i];
		stage_object_data[i]->id = data->id;
		stage_object_data[i]->object_name = &stage_object_load_data.text_buffer[data->object_name];
		stage_object_data[i]->model_path = &stage_object_load_data.text_buffer[data->model_path];
		stage_object_data[i]->object_category = data->object_category;
		stage_object_data[i]->angle_x = data->angle_x;
		stage_object_data[i]->angle_y = data->angle_y;
		stage_object_data[i]->angle_z = data->angle_z;
		stage_object_data[i]->scale_x = data->scale_x;
		stage_object_data[i]->scale_y = data->scale_y;
		stage_object_data[i]->scale_z = data->scale_z;
	}
}

//-----------------------------------
// ���[���h�}�b�v�̃I�u�W�F�N�g�f�[�^�ǂݍ���
//-----------------------------------
void GameDataBase::LoadWorldMapObjectData()
{
	// �o�C�i���t�@�C������ǂݍ��ރf�[�^���쐬
	CreateData("Data/GameData/WorldMapObjectPositionData.dat", world_map_object_load_data);

	// ���[���h�}�b�v�̃I�u�W�F�N�g���W�f�[�^����
	for (int i = 0; i < world_map_object_load_data.data_count; ++i)
	{
		world_map_object_position_data.emplace_back(std::make_shared<WorldMapObjectPositionData>());
	}

	// ���[���h�}�b�v�̃I�u�W�F�N�g���W�f�[�^�\�z
	for (int i = 0; i < world_map_object_load_data.data_count; ++i)
	{
		WorldMapObjectPositionDataReader* data = &((WorldMapObjectPositionDataReader*)&world_map_object_load_data.buffer[sizeof(world_map_object_load_data.headder) + world_map_object_load_data.headder.string_length])[i];
		world_map_object_position_data[i]->id = data->id;
		world_map_object_position_data[i]->name = &world_map_object_load_data.text_buffer[data->name];
		world_map_object_position_data[i]->object_category = data->object_category;
		world_map_object_position_data[i]->position_x = data->position_x;
		world_map_object_position_data[i]->position_y = data->position_y;
		world_map_object_position_data[i]->position_z = data->position_z;
	}

}

//-----------------------------------
// �o�g���}�b�v�̃I�u�W�F�N�g�f�[�^�ǂݍ���
//-----------------------------------
void GameDataBase::LoadButtleMapObjectData()
{
	// �o�C�i���t�@�C������ǂݍ��ރf�[�^���쐬
	CreateData("Data/GameData/ButtleMapObjectPositionData.dat", buttle_map_object_load_data);

	// �o�g���}�b�v�̃I�u�W�F�N�g���W�f�[�^����
	for (int i = 0; i < buttle_map_object_load_data.data_count; ++i)
	{
		buttle_map_object_position_data.emplace_back(std::make_shared<ButtleMapObjectPositionData>());
	}


	// �o�g���}�b�v�̃I�u�W�F�N�g���W�f�[�^	�\�z
	for (int i = 0; i < buttle_map_object_load_data.data_count; ++i)
	{
		ButtleMapObjectPositionDataReader* data = &((ButtleMapObjectPositionDataReader*)&buttle_map_object_load_data.buffer[sizeof(buttle_map_object_load_data.headder) + buttle_map_object_load_data.headder.string_length])[i];
		buttle_map_object_position_data[i]->id = data->id;
		buttle_map_object_position_data[i]->name = &buttle_map_object_load_data.text_buffer[data->name];
		buttle_map_object_position_data[i]->object_category = data->object_category;
		buttle_map_object_position_data[i]->position_x = data->position_x;
		buttle_map_object_position_data[i]->position_y = data->position_y;
		buttle_map_object_position_data[i]->position_z = data->position_z;
	}

}

//-----------------------------------
// �G�f�[�^�ǂݍ���
//-----------------------------------
void GameDataBase::LoadEnemyData()
{
	// �o�C�i���t�@�C������ǂݍ��ރf�[�^���쐬
	CreateData("Data/GameData/EnemyData.dat", enmy_load_data);

	// �G�̃X�e�[�^�X�f�[�^����
	for (int i = 0; i < enmy_load_data.data_count; ++i)
	{
		enemy_data.emplace_back(std::make_shared<EnemyData>());
	}

	// �G�̃X�e�[�^�X�f�[�^�\�z
	for (int i = 0; i < enmy_load_data.data_count; ++i)
	{
		EnemyDataReader* data = &((EnemyDataReader*)&enmy_load_data.buffer[sizeof(enmy_load_data.headder) + enmy_load_data.headder.string_length])[i];
		enemy_data[i]->id = data->id;
		enemy_data[i]->name = &enmy_load_data.text_buffer[data->name];
		enemy_data[i]->category = data->category;
		enemy_data[i]->model_path = &enmy_load_data.text_buffer[data->path];
		enemy_data[i]->animation_node_name = &enmy_load_data.text_buffer[data->node_name];
		enemy_data[i]->scale_x = data->scale_x;
		enemy_data[i]->scale_y = data->scale_y;
		enemy_data[i]->scale_z = data->scale_z;
		enemy_data[i]->angle_x = data->angle_x;
		enemy_data[i]->angle_y = data->angle_y;
		enemy_data[i]->angle_z = data->angle_z;
		enemy_data[i]->hp = data->hp;
		enemy_data[i]->deffence = data->deffence;
		enemy_data[i]->attack = data->attack;
		enemy_data[i]->speed = data->speed;
		enemy_data[i]->speed_scale = data->speed_sclae;
	}

}

//-----------------------------------
// �o�C�i���t�@�C������G�̔z�u�f�[�^�ǂݍ���
//-----------------------------------
void GameDataBase::LoadEnemySetPositionData()
{
	// �o�C�i���t�@�C������ǂݍ��ރf�[�^���쐬
	CreateData("Data/GameData/WorldMapData.dat", enemy_position_load_data);

	// ���[���h�}�b�v�̓G�����f�[�^����
	for (int i = 0; i < enemy_position_load_data.data_count; ++i)
	{
		world_map_data.emplace_back(std::make_shared<WorldMapData>());
	}	

	// ���[���h�}�b�v�̓G�����f�[�^�\�z
	for (int i = 0; i < enemy_position_load_data.data_count; ++i)
	{
		WorldMapDataReader* data = &((WorldMapDataReader*)&enemy_position_load_data.buffer[sizeof(enemy_position_load_data.headder) + enemy_position_load_data.headder.string_length])[i];
		world_map_data[i]->id = data->id;
		world_map_data[i]->enemy_id = data->enemy_id;
		world_map_data[i]->tag = data->tag;
	}

}

//-----------------------------------
// �G�̃e���g���[�f�[�^�ǂݍ���
//-----------------------------------
void GameDataBase::LoadEnemyTerritoryData()
{
	// �o�C�i���t�@�C������ǂݍ��ރf�[�^���쐬
	CreateData("Data/GameData/EnemyTerritoryPosition.dat", enemy_territory_load_data);

	// �G�̃e���g���[�f�[�^����
	for (int i = 0; i < enemy_territory_load_data.data_count; ++i)
	{
		enemy_territory_data.emplace_back(std::make_shared<EnemyTerritoryPosition>());
	}

	// �G�̃e���g���[�f�[�^�\�z
	for (int i = 0; i < enemy_territory_load_data.data_count; ++i)
	{
		EnemyAppearancePositionReader* data = &((EnemyAppearancePositionReader*)&enemy_territory_load_data.buffer[sizeof(enemy_territory_load_data.headder) + enemy_territory_load_data.headder.string_length])[i];
		enemy_territory_data[i]->id = data->id;
		enemy_territory_data[i]->tag = data->tag;
		enemy_territory_data[i]->position_x = data->position_x;
		enemy_territory_data[i]->position_y = data->position_y;
		enemy_territory_data[i]->position_z = data->position_z;
		enemy_territory_data[i]->radius = data->radius;
	}

}

//-----------------------------------
// �����蔻��f�[�^�ǂݍ���
//-----------------------------------
void GameDataBase::LoadCollisionData()
{
	// �o�C�i���t�@�C������ǂݍ��ރf�[�^���쐬
	CreateData("Data/GameData/CollisionParameterData.dat", collision_parameter_load_data);

	// �����蔻��̃p�����[�^�f�[�^����
	for (int i = 0; i < collision_parameter_load_data.data_count; ++i)
	{
		collision_parameter_data.emplace_back(std::make_shared<CollisionParameterData>());
	}

	// �R���W�����p�����[�^�f�[�^�\�z
	for (int i = 0; i < collision_parameter_load_data.data_count; i++)
	{
		CollitionParameterReader* data = &((CollitionParameterReader*)&collision_parameter_load_data.buffer[sizeof(collision_parameter_load_data.headder) + collision_parameter_load_data.headder.string_length])[i];
		collision_parameter_data[i]->id = data->id;
		collision_parameter_data[i]->collision_name = &collision_parameter_load_data.text_buffer[data->collision_name];
		collision_parameter_data[i]->node_name = &collision_parameter_load_data.text_buffer[data->node_name];
		collision_parameter_data[i]->angle = data->angle;
		collision_parameter_data[i]->radius = data->radius;
		collision_parameter_data[i]->height = data->height;
		collision_parameter_data[i]->weight = data->weight;
		collision_parameter_data[i]->local_x = data->local_x;
		collision_parameter_data[i]->local_y = data->local_y;
		collision_parameter_data[i]->local_z = data->local_z;
		collision_parameter_data[i]->collision_flag = data->collision_flag;
		collision_parameter_data[i]->actor_type = data->actor_type;
		collision_parameter_data[i]->collision_update_type = data->collision_update_type;
		collision_parameter_data[i]->collision_type = data->collision_type;
		collision_parameter_data[i]->enemy_category = data->enemy_category;
	}

}

//-----------------------------------
// �J�����O�p�̓����蔻��f�[�^�ǂݍ���
//-----------------------------------
void GameDataBase::LoadCullingCollisionData()
{
	// �o�C�i���t�@�C������ǂݍ��ރf�[�^���쐬
	CreateData("Data/GameData/CullingCollisionParameterData.dat", culling_parameter_load_data);

	// �����蔻��̃p�����[�^�f�[�^����
	for (int i = 0; i < culling_parameter_load_data.data_count; ++i)
	{
		culling_parameter_data.emplace_back(std::make_shared<CullingCollisionParameterData>());
	}
	// �J�����O�R���W�����p�����[�^�f�[�^�\�z
	for (int i = 0; i < culling_parameter_load_data.data_count; i++)
	{
		CullingCollisionParameterDataReader* data = &((CullingCollisionParameterDataReader*)&culling_parameter_load_data.buffer[sizeof(culling_parameter_load_data.headder) + culling_parameter_load_data.headder.string_length])[i];
		culling_parameter_data[i]->id = data->id;
		culling_parameter_data[i]->collision_name = &culling_parameter_load_data.text_buffer[data->collision_name];
		culling_parameter_data[i]->node_name = &culling_parameter_load_data.text_buffer[data->node_name];
		culling_parameter_data[i]->angle = data->angle;
		culling_parameter_data[i]->radius_x = data->radius_x;
		culling_parameter_data[i]->radius_y = data->radius_y;
		culling_parameter_data[i]->radius_z = data->radius_z;
		culling_parameter_data[i]->local_x = data->local_x;
		culling_parameter_data[i]->local_y = data->local_y;
		culling_parameter_data[i]->local_z = data->local_z;
		culling_parameter_data[i]->update_type = data->update_type;
		culling_parameter_data[i]->enemy_category = data->enemy_category;
	}

}

//-----------------------------------
// �U���̓����蔻����s�����ԃf�[�^�ǂݍ���
//-----------------------------------
void GameDataBase::LoadCollisionTimeData()
{
	// �o�C�i���t�@�C������ǂݍ��ރf�[�^���쐬
	CreateData("Data/GameData/AttackCollitionTime.dat", collision_time_load_data);

	// �v���C���[�̔C�ӂ̃A�j���[�V������ԊJ�n���ԂƏI������(�����蔻��Ɏg��)�f�[�^����
	for (int i = 0; i < collision_time_load_data.data_count; ++i)
	{
		collision_time_data.emplace_back(std::make_shared<AttackCollitionTime>());
	}

	// �v���C���[�̔C�ӂ̃A�j���[�V������ԊJ�n���ԂƏI������(�����蔻��Ɏg��)�f�[�^�\�z
	for (int i = 0; i < collision_time_load_data.data_count; ++i)
	{
		AttackCollitionTimeReader* data = &((AttackCollitionTimeReader*)&collision_time_load_data.buffer[sizeof(collision_time_load_data.buffer) + collision_time_load_data.headder.string_length])[i];
		collision_time_data[i]->id = data->id;
		collision_time_data[i]->attack_category = data->attack_category;
		collision_time_data[i]->attacker_category = data->attacker_category;
		collision_time_data[i]->start_time = data->start_time;
		collision_time_data[i]->end_time = data->end_time;
	}

}


//-----------------------------------
// �o�C�i���t�@�C������A�j���[�V�����f�[�^�ǂݍ���
//-----------------------------------
void GameDataBase::LoadAnimationData()
{
	// �o�C�i���t�@�C������ǂݍ��ރf�[�^���쐬
	CreateData("Data/GameData/AnimationData.dat", animation_load_data);

	// �A�j���[�V�����f�[�^�\�z
	for (int i = 0; i < animation_load_data.data_count; ++i)
	{
		AnimationDataReader* data = &((AnimationDataReader*)&animation_load_data.buffer[sizeof(animation_load_data.headder) + animation_load_data.headder.string_length])[i];
		std::shared_ptr<AnimationData> animation = std::make_unique<AnimationData>();
		animation->id = data->id;
		animation->animation_name = &animation_load_data.text_buffer[data->animation_name];
		animation->number = data->number;
		animation->roop_flag = data->roop_flag;
		animation->blend = data->blend;
		// �}�b�v�ɒǉ�
		animation_data[animation->animation_name] = animation;
	}

}

//--------------------------------------------------
// �G���J�E���g�����G�̃e���g���[���̓G�̃f�[�^���W�߂�
//--------------------------------------------------
void GameDataBase::EnemyFriendFromTerritory(EnemyTerritoryTag territory_tag)
{
	// �G�}�l�[�W���[�擾
	EnemyManager& enemy_manager = EnemyManager::Instance();
	// ���łɓo�^����Ă�����
	if (encount_enemy.size() > 0)
	{
		// �N���A����
		encount_enemy.clear();
	}
	// �G�̐����擾
	int enemy_count = enemy_manager.GetEnemyCount();

	for (int i = 0; i < enemy_count; ++i)
	{
		std::shared_ptr<Enemy> enemy = enemy_manager.GetEnemy(i);
		// �����e���g���[���G���J�E���g�����G�̃e���g���[�Ɠ����Ȃ�
		if (enemy->GetBelongingToTerritory() == territory_tag)
		{
			// �G���J�E���g�����G�̃��X�g�ɒǉ�����
			EncountEnemyTerritory enemy_data;
			enemy_data.id = enemy->GetEnemyDataID();
			enemy_data.tag = territory_tag;
			encount_enemy.emplace_back(enemy_data);
		}
	}
}

//---------------------------------------------------------------------------------
// �X�e�[�W�I�u�W�F�N�g�f�[�^�擾
//---------------------------------------------------------------------------------
std::shared_ptr<StageObjectData> GameDataBase::GetButtleMapObjectData(const ActorType& category) const
{
	// �X�e�[�W�I�u�W�F�N�g�̃f�[�^����
	for (std::shared_ptr<StageObjectData> data : stage_object_data)
	{
		// �����̃J�e�S���[�Ɠ����Ȃ炻�̃f�[�^��Ԃ�
		if (data->object_category == category) return data;
	}

	return nullptr;
}

//---------------------------------------------------------------------------------
// �J�e�S���[���Ƃ̃p�����[�^�f�[�^�𒊏o�����f�[�^��n��
//---------------------------------------------------------------------------------
std::vector<std::shared_ptr<CollisionParameterData>> GameDataBase::GetAttackCollitionParamterDataList(ActorType enemy_category) const
{
	std::vector<std::shared_ptr<CollisionParameterData>> collision_arameter;
	// �����A�N�^�[�e�S���[�����f�[�^��ǉ����Ĕz����쐬����
	for (std::shared_ptr<CollisionParameterData> data : collision_parameter_data)
	{
		if (data->enemy_category == enemy_category)
		{
			collision_arameter.emplace_back(data);
		}
	}
	return collision_arameter;
}

//---------------------------------------------------------------------------------
// �J�e�S���[����J�����O�R���W�����p�����[�^�f�[�^���f�[�^��n��
//---------------------------------------------------------------------------------
std::vector<std::shared_ptr<CullingCollisionParameterData>> GameDataBase::GetAttackCullingCollisionParameterDataList(ActorType enemy_category) const
{
	std::vector<std::shared_ptr<CullingCollisionParameterData>> culling_arameter;
	// �����A�N�^�[�e�S���[�����f�[�^��ǉ����Ĕz����쐬����
	for (std::shared_ptr<CullingCollisionParameterData> data : culling_parameter_data)
	{
		if (data->enemy_category == enemy_category)
		{
			culling_arameter.emplace_back(data);
		}
	}
	return culling_arameter;
}

//---------------------------------------------------------------------------------
// �U���҂̃J�e�S���[�ƍU���J�e�S���[���瓖���蔻����s���A�j���[�V������Ԃ��擾
//---------------------------------------------------------------------------------
std::shared_ptr<AttackCollitionTime> GameDataBase::GetAttackCollitionTimeData(AttackCategory attack_category, ActorType attacker_category) const
{
	// �����U���J�e�S���[�����f�[�^��Ԃ�
	for (std::shared_ptr<AttackCollitionTime> data : collision_time_data)
	{
		if (data->attack_category == attack_category &&
			data->attacker_category == attacker_category)
		{
			return data;
		}
	}
	return nullptr;
}

//-----------------------------------
// �R���W�������s�����ԃf�[�^�擾
//-----------------------------------
std::shared_ptr<AttackCollitionTime> GameDataBase::GetAttackCollitionTimeData(AttackCategory attack_category) const
{
	// �����U���J�e�S���[�����f�[�^��Ԃ�
	for (std::shared_ptr<AttackCollitionTime> data : collision_time_data)
	{
		if (data->attack_category == attack_category)
		{
			return data;
		}
	}
	return nullptr;
}

//-----------------------------------
// �G�f�[�^ID����G�f�[�^�擾
//-----------------------------------
std::shared_ptr<EnemyData> GameDataBase::GetEnemyDataFromID(int& enemy_id) const
{
	for (std::shared_ptr<EnemyData> data : enemy_data)
	{
		if (data->id == enemy_id)
		{
			return data;
		}
	}
	return nullptr;
}

//-----------------------------------
// �e���g���[�̃^�O����e���g���[�f�[�^���擾����
//-----------------------------------
std::shared_ptr<EnemyTerritoryPosition> GameDataBase::GetEnemyTerritoryData(EnemyTerritoryTag& tag) const
{
	// �����^�O�����f�[�^��Ԃ�
	for (std::shared_ptr<EnemyTerritoryPosition> data : enemy_territory_data)
	{
		if (data->tag == tag)
		{
			return data;
		}
	}
	return nullptr;
}

//-----------------------------------
// �A�j���[�V�����f�[�^�擾
//-----------------------------------
std::shared_ptr<AnimationData> GameDataBase::GetAnimationData(std::string animation_name)
{
	// �����A�j���[�V�����̖��O�����f�[�^��Ԃ�
	AnimationMap::iterator it = animation_data.find(animation_name);
	if (it != animation_data.end())
	{
		return it->second;
	}
	return nullptr;
}
