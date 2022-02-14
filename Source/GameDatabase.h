#pragma once
#include <string>
#include<vector>
#include <memory>
#include <map>
#include "EnemyData.h"
#include "EnemyTerritoryPosition.h"
#include "ActorType.h"
#include "CollisionParameterData.h"
#include "CullingCollisionParameterData.h"
#include "CollisionMeshType.h"
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

struct CollitionParameterReader
{
	int	id;	//!<	ID
	EnemyCategory	enemy_category;	//!<	�A�N�^�[
	CollisionMeshType	collision_type;	//!<	�R���W�����̎��
	int collision_name;	//!<	�R���W�����̖��O
	int node_name;	//!<	�m�[�h�̖��O
	float	angle;	//!<	�A���O��
	float	radius;	//!<	���a
	float	height;	//!<	����
	float	weight;	//!<	�E�G�C�g
	float	local_x;	//!<	���[�J��X���W
	float	local_y;	//!<	���[�J��Y���W
	float	local_z;	//!<	���[�J��Z���W
	int	collision_flag;	//!<	�R���W�����t���O
	ActorType	actor_type;	//!<	��������A�N�^�[�̎��
	CollisionUpdateType	collision_update_type;	//!<	�X�V���@�̎��
};

struct	CullingCollisionParameterDataReader
{
	int	id;	//!<	id
	EnemyCategory	enemy_category;	//!<	�A�N�^�[�̎��
	int collision_name;	//!<	���O
	int node_name;	//!<	�m�[�h�̖��O
	float	angle;	//!<	�A���O��
	float	radius_x;	//!<	���aX
	float	radius_y;	//!<	���aY
	float	radius_z;	//!<	���aZ
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

struct EncountEnemyTerritory
{
	int id;
	EnemyTerritoryTag tag;
};

class Actor;
class Texture;

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

	// �G���J�E���g�����G�̃e���g���[���̓G�̃f�[�^���W�߂�
	void EnemyFriendFromTerritory(EnemyTerritoryTag territory_tag);

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
	std::vector<std::shared_ptr<EnemyTerritoryPosition>> GetEnemyTerritoryDataList() const { return enemy_territory_data; }
	
	// �G�̏o���ʒu�擾
	std::shared_ptr<EnemyTerritoryPosition> GetEnemyTerritoryData(int& index) const { return enemy_territory_data.at(index); }
	
	// �e���g���[�^�O����G�̏o���ʒu�擾
	std::shared_ptr<EnemyTerritoryPosition> GetEnemyTerritoryData(EnemyTerritoryTag& tag) const;
	
	// �G�̏o���ʒu���X�g�̃f�[�^�����擾
	const	int GetEnemyTerritoryCount() { return	enemy_territory_data_count; }

	// �G���J�E���g�����G�f�[�^���X�g�擾
	std::vector<EncountEnemyTerritory> GetEncountEnemyList() { return encount_enemy; }

	// �R���W�����p�����[�^���X�g�擾
	std::vector<std::shared_ptr<CollisionParameterData>> GetAttackCollitionParamterDataList() const { return collision_parameter_data; }
	
	// �J�e�S���[���Ƃ̃p�����[�^�f�[�^�𒊏o�����f�[�^��n��
	std::vector<std::shared_ptr<CollisionParameterData>> GetAttackCollitionParamterDataList(EnemyCategory enemy_category) const;

	// �R���W�����p�����[�^���X�g�擾
	std::vector<std::shared_ptr<CullingCollisionParameterData>> GetAttackCullingCollisionParameterDataList() const { return culling_parameter_data; }

	// �J�e�S���[���Ƃ̃p�����[�^�f�[�^�𒊏o�����f�[�^��n��
	std::vector<std::shared_ptr<CullingCollisionParameterData>> GetAttackCullingCollisionParameterDataList(EnemyCategory enemy_category) const;
	
	// �����蔻����s���A�j���[�V������ԃ��X�g���擾
	std::vector<std::shared_ptr<AttackCollitionTime>> GetAttackCollitionTimeDataList() const { return collision_time_data; }
	
	// �G�̃J�e�S���[�ƍU���҂��瓖���蔻����s���A�j���[�V������Ԃ��擾
	std::shared_ptr<AttackCollitionTime> GetAttackCollitionTimeData(AttackCategory attack_category, EnemyCategory attacker_category) const;
	
	// �����蔻����s���A�j���[�V������ԃ��X�g�̃f�[�^�����擾
	const	int GetCollitionTimeCount() { return	collision_time_data_count; }
	
	// �G�̃J�e�S���[���瓖���蔻����s���A�j���[�V������Ԃ��擾
	std::shared_ptr<AttackCollitionTime> GetAttackCollitionTimeData(AttackCategory attack_category) const;

	// �A�j���[�V�����f�[�^�擾
	std::shared_ptr<AnimationData> GetAnimationData(std::string animation_name);

	// �Q�[���I�[�o�[�E�N���A�̏u�Ԃ̃e�N�X�`���擾
	Texture* GetTimingTexture() { return timing_texture.get(); }
public:
	enum EnemyTerritoryDataIndex
	{
		Buttle_Territory_index = 6,	// �o�g���V�[���̃e���g���[�f�[�^���i�[����Ă���ŏ��̃C���f�b�N�X
		Buttle_Appearance_Position_Index = 7,	// �o�g���V�[���̏o���ʒu�f�[�^���i�[����Ă���ŏ��̃C���f�b�N�X
	};

private:
	std::vector<std::shared_ptr<WorldMapData>> world_map_data;
	std::vector<std::shared_ptr<EnemyData>> enemy_data;
	std::vector<std::shared_ptr<EnemyTerritoryPosition>> enemy_territory_data;
	std::vector<std::shared_ptr<AttackCollitionTime>> collision_time_data;
	std::vector<std::shared_ptr<CollisionParameterData>> collision_parameter_data;
	std::vector<std::shared_ptr<CullingCollisionParameterData>> culling_parameter_data;
	std::vector<EncountEnemyTerritory> encount_enemy;
	using AnimationMap = std::map<std::string, std::shared_ptr<AnimationData>>;
	AnimationMap animation_data;

	std::unique_ptr<Texture> timing_texture; // �Q�[���I�[�o�[�E�N���A�̏u�Ԃ̃e�N�X�`��


	int world_map_data_count;

	int enemy_data_count;
	char* enemy_data_text_buffer;

	int enemy_territory_data_count;
	char* enemy_territory_data_text_buffer;

	int culling_parameter_data_count;
	char* culling_parameter_data_text_buffer;
	
	int collision_parameter_data_count;
	char* collision_parameter_data_text_buffer;

	int collision_time_data_count;

	char* animation_data_text_buffer;
	int animation_data_count;
};