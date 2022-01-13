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
	EnemyCategory	attacker_category;	//!<	�U���҂̃J�e�S��
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
	// ���[���h�f�[�^���X�g�擾
	std::vector<std::shared_ptr<WorldMapData>> GetWorldMapDataList() const { return world_map_data; }
	// ���[���h�f�[�^�擾
	std::shared_ptr<WorldMapData> GetWorldMapData(int& index) const { return world_map_data.at(index); }
	// ���[���h�f�[�^���X�g�̃f�[�^�����擾
	const	int GetWorldMapDataCount() { return world_map_data_count; }

	// �G�f�[�^���X�g�擾
	std::vector<std::shared_ptr<EnemyData>> GetEnemyDataList() const { return enemy_data; }
	// �G�f�[�^�擾
	std::shared_ptr<EnemyData> GetEnemyData(int& index) const { return enemy_data.at(index); }
	// ID����G�f�[�^�擾
	std::shared_ptr<EnemyData> GetEnemyDataFromID(int& enemy_id) const;
	// �G�f�[�^���X�g�̃f�[�^�����擾
	const	int GetEnemyDataCount() { return enemy_data_count; }

	// �G�̏o���ʒu���X�g�擾
	std::vector<std::shared_ptr<EnemyAppearancePosition>> GetEnemyAppearanceDataList() const { return enemy_appearance_data; }
	// �G�̏o���ʒu�擾
	std::shared_ptr<EnemyAppearancePosition> GetEnemyAppearanceData(int& index) const { return enemy_appearance_data.at(index); }
	// �e���g���[�^�O����G�̏o���ʒu�擾
	std::shared_ptr<EnemyAppearancePosition> GetEnemyAppearanceData(EnemyTerritoryTag& tag) const;
	// �G�̏o���ʒu���X�g�̃f�[�^�����擾
	const	int GetEnemyOccurCount() { return	enemy_appearance_data_count; }

	// �����蔻����s���A�j���[�V������ԃ��X�g���擾
	std::vector<std::shared_ptr<AttackCollitionTime>> GetAttackCollitionTimeDataList() const { return collision_time_data; }
	// �G�̃J�e�S���[���瓖���蔻����s���A�j���[�V������Ԃ��擾
	std::shared_ptr<AttackCollitionTime> GetAttackCollitionTimeData(AttackCategory attack_category, EnemyCategory attacker_category) const;
	// �����蔻����s���A�j���[�V������ԃ��X�g�̃f�[�^�����擾
	const	int GetCollitionTimeCount() { return	collision_time_data_count; }

	std::shared_ptr<AttackCollitionTime> GetAttackCollitionTimeData(AttackCategory attack_category) const;

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