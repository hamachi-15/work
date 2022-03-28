#pragma once
#include <string>
#include<vector>
#include <memory>
#include <map>

// 前方宣言
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
	int object_name;	//!<	名前
	int model_path;	//!<	モデルパス
	ActorType	object_category;	//!<	オブジェクトの種類
	float	scale_x;	//!<	大きさ(X)
	float	scale_y;	//!<	大きさ(Y)
	float	scale_z;	//!<	大きさ(Z)
	float	angle_x;	//!<	角度(X)
	float	angle_y;	//!<	角度(Y)
	float	angle_z;	//!<	角度(Z)
};

struct WorldMapObjectPositionDataReader
{
	int	id;	//!<	id
	int name;	//!<	名前
	ActorType	object_category;	//!<	オブジェクトの種類
	float	position_x;	//!<	座標(X)
	float	position_y;	//!<	座標(Y)
	float	position_z;	//!<	座標(Z)
};

struct ButtleMapObjectPositionDataReader
{
	int	id;	//!<	id
	int name;	//!<	名前
	ActorType	object_category;	//!<	オブジェクトの種類
	float	position_x;	//!<	座標(X)
	float	position_y;	//!<	座標(Y)
	float	position_z;	//!<	座標(Z)
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
	ActorType	enemy_category;	//!<	アクター
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
	ActorCategory	actor_type;	//!<	所持するアクターの種類
	CollisionUpdateType	collision_update_type;	//!<	更新方法の種類
};

struct	CullingCollisionParameterDataReader
{
	int	id;	//!<	id
	ActorType	enemy_category;	//!<	アクターの種類
	int collision_name;	//!<	名前
	int node_name;	//!<	ノードの名前
	float	angle;	//!<	アングル
	float	radius_x;	//!<	半径X
	float	radius_y;	//!<	半径Y
	float	radius_z;	//!<	半径Z
	float	local_x;	//!<	ローカルX座標
	float	local_y;	//!<	ローカルY座標
	float	local_z;	//!<	ローカルZ座標
	CollisionUpdateType	update_type;	//!<	更新方法の種類
};

struct AttackCollitionTimeReader
{
	int	id;	//!<	Id
	AttackCategory	attack_category;	//!<	カテゴリー
	ActorType	attacker_category;	//!<	攻撃者のカテゴリ
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
	// 読み込むデータの破棄
	/// 引数
	/// データの数と文字列の長さを格納したやつ
	void DeleteLoadData(LoadData& data);

	// 読み込むデータを作成する
	/// 引数
	/// LoadData：データの数と文字列の長さを格納したやつ
	void CreateData(const char* filename, LoadData& data);

	// ステージオブジェクトデータ読み込み
	void LoadStageObjectData();

	// ワールドマップのオブジェクトデータ読み込み
	void LoadWorldMapObjectData();

	// バトルマップのオブジェクトデータ読み込み
	void LoadButtleMapObjectData();

	// 敵データ読み込み
	void LoadEnemyData();

	// バイナリファイルから敵の配置データ読み込み
	void LoadEnemySetPositionData();

	// 敵のテリトリーデータ読み込み
	void LoadEnemyTerritoryData();

	// 当たり判定データ読み込み
	void LoadCollisionData();

	// カリング用の当たり判定データ読み込み
	void LoadCullingCollisionData();

	// 攻撃の当たり判定を行う時間データ読み込み
	void LoadCollisionTimeData();

	// バイナリファイルからアニメーションデータ読み込み
	void LoadAnimationData();

	// エンカウントした敵のテリトリー内の敵のデータを集める
	void EnemyFriendFromTerritory(EnemyTerritoryTag territory_tag);

	// ステージオブジェクトデータリスト取得
	std::vector<std::shared_ptr<StageObjectData>> GetButtleMapObjectDataList() const { return stage_object_data; }

	// ワールドのステージオブジェクトデータ取得
	std::shared_ptr<StageObjectData> GetButtleMapObjectData(int& index) const { return stage_object_data.at(index); }

	// ワールドのステージオブジェクトデータ取得
	std::shared_ptr<StageObjectData> GetButtleMapObjectData(const ActorType& category) const;

	// ステージオブジェクトデータリストのデータ数を取得
	const int GetButtleMapObjectDataCount() { return stage_object_load_data.data_count; }

	// ワールドステージオブジェクトデータリスト取得
	std::vector<std::shared_ptr<WorldMapObjectPositionData>> GetWorldMapObjectPositionDataList() const { return world_map_object_position_data; }

	// ワールドステージオブジェクトデータ取得
	std::shared_ptr<WorldMapObjectPositionData> GetWorldMapObjectPositionData(int& index) const { return world_map_object_position_data.at(index); }

	// ステージオブジェクトデータリストのデータ数を取得
	const int GetWorldMapObjectPositionDataCount() { return world_map_object_load_data.data_count; }

	// バトルステージオブジェクトデータリスト取得
	std::vector<std::shared_ptr<ButtleMapObjectPositionData>> GetButtleMapObjectPositionDataList() const { return buttle_map_object_position_data; }

	// バトルステージオブジェクトデータ取得
	std::shared_ptr<ButtleMapObjectPositionData> GetButtleMapObjectPositionData(int& index) const { return buttle_map_object_position_data.at(index); }

	// ステージオブジェクトデータリストのデータ数を取得
	const int GetButtleMapObjectPositionDataCount() { return buttle_map_object_load_data.data_count; }

	// ワールドデータリスト取得
	std::vector<std::shared_ptr<WorldMapData>> GetWorldMapDataList() const { return world_map_data; }
	
	// ワールドデータ取得
	std::shared_ptr<WorldMapData> GetWorldMapData(int& index) const { return world_map_data.at(index); }
	
	// ワールドデータリストのデータ数を取得
	const int GetWorldMapDataCount() { return world_map_object_load_data.data_count; }

	// 敵データリスト取得
	std::vector<std::shared_ptr<EnemyData>> GetEnemyDataList() const { return enemy_data; }
	
	// 敵データ取得
	std::shared_ptr<EnemyData> GetEnemyData(int& index) const { return enemy_data.at(index); }
	
	// IDから敵データ取得
	std::shared_ptr<EnemyData> GetEnemyDataFromID(int& enemy_id) const;
	
	// 敵データリストのデータ数を取得
	const int GetEnemyDataCount() { return enmy_load_data.data_count; }

	// 敵の出現位置リスト取得
	std::vector<std::shared_ptr<EnemyTerritoryPosition>> GetEnemyTerritoryDataList() const { return enemy_territory_data; }
	
	// 敵の出現位置取得
	std::shared_ptr<EnemyTerritoryPosition> GetEnemyTerritoryData(int& index) const { return enemy_territory_data.at(index); }
	
	// テリトリータグから敵の出現位置取得
	std::shared_ptr<EnemyTerritoryPosition> GetEnemyTerritoryData(EnemyTerritoryTag& tag) const;
	
	// 敵の出現位置リストのデータ数を取得
	const int GetEnemyTerritoryCount() { return	enemy_territory_load_data.data_count; }

	// エンカウントした敵データリスト取得
	std::vector<EncountEnemyTerritory> GetEncountEnemyList() { return encount_enemy; }

	// コリジョンパラメータリスト取得
	std::vector<std::shared_ptr<CollisionParameterData>> GetAttackCollitionParamterDataList() const { return collision_parameter_data; }
	
	// カテゴリーごとのパラメータデータを抽出したデータを渡す
	std::vector<std::shared_ptr<CollisionParameterData>> GetAttackCollitionParamterDataList(ActorType enemy_category) const;

	// カリングコリジョンパラメータリスト取得
	std::vector<std::shared_ptr<CullingCollisionParameterData>> GetAttackCullingCollisionParameterDataList() const { return culling_parameter_data; }

	// カテゴリーからカリングコリジョンパラメータデータをデータを渡す
	std::vector<std::shared_ptr<CullingCollisionParameterData>> GetAttackCullingCollisionParameterDataList(ActorType enemy_category) const;
	
	// 当たり判定を行うアニメーション区間リストを取得
	std::vector<std::shared_ptr<AttackCollitionTime>> GetAttackCollitionTimeDataList() const { return collision_time_data; }
	
	// 敵のカテゴリーと攻撃者から当たり判定を行うアニメーション区間を取得
	std::shared_ptr<AttackCollitionTime> GetAttackCollitionTimeData(AttackCategory attack_category, ActorType attacker_category) const;
	
	// 当たり判定を行うアニメーション区間リストのデータ数を取得
	const int GetCollitionTimeCount() { return	collision_time_load_data.data_count; }
	
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

	std::unique_ptr<Texture> timing_texture; // ゲームオーバー・クリアの瞬間のテクスチャ
	
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