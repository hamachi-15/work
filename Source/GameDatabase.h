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
	AttackCategory	attack_category;	//!<	カテゴリー
	EnemyCategory	attacker_category;	//!<	攻撃者のカテゴリ
	float	start_time;	//!<	当たり判定開始時間
	float	end_time;	//!<	当たり判定終了時間
};

struct AnimationDataReader
{
	int	id;	//!<	ID
	int animation_name;	//!<	アニメーション
	int	number;	//!<	番号
	int	roop_flag;	//!<	ループ再生
	float	blend;	//!<	アニメーション遷移時のブレンド率
};

class GameDataBase
{
private:
	GameDataBase();
	~GameDataBase();
public:
	// インスタンス取得
	static GameDataBase& Instance()
	{
		static GameDataBase instance;
		return instance;
	}
	// ワールドデータリスト取得
	std::vector<std::shared_ptr<WorldMapData>> GetWorldMapDataList() const { return world_map_data; }
	// ワールドデータ取得
	std::shared_ptr<WorldMapData> GetWorldMapData(int& index) const { return world_map_data.at(index); }
	// ワールドデータリストのデータ数を取得
	const	int GetWorldMapDataCount() { return world_map_data_count; }

	// 敵データリスト取得
	std::vector<std::shared_ptr<EnemyData>> GetEnemyDataList() const { return enemy_data; }
	// 敵データ取得
	std::shared_ptr<EnemyData> GetEnemyData(int& index) const { return enemy_data.at(index); }
	// IDから敵データ取得
	std::shared_ptr<EnemyData> GetEnemyDataFromID(int& enemy_id) const;
	// 敵データリストのデータ数を取得
	const	int GetEnemyDataCount() { return enemy_data_count; }

	// 敵の出現位置リスト取得
	std::vector<std::shared_ptr<EnemyAppearancePosition>> GetEnemyAppearanceDataList() const { return enemy_appearance_data; }
	// 敵の出現位置取得
	std::shared_ptr<EnemyAppearancePosition> GetEnemyAppearanceData(int& index) const { return enemy_appearance_data.at(index); }
	// テリトリータグから敵の出現位置取得
	std::shared_ptr<EnemyAppearancePosition> GetEnemyAppearanceData(EnemyTerritoryTag& tag) const;
	// 敵の出現位置リストのデータ数を取得
	const	int GetEnemyOccurCount() { return	enemy_appearance_data_count; }

	// 当たり判定を行うアニメーション区間リストを取得
	std::vector<std::shared_ptr<AttackCollitionTime>> GetAttackCollitionTimeDataList() const { return collision_time_data; }
	// 敵のカテゴリーから当たり判定を行うアニメーション区間を取得
	std::shared_ptr<AttackCollitionTime> GetAttackCollitionTimeData(AttackCategory attack_category, EnemyCategory attacker_category) const;
	// 当たり判定を行うアニメーション区間リストのデータ数を取得
	const	int GetCollitionTimeCount() { return	collision_time_data_count; }

	std::shared_ptr<AttackCollitionTime> GetAttackCollitionTimeData(AttackCategory attack_category) const;

	// アニメーションデータ取得
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