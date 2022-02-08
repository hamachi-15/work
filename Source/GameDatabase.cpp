#include "GameDatabase.h"
#include <iostream>
#include <fstream>
#include "EnemyManager.h"
#include "Actor.h"

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
	// �t�@�C���ǂݍ���
	char* world_map_data_buffer = LoadBuffer("Data/GameData/WorldMapData.dat");

	char* enemy_data_buffer = LoadBuffer("Data/GameData/EnemyData.dat");

	char* enemy_territory_data_buffer = LoadBuffer("Data/GameData/EnemyTerritoryPosition.dat");

	char* culling_parameter_data_buffer = LoadBuffer("Data/GameData/CullingCollisionParameter.dat");

	char* collision_parameter_data_buffer = LoadBuffer("Data/GameData/CollisionParameterData.dat");

	char* collision_time_buffer = LoadBuffer("Data/GameData/AttackCollitionTime.dat");

	char* animation_data_buffer = LoadBuffer("Data/GameData/AnimationData.dat");

	// �f�[�^�w�b�_�ǂݍ���
	DataHeadder	world_map_data_headder;
	memcpy_s(&world_map_data_headder, sizeof(world_map_data_headder), world_map_data_buffer, sizeof(world_map_data_headder));
	
	DataHeadder	enemy_data_headder;
	memcpy_s(&enemy_data_headder, sizeof(enemy_data_headder), enemy_data_buffer, sizeof(enemy_data_headder));
	
	DataHeadder	enemy_territory_data_headder;
	memcpy_s(&enemy_territory_data_headder, sizeof(enemy_territory_data_headder), enemy_territory_data_buffer, sizeof(enemy_territory_data_buffer));
	
	DataHeadder	collision_time_data_headder;
	memcpy_s(&collision_time_data_headder, sizeof(collision_time_data_headder), collision_time_buffer, sizeof(collision_time_buffer));
	
	DataHeadder	collision_parameter_data_headder;
	memcpy_s(&collision_parameter_data_headder, sizeof(collision_parameter_data_headder), collision_parameter_data_buffer, sizeof(collision_parameter_data_buffer));
	
	DataHeadder	culling_parameter_data_headder;
	memcpy_s(&culling_parameter_data_headder, sizeof(culling_parameter_data_headder), culling_parameter_data_buffer, sizeof(culling_parameter_data_buffer));
	
	DataHeadder aniamtion_data_headder;
	memcpy_s(&aniamtion_data_headder, sizeof(aniamtion_data_headder), animation_data_buffer, sizeof(animation_data_buffer));


	// �f�[�^���ݒ�
	world_map_data_count = world_map_data_headder.data_count;

	enemy_data_count = enemy_data_headder.data_count;

	enemy_territory_data_count = enemy_territory_data_headder.data_count;

	collision_time_data_count = collision_time_data_headder.data_count;

	collision_parameter_data_count = collision_parameter_data_headder.data_count;
	
	culling_parameter_data_count = culling_parameter_data_headder.data_count;

	animation_data_count = aniamtion_data_headder.data_count;

	// �f�[�^����
	// ���[���h�}�b�v�̓G�����f�[�^
	for (int i = 0; i < world_map_data_count; ++i)
	{
		world_map_data.emplace_back(std::make_unique<WorldMapData>());
	}

	// �G�̊�b�X�e�[�^�X
	for (int i = 0; i < enemy_data_count; ++i)
	{
		enemy_data.emplace_back(std::make_unique<EnemyData>());
	}

	// �G�̏o���|�C���g
	for (int i = 0; i < enemy_territory_data_count; ++i)
	{
		enemy_territory_data.emplace_back(std::make_unique<EnemyTerritoryPosition>());
	}

	// �����蔻��̃p�����[�^
	for (int i = 0; i < collision_parameter_data_count; ++i)
	{
		collision_parameter_data.emplace_back(std::make_unique<CollisionParameterData>());
	}

	// �����蔻��̃p�����[�^
	for (int i = 0; i < culling_parameter_data_count; ++i)
	{
		culling_parameter_data.emplace_back(std::make_unique<CullingCollisionParameterData>());
	}
	
	// �v���C���[�̔C�ӂ̃A�j���[�V������ԊJ�n���ԂƏI������(�����蔻��Ɏg��)
	for (int i = 0; i < collision_time_data_count; ++i)
	{
		collision_time_data.emplace_back(std::make_unique<AttackCollitionTime>());
	}

	// �A�j���[�V�����f�[�^
	//for (int i = 0; i < animation_data_count; ++i)
	//{
	//	animation_data.emplace_back(std::make_unique<AnimationData>());
	//}

	// �e�L�X�g�o�b�t�@����
	enemy_data_text_buffer = new char[enemy_data_headder.string_length];
	enemy_territory_data_text_buffer = new char[enemy_territory_data_headder.string_length];
	animation_data_text_buffer = new char[aniamtion_data_headder.string_length];
	collision_parameter_data_text_buffer = new char[collision_parameter_data_headder.string_length];
	culling_parameter_data_text_buffer = new char[culling_parameter_data_headder.string_length];
	
	// �e�L�X�g�o�b�t�@�ǂݍ���
	memcpy_s(enemy_data_text_buffer, enemy_data_headder.string_length, &enemy_data_buffer[sizeof(enemy_data_headder)], enemy_data_headder.string_length);
	memcpy_s(enemy_territory_data_text_buffer, enemy_territory_data_headder.string_length, &enemy_territory_data_buffer[sizeof(enemy_territory_data_headder)], enemy_territory_data_headder.string_length);
	memcpy_s(animation_data_text_buffer, aniamtion_data_headder.string_length, &animation_data_buffer[sizeof(aniamtion_data_headder)], aniamtion_data_headder.string_length);
	memcpy_s(collision_parameter_data_text_buffer, collision_parameter_data_headder.string_length, &collision_parameter_data_buffer[sizeof(collision_parameter_data_headder)], collision_parameter_data_headder.string_length);
	memcpy_s(culling_parameter_data_text_buffer, culling_parameter_data_headder.string_length, &culling_parameter_data_buffer[sizeof(culling_parameter_data_headder)], culling_parameter_data_headder.string_length);

	// �f�[�^�\�z
	// ���[���h�}�b�v�̓G�����f�[�^
	for (int i = 0; i < world_map_data_count; ++i)
	{
		WorldMapDataReader* data = &((WorldMapDataReader*)&world_map_data_buffer[sizeof(world_map_data_headder) + world_map_data_headder.string_length])[i];
		world_map_data[i]->id = data->id;
		world_map_data[i]->enemy_id = data->enemy_id;
		world_map_data[i]->tag = data->tag;
	}

	// �G�̏o���|�C���g
	for (int i = 0; i < enemy_territory_data_count; ++i)
	{
		EnemyAppearancePositionReader* data = &((EnemyAppearancePositionReader*)&enemy_territory_data_buffer[sizeof(enemy_territory_data_headder) + enemy_territory_data_headder.string_length])[i];
		enemy_territory_data[i]->id = data->id;
		enemy_territory_data[i]->tag = data->tag;
		enemy_territory_data[i]->position_x = data->position_x;
		enemy_territory_data[i]->position_y = data->position_y;
		enemy_territory_data[i]->position_z = data->position_z;
		enemy_territory_data[i]->radius = data->radius;
	}
	
	// �G�̊�b�X�e�[�^�X
	for (int i = 0; i < enemy_data_count; ++i)
	{
		EnemyDataReader* data = &((EnemyDataReader*)&enemy_data_buffer[sizeof(enemy_data_headder) + enemy_data_headder.string_length])[i];
		enemy_data[i]->id = data->id;
		enemy_data[i]->name = &enemy_data_text_buffer[data->name];
		enemy_data[i]->category = data->category;
		enemy_data[i]->model_path = &enemy_data_text_buffer[data->path];
		enemy_data[i]->animation_node_name = &enemy_data_text_buffer[data->node_name];
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

	// �v���C���[�̔C�ӂ̃A�j���[�V������ԊJ�n���ԂƏI������(�����蔻��Ɏg��)
	for (int i = 0; i < collision_time_data_count; ++i)
	{
		AttackCollitionTimeReader* data = &((AttackCollitionTimeReader*)&collision_time_buffer[sizeof(collision_time_data_headder) + collision_time_data_headder.string_length])[i];
		collision_time_data[i]->id = data->id;
		collision_time_data[i]->attack_category = data->attack_category;
		collision_time_data[i]->attacker_category = data->attacker_category;
		collision_time_data[i]->start_time = data->start_time;
		collision_time_data[i]->end_time = data->end_time;
	}

	// �R���W�����p�����[�^�ݒ�
	for (int i = 0; i < collision_parameter_data_count; i++)
	{
		CollitionParameterReader* data = &((CollitionParameterReader*)&collision_parameter_data_buffer[sizeof(collision_parameter_data_headder) + collision_parameter_data_headder.string_length])[i];
		collision_parameter_data[i]->id = data->id;
		collision_parameter_data[i]->collision_name = &collision_parameter_data_text_buffer[data->collision_name];
		collision_parameter_data[i]->node_name = &collision_parameter_data_text_buffer[data->node_name];
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
		collision_parameter_data[i]->enemy_category  = data->enemy_category;
	}

	// �J�����O�R���W�����p�����[�^�ݒ�
	for (int i = 0; i < culling_parameter_data_count; i++)
	{
		CullingCollisionParameterDataReader* data = &((CullingCollisionParameterDataReader*)& culling_parameter_data_buffer[sizeof(culling_parameter_data_headder) + culling_parameter_data_headder.string_length])[i];
		culling_parameter_data[i]->id = data->id;
		culling_parameter_data[i]->collision_name = &culling_parameter_data_text_buffer[data->collision_name];
		culling_parameter_data[i]->node_name = &culling_parameter_data_text_buffer[data->node_name];
		culling_parameter_data[i]->angle = data->angle;
		culling_parameter_data[i]->radius_x = data->radius_x;
		culling_parameter_data[i]->radius_y = data->radius_y;
		culling_parameter_data[i]->radius_z = data->radius_z;
		culling_parameter_data[i]->enemy_category = data->enemy_category;
	}

	// �A�j���[�V�����f�[�^�ݒ�
	for (int i = 0; i < animation_data_count; ++i)
	{
		AnimationDataReader* data = &((AnimationDataReader*)&animation_data_buffer[sizeof(aniamtion_data_headder) + aniamtion_data_headder.string_length])[i];
		std::shared_ptr<AnimationData> animation = std::make_unique<AnimationData>();
		animation->id = data->id;
		animation->animation_name = &animation_data_text_buffer[data->animation_name];
		animation->number = data->number;
		animation->roop_flag = data->roop_flag;
		animation->blend = data->blend;
		// �}�b�v�ɒǉ�
		animation_data[animation->animation_name] = animation;
	}
	
	// �t�@�C���̍폜
	delete[] world_map_data_buffer;
	delete[] enemy_data_buffer;
	delete[] enemy_territory_data_buffer;
	delete[] collision_time_buffer;
	delete[] animation_data_buffer;
	delete[] collision_parameter_data_buffer;
	delete[] culling_parameter_data_buffer;
}

//-----------------------------------
// �f�X�g���N�^
//-----------------------------------
GameDataBase::~GameDataBase()
{
	delete[] enemy_data_text_buffer;
	delete[] enemy_territory_data_text_buffer;
	delete[] animation_data_text_buffer;
	delete[] collision_parameter_data_text_buffer;
	delete[] culling_parameter_data_text_buffer;
}

//---------------------------------------------------------------------------------
// �G���J�E���g�����G�̃e���g���[���̓G�̃f�[�^���W�߂�
//---------------------------------------------------------------------------------
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
			EncountEnemyTerritory enemy_data;
			enemy_data.id = enemy->GetEnemyDataID();
			encount_enemy.emplace_back(enemy_data);
		}
	}
}

//---------------------------------------------------------------------------------
// �J�e�S���[���Ƃ̃p�����[�^�f�[�^�𒊏o�����f�[�^��n��
//---------------------------------------------------------------------------------
std::vector<std::shared_ptr<CollisionParameterData>> GameDataBase::GetAttackCollitionParamterDataList(EnemyCategory enemy_category) const
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
// �J�e�S���[���Ƃ̃p�����[�^�f�[�^�𒊏o�����f�[�^��n��
//---------------------------------------------------------------------------------
std::vector<std::shared_ptr<CullingCollisionParameterData>> GameDataBase::GetAttackCullingCollisionParameterDataList(EnemyCategory enemy_category) const
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
std::shared_ptr<AttackCollitionTime> GameDataBase::GetAttackCollitionTimeData(AttackCategory attack_category, EnemyCategory attacker_category) const
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
