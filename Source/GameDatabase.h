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
	EnemyCategory	enemy_category;	//!<	アクター
	CollisionMeshType	collision_type;	//!<	コリジョンの種類
	int collision_name;	//!<	コリジョンの名前
	int node_name;	//!<	ノードの名前
	float	angle;	//!<	アングル
	float	radius;	//!<	半径
	float	height;	//!<	高さ
	float	weight;	//!<	ウエイト
	float	local_x;	//!<	ローカルX座標
	float	local_y;	//!<	ローカルY座標
	float	local_z;	//!<	ローカルZ座標
	int	collision_flag;	//!<	コリジョンフラグ
	ActorType	actor_type;	//!<	所持するアクターの種類
	CollisionUpdateType	collision_update_type;	//!<	更新方法の種類
};

struct	CullingCollisionParameterDataReader
{
	int	id;	//!<	id
	EnemyCategory	enemy_category;	//!<	アクターの種類
	int collision_name;	//!<	名前
	int node_name;	//!<	ノードの名前
	float	angle;	//!<	アングル
	float	radius_x;	//!<	半径X
	float	radius_y;	//!<	半径Y
	float	radius_z;	//!<	半径Z
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
	// インスタンス取得
	static GameDataBase& Instance()
	{
		static GameDataBase instance;
		return instance;
	}

	// エンカウントした敵のテリトリー内の敵のデータを集める
	void EnemyFriendFromTerritory(EnemyTerritoryTag territory_tag);

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
	std::vector<std::shared_ptr<EnemyTerritoryPosition>> GetEnemyTerritoryDataList() const { return enemy_territory_data; }
	
	// 敵の出現位置取得
	std::shared_ptr<EnemyTerritoryPosition> GetEnemyTerritoryData(int& index) const { return enemy_territory_data.at(index); }
	
	// テリトリータグから敵の出現位置取得
	std::shared_ptr<EnemyTerritoryPosition> GetEnemyTerritoryData(EnemyTerritoryTag& tag) const;
	
	// 敵の出現位置リストのデータ数を取得
	const	int GetEnemyTerritoryCount() { return	enemy_territory_data_count; }

	// エンカウントした敵データリスト取得
	std::vector<EncountEnemyTerritory> GetEncountEnemyList() { return encount_enemy; }

	// コリジョンパラメータリスト取得
	std::vector<std::shared_ptr<CollisionParameterData>> GetAttackCollitionParamterDataList() const { return collision_parameter_data; }
	
	// カテゴリーごとのパラメータデータを抽出したデータを渡す
	std::vector<std::shared_ptr<CollisionParameterData>> GetAttackCollitionParamterDataList(EnemyCategory enemy_category) const;

	// コリジョンパラメータリスト取得
	std::vector<std::shared_ptr<CullingCollisionParameterData>> GetAttackCullingCollisionParameterDataList() const { return culling_parameter_data; }

	// カテゴリーごとのパラメータデータを抽出したデータを渡す
	std::vector<std::shared_ptr<CullingCollisionParameterData>> GetAttackCullingCollisionParameterDataList(EnemyCategory enemy_category) const;
	
	// 当たり判定を行うアニメーション区間リストを取得
	std::vector<std::shared_ptr<AttackCollitionTime>> GetAttackCollitionTimeDataList() const { return collision_time_data; }
	
	// 敵のカテゴリーと攻撃者から当たり判定を行うアニメーション区間を取得
	std::shared_ptr<AttackCollitionTime> GetAttackCollitionTimeData(AttackCategory attack_category, EnemyCategory attacker_category) const;
	
	// 当たり判定を行うアニメーション区間リストのデータ数を取得
	const	int GetCollitionTimeCount() { return	collision_time_data_count; }
	
	// 敵のカテゴリーから当たり判定を行うアニメーション区間を取得
	std::shared_ptr<AttackCollitionTime> GetAttackCollitionTimeData(AttackCategory attack_category) const;

	// アニメーションデータ取得
	std::shared_ptr<AnimationData> GetAnimationData(std::string animation_name);

	// ゲームオーバー・クリアの瞬間のテクスチャ取得
	Texture* GetTimingTexture() { return timing_texture.get(); }
public:
	enum EnemyTerritoryDataIndex
	{
		Buttle_Territory_index = 6,	// バトルシーンのテリトリーデータが格納されている最初のインデックス
		Buttle_Appearance_Position_Index = 7,	// バトルシーンの出現位置データが格納されている最初のインデックス
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

	std::unique_ptr<Texture> timing_texture; // ゲームオーバー・クリアの瞬間のテクスチャ


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