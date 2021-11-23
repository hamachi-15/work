#pragma once
#include <string>
#include<vector>
#include <memory>
#include <map>
#include "EnemyData.h"
#include "EnemyOccurPosition.h"
#include "AttackCollitionTime.h"
#include "AnimationData.h"
struct DataHeadder
{
	int data_count;
	int string_length;
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

struct EnemyOccurPositionReader
{
	int id;
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

	inline std::vector<std::shared_ptr<EnemyData>> GetEnemyData() const { return enemy_data; }
	inline	const	int GetEnemyDataCount() { return	enemy_data_count; }

	inline std::vector<std::shared_ptr<EnemyOccurPosition>> GetEnemyOccurData() const { return enemy_occur_data; }
	inline	const	int GetEnemyOccurCount() { return	enemy_occur_data_count; }

	inline std::vector<std::shared_ptr<AttackCollitionTime>> GetAttackCollitionTimeData() const { return collision_time_data; }
	inline	const	int GetCollitionTimeCount() { return	collision_time_data_count; }

	std::shared_ptr<AttackCollitionTime> GetAttackCollitionTimeData(AttackCategory category) const;

	// �A�j���[�V�����f�[�^�擾
	std::shared_ptr<AnimationData> GetAnimationData(std::string animation_name);
private:
	std::vector<std::shared_ptr<EnemyData>> enemy_data;
	std::vector<std::shared_ptr<EnemyOccurPosition>> enemy_occur_data;
	std::vector<std::shared_ptr<AttackCollitionTime>> collision_time_data;
	using AnimationMap = std::map<std::string, std::shared_ptr<AnimationData>>;
	AnimationMap animation_data;
	int enemy_data_count;
	char* enemy_data_text_buffer;
	int enemy_occur_data_count;
	char* enemy_occur_data_text_buffer;
	int collision_time_data_count;
	char* animation_data_text_buffer;
	int animation_data_count;
};