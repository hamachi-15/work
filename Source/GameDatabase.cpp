#include "GameDatabase.h"
#include <iostream>
#include <fstream>

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
	char* enemy_data_buffer = LoadBuffer("Data/GameData/EnemyData.dat");
	char* enemy_occur_data_buffer = LoadBuffer("Data/GameData/EnemyOccurPosition.dat");
	char* collision_time_buffer = LoadBuffer("Data/GameData/AttackCollitionTime.dat");
	char* animation_data_buffer = LoadBuffer("Data/GameData/AnimationData.dat");

	// �f�[�^�w�b�_�ǂݍ���
	DataHeadder	enemy_data_headder;
	memcpy_s(&enemy_data_headder, sizeof(enemy_data_headder), enemy_data_buffer, sizeof(enemy_data_headder));
	DataHeadder	enemy_occur_data_headder;
	memcpy_s(&enemy_occur_data_headder, sizeof(enemy_occur_data_headder), enemy_occur_data_buffer, sizeof(enemy_occur_data_buffer));
	DataHeadder	collision_time_data_headder;
	memcpy_s(&collision_time_data_headder, sizeof(collision_time_data_headder), collision_time_buffer, sizeof(collision_time_buffer));
	DataHeadder aniamtion_data_headder;
	memcpy_s(&aniamtion_data_headder, sizeof(aniamtion_data_headder), animation_data_buffer, sizeof(animation_data_buffer));


	// �f�[�^���ݒ�
	enemy_data_count = enemy_data_headder.data_count;
	enemy_occur_data_count = enemy_occur_data_headder.data_count;
	collision_time_data_count = collision_time_data_headder.data_count;
	animation_data_count = aniamtion_data_headder.data_count;

	// �f�[�^����
	// �G�̊�b�X�e�[�^�X
	for (int i = 0; i < enemy_data_count; ++i)
	{
		enemy_data.emplace_back(std::make_unique<EnemyData>());
	}
	// �G�̏o���|�C���g
	for (int i = 0; i < enemy_occur_data_count; ++i)
	{
		enemy_occur_data.emplace_back(std::make_unique<EnemyOccurPosition>());
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
	enemy_occur_data_text_buffer = new char[enemy_occur_data_headder.string_length];
	animation_data_text_buffer = new char[aniamtion_data_headder.string_length];

	// �e�L�X�g�o�b�t�@�ǂݍ���
	memcpy_s(enemy_data_text_buffer, enemy_data_headder.string_length, &enemy_data_buffer[sizeof(enemy_data_headder)], enemy_data_headder.string_length);
	memcpy_s(enemy_occur_data_text_buffer, enemy_occur_data_headder.string_length, &enemy_occur_data_buffer[sizeof(enemy_occur_data_headder)], enemy_occur_data_headder.string_length);
	memcpy_s(animation_data_text_buffer, aniamtion_data_headder.string_length, &animation_data_buffer[sizeof(aniamtion_data_headder)], aniamtion_data_headder.string_length);

	// �f�[�^�\�z
	// �G�̏o���|�C���g
	for (int i = 0; i < enemy_occur_data_count; ++i)
	{
		EnemyOccurPositionReader* data = &((EnemyOccurPositionReader*)&enemy_occur_data_buffer[sizeof(enemy_occur_data_headder) + enemy_occur_data_headder.string_length])[i];
		enemy_occur_data[i]->position_x = data->position_x;
		enemy_occur_data[i]->position_y = data->position_y;
		enemy_occur_data[i]->position_z = data->position_z;
		enemy_occur_data[i]->radius = data->radius;
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
		collision_time_data[i]->start_time = data->start_time;
		collision_time_data[i]->end_time = data->end_time;
	}

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
	delete[] enemy_data_buffer;
	delete[] enemy_occur_data_buffer;
	delete[] collision_time_buffer;
	delete[] animation_data_buffer;
}

//-----------------------------------
// �f�X�g���N�^
//-----------------------------------
GameDataBase::~GameDataBase()
{
	delete[] enemy_data_text_buffer;
	delete[] enemy_occur_data_text_buffer;
	delete[] animation_data_text_buffer;
}

//-----------------------------------
// �R���W�������s�����Ԏ擾
//-----------------------------------
std::shared_ptr<AttackCollitionTime> GameDataBase::GetAttackCollitionTimeData(AttackCategory category) const
{
	for (std::shared_ptr<AttackCollitionTime> data : collision_time_data)
	{
		if (data->attack_category == category)
		{
			return data;
		}
	}
	return nullptr;
}

// �A�j���[�V�����f�[�^�擾
std::shared_ptr<AnimationData> GameDataBase::GetAnimationData(std::string animation_name)
{
	AnimationMap::iterator it = animation_data.find(animation_name);
	if (it != animation_data.end())
	{
		return it->second;
	}
	return nullptr;
}
