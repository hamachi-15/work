#pragma once
#include <string>
#include<vector>
#include <memory>
#include <map>
#include "EnemyData.h"
#include "EnemyAppearancePosition.h"
#include "AttackCollitionTime.h"
#include "AnimationData.h"
#include "WorldMapData.h"

struct DataHeadder
{
	int data_count;
	int string_length;
};

struct WorldMapDataReader
{
	int id;
	int enemy_id;
	EnemyTerritoryTag tag;
};

struct EnemyDataReader
{
	int id;
	int name;
	EnemyCategory category;
	int path;
	int node_name;
	float scale_x;
	float scale_y;
	float scale_z;
	float angle_x;
	float angle_y;
	float angle_z;
	int hp;
	int deffence;
	int attack;
	float speed;
	float speed_sclae;
};

struct EnemyAppearancePositionReader
{
	int id;
	EnemyTerritoryTag tag;
	float position_x;
	float position_y;
	float position_z;
	float radius;
};

struct AttackCollitionTimeReader
{
	int	id;	//!<	Id
	AttackCategory	attack_category;	//!<	�J�e�S���[
	float	start_time;	//!<	�����蔻��J�n����
	float	end_time;	//!<	�����蔻��I������

};

struct AnimationDataReader
{
	int	id;	//!<	ID
	int animation_name;	//!<	�A�j���[�V����
	int	number;	//!<	�ԍ�
	int	roop_flag;	//!<	���[�v�Đ�
	float	blend;	//!<	�A�j���[�V�����J�ڎ��̃u�����h��
};

class GameDataBase
{
private:
	GameDataBase();
	~GameDataBase();
public:
	// �C���X�^���X�擾
	static GameDataBase& Instance()
	{
		static GameDataBase instance;
		return instance;
	}
	// ���[���h�f�[�^�擾
	std::vector<std::shared_ptr<WorldMapData>> GetWorldMapDataList() const { return world_map_data; }
	std::shared_ptr<WorldMapData> GetWorldMapData(int& index) const { return world_map_data.at(index); }
	const	int GetWorldMapDataCount() { return world_map_data_count; }

	// �G�f�[�^�擾
	std::vector<std::shared_ptr<EnemyData>> GetEnemyDataList() const { return enemy_data; }
	std::shared_ptr<EnemyData> GetEnemyData(int& index) const { return enemy_data.at(index); }
	std::shared_ptr<EnemyData> GetEnemyDataFromID(int& enemy_id) const;
	const	int GetEnemyDataCount() { return enemy_data_count; }

	std::vector<std::shared_ptr<EnemyAppearancePosition>> GetEnemyAppearanceDataList() const { return enemy_appearance_data; }
	std::shared_ptr<EnemyAppearancePosition> GetEnemyAppearanceData(int& index) const { return enemy_appearance_data.at(index); }
	std::shared_ptr<EnemyAppearancePosition> GetEnemyAppearanceData(EnemyTerritoryTag& tag) const;

	const	int GetEnemyOccurCount() { return	enemy_appearance_data_count; }

	std::vector<std::shared_ptr<AttackCollitionTime>> GetAttackCollitionTimeData() const { return collision_time_data; }
	const	int GetCollitionTimeCount() { return	collision_time_data_count; }

	std::shared_ptr<AttackCollitionTime> GetAttackCollitionTimeData(AttackCategory category) const;

	// �A�j���[�V�����f�[�^�擾
	std::shared_ptr<AnimationData> GetAnimationData(std::string animation_name);
private:
	std::vector<std::shared_ptr<WorldMapData>> world_map_data;
	std::vector<std::shared_ptr<EnemyData>> enemy_data;
	std::vector<std::shared_ptr<EnemyAppearancePosition>> enemy_appearance_data;
	std::vector<std::shared_ptr<AttackCollitionTime>> collision_time_data;
	using AnimationMap = std::map<std::string, std::shared_ptr<AnimationData>>;
	AnimationMap animation_data;

	int world_map_data_count;

	int enemy_data_count;
	char* enemy_data_text_buffer;

	int enemy_appearance_data_count;
	char* enemy_appearance_data_text_buffer;

	int collision_time_data_count;

	char* animation_data_text_buffer;
	int animation_data_count;
};