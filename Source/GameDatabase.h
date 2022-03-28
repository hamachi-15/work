#pragma once
#include <string>
#include<vector>
#include <memory>
#include <map>

// �O���錾
enum class ActorType;
enum class ActorCategory;
enum class CollisionMeshType;
enum class CollisionUpdateType;
enum class AttackCategory;
enum class EnemyTerritoryTag;
struct StageObjectData;
struct WorldMapObjectPositionData;
struct ButtleMapObjectPositionData;
struct EnemyData;
struct EnemyTerritoryPosition;
struct CollisionParameterData;
struct CullingCollisionParameterData;
struct AttackCollitionTime;
struct WorldMapData;
struct AnimationData;
class Actor;
class Texture;

struct StageObjectDataReader
{
	int	id;	//!<	id
	int object_name;	//!<	���O
	int model_path;	//!<	���f���p�X
	ActorType	object_category;	//!<	�I�u�W�F�N�g�̎��
	float	scale_x;	//!<	�傫��(X)
	float	scale_y;	//!<	�傫��(Y)
	float	scale_z;	//!<	�傫��(Z)
	float	angle_x;	//!<	�p�x(X)
	float	angle_y;	//!<	�p�x(Y)
	float	angle_z;	//!<	�p�x(Z)
};

struct WorldMapObjectPositionDataReader
{
	int	id;	//!<	id
	int name;	//!<	���O
	ActorType	object_category;	//!<	�I�u�W�F�N�g�̎��
	float	position_x;	//!<	���W(X)
	float	position_y;	//!<	���W(Y)
	float	position_z;	//!<	���W(Z)
};

struct ButtleMapObjectPositionDataReader
{
	int	id;	//!<	id
	int name;	//!<	���O
	ActorType	object_category;	//!<	�I�u�W�F�N�g�̎��
	float	position_x;	//!<	���W(X)
	float	position_y;	//!<	���W(Y)
	float	position_z;	//!<	���W(Z)
};

struct DataHeadder
{
	int data_count;
	int string_length;
};

struct LoadData
{
	DataHeadder headder;
	char* buffer = nullptr;
	int data_count = 0;
	char* text_buffer = nullptr;
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
	ActorType category;
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
	ActorType	enemy_category;	//!<	�A�N�^�[
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
	ActorCategory	actor_type;	//!<	��������A�N�^�[�̎��
	CollisionUpdateType	collision_update_type;	//!<	�X�V���@�̎��
};

struct	CullingCollisionParameterDataReader
{
	int	id;	//!<	id
	ActorType	enemy_category;	//!<	�A�N�^�[�̎��
	int collision_name;	//!<	���O
	int node_name;	//!<	�m�[�h�̖��O
	float	angle;	//!<	�A���O��
	float	radius_x;	//!<	���aX
	float	radius_y;	//!<	���aY
	float	radius_z;	//!<	���aZ
	float	local_x;	//!<	���[�J��X���W
	float	local_y;	//!<	���[�J��Y���W
	float	local_z;	//!<	���[�J��Z���W
	CollisionUpdateType	update_type;	//!<	�X�V���@�̎��
};

struct AttackCollitionTimeReader
{
	int	id;	//!<	Id
	AttackCategory	attack_category;	//!<	�J�e�S���[
	ActorType	attacker_category;	//!<	�U���҂̃J�e�S��
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
	// �ǂݍ��ރf�[�^�̔j��
	/// ����
	/// �f�[�^�̐��ƕ�����̒������i�[�������
	void DeleteLoadData(LoadData& data);

	// �ǂݍ��ރf�[�^���쐬����
	/// ����
	/// LoadData�F�f�[�^�̐��ƕ�����̒������i�[�������
	void CreateData(const char* filename, LoadData& data);

	// �X�e�[�W�I�u�W�F�N�g�f�[�^�ǂݍ���
	void LoadStageObjectData();

	// ���[���h�}�b�v�̃I�u�W�F�N�g�f�[�^�ǂݍ���
	void LoadWorldMapObjectData();

	// �o�g���}�b�v�̃I�u�W�F�N�g�f�[�^�ǂݍ���
	void LoadButtleMapObjectData();

	// �G�f�[�^�ǂݍ���
	void LoadEnemyData();

	// �o�C�i���t�@�C������G�̔z�u�f�[�^�ǂݍ���
	void LoadEnemySetPositionData();

	// �G�̃e���g���[�f�[�^�ǂݍ���
	void LoadEnemyTerritoryData();

	// �����蔻��f�[�^�ǂݍ���
	void LoadCollisionData();

	// �J�����O�p�̓����蔻��f�[�^�ǂݍ���
	void LoadCullingCollisionData();

	// �U���̓����蔻����s�����ԃf�[�^�ǂݍ���
	void LoadCollisionTimeData();

	// �o�C�i���t�@�C������A�j���[�V�����f�[�^�ǂݍ���
	void LoadAnimationData();

	// �G���J�E���g�����G�̃e���g���[���̓G�̃f�[�^���W�߂�
	void EnemyFriendFromTerritory(EnemyTerritoryTag territory_tag);

	// �X�e�[�W�I�u�W�F�N�g�f�[�^���X�g�擾
	std::vector<std::shared_ptr<StageObjectData>> GetButtleMapObjectDataList() const { return stage_object_data; }

	// ���[���h�̃X�e�[�W�I�u�W�F�N�g�f�[�^�擾
	std::shared_ptr<StageObjectData> GetButtleMapObjectData(int& index) const { return stage_object_data.at(index); }

	// ���[���h�̃X�e�[�W�I�u�W�F�N�g�f�[�^�擾
	std::shared_ptr<StageObjectData> GetButtleMapObjectData(const ActorType& category) const;

	// �X�e�[�W�I�u�W�F�N�g�f�[�^���X�g�̃f�[�^�����擾
	const int GetButtleMapObjectDataCount() { return stage_object_load_data.data_count; }

	// ���[���h�X�e�[�W�I�u�W�F�N�g�f�[�^���X�g�擾
	std::vector<std::shared_ptr<WorldMapObjectPositionData>> GetWorldMapObjectPositionDataList() const { return world_map_object_position_data; }

	// ���[���h�X�e�[�W�I�u�W�F�N�g�f�[�^�擾
	std::shared_ptr<WorldMapObjectPositionData> GetWorldMapObjectPositionData(int& index) const { return world_map_object_position_data.at(index); }

	// �X�e�[�W�I�u�W�F�N�g�f�[�^���X�g�̃f�[�^�����擾
	const int GetWorldMapObjectPositionDataCount() { return world_map_object_load_data.data_count; }

	// �o�g���X�e�[�W�I�u�W�F�N�g�f�[�^���X�g�擾
	std::vector<std::shared_ptr<ButtleMapObjectPositionData>> GetButtleMapObjectPositionDataList() const { return buttle_map_object_position_data; }

	// �o�g���X�e�[�W�I�u�W�F�N�g�f�[�^�擾
	std::shared_ptr<ButtleMapObjectPositionData> GetButtleMapObjectPositionData(int& index) const { return buttle_map_object_position_data.at(index); }

	// �X�e�[�W�I�u�W�F�N�g�f�[�^���X�g�̃f�[�^�����擾
	const int GetButtleMapObjectPositionDataCount() { return buttle_map_object_load_data.data_count; }

	// ���[���h�f�[�^���X�g�擾
	std::vector<std::shared_ptr<WorldMapData>> GetWorldMapDataList() const { return world_map_data; }
	
	// ���[���h�f�[�^�擾
	std::shared_ptr<WorldMapData> GetWorldMapData(int& index) const { return world_map_data.at(index); }
	
	// ���[���h�f�[�^���X�g�̃f�[�^�����擾
	const int GetWorldMapDataCount() { return world_map_object_load_data.data_count; }

	// �G�f�[�^���X�g�擾
	std::vector<std::shared_ptr<EnemyData>> GetEnemyDataList() const { return enemy_data; }
	
	// �G�f�[�^�擾
	std::shared_ptr<EnemyData> GetEnemyData(int& index) const { return enemy_data.at(index); }
	
	// ID����G�f�[�^�擾
	std::shared_ptr<EnemyData> GetEnemyDataFromID(int& enemy_id) const;
	
	// �G�f�[�^���X�g�̃f�[�^�����擾
	const int GetEnemyDataCount() { return enmy_load_data.data_count; }

	// �G�̏o���ʒu���X�g�擾
	std::vector<std::shared_ptr<EnemyTerritoryPosition>> GetEnemyTerritoryDataList() const { return enemy_territory_data; }
	
	// �G�̏o���ʒu�擾
	std::shared_ptr<EnemyTerritoryPosition> GetEnemyTerritoryData(int& index) const { return enemy_territory_data.at(index); }
	
	// �e���g���[�^�O����G�̏o���ʒu�擾
	std::shared_ptr<EnemyTerritoryPosition> GetEnemyTerritoryData(EnemyTerritoryTag& tag) const;
	
	// �G�̏o���ʒu���X�g�̃f�[�^�����擾
	const int GetEnemyTerritoryCount() { return	enemy_territory_load_data.data_count; }

	// �G���J�E���g�����G�f�[�^���X�g�擾
	std::vector<EncountEnemyTerritory> GetEncountEnemyList() { return encount_enemy; }

	// �R���W�����p�����[�^���X�g�擾
	std::vector<std::shared_ptr<CollisionParameterData>> GetAttackCollitionParamterDataList() const { return collision_parameter_data; }
	
	// �J�e�S���[���Ƃ̃p�����[�^�f�[�^�𒊏o�����f�[�^��n��
	std::vector<std::shared_ptr<CollisionParameterData>> GetAttackCollitionParamterDataList(ActorType enemy_category) const;

	// �J�����O�R���W�����p�����[�^���X�g�擾
	std::vector<std::shared_ptr<CullingCollisionParameterData>> GetAttackCullingCollisionParameterDataList() const { return culling_parameter_data; }

	// �J�e�S���[����J�����O�R���W�����p�����[�^�f�[�^���f�[�^��n��
	std::vector<std::shared_ptr<CullingCollisionParameterData>> GetAttackCullingCollisionParameterDataList(ActorType enemy_category) const;
	
	// �����蔻����s���A�j���[�V������ԃ��X�g���擾
	std::vector<std::shared_ptr<AttackCollitionTime>> GetAttackCollitionTimeDataList() const { return collision_time_data; }
	
	// �G�̃J�e�S���[�ƍU���҂��瓖���蔻����s���A�j���[�V������Ԃ��擾
	std::shared_ptr<AttackCollitionTime> GetAttackCollitionTimeData(AttackCategory attack_category, ActorType attacker_category) const;
	
	// �����蔻����s���A�j���[�V������ԃ��X�g�̃f�[�^�����擾
	const int GetCollitionTimeCount() { return	collision_time_load_data.data_count; }
	
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
	std::vector<std::shared_ptr<StageObjectData>> stage_object_data;
	std::vector<std::shared_ptr<ButtleMapObjectPositionData>> buttle_map_object_position_data;
	std::vector<std::shared_ptr<WorldMapObjectPositionData>> world_map_object_position_data;
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
	
	LoadData stage_object_load_data;
	LoadData world_map_object_load_data;
	LoadData buttle_map_object_load_data;
	LoadData enmy_load_data;
	LoadData enemy_position_load_data;
	LoadData enemy_territory_load_data;
	LoadData culling_parameter_load_data;
	LoadData collision_time_load_data;
	LoadData collision_parameter_load_data;
	LoadData animation_load_data;
};