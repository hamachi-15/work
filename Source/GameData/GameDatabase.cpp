#include "GameDatabase.h"
#include <iostream>
#include <fstream>
// 描画
#include "Graphics.h"
#include "Texture.h"

// コンポーネント
#include "Actor.h"
// マネージャー
#include "EnemyManager.h"

// データベース
#include "StageObjectData.h"
#include "WorldMapObjectPositionData.h"
#include "ButtleMapObjectPositionData.h"
#include "EnemyData.h"
#include "EnemyTerritoryPosition.h"
#include "CollisionParameterData.h"
#include "CullingCollisionParameterData.h"
#include "AttackCollitionTime.h"
#include "AnimationData.h"
#include "WorldMapData.h"

//-----------------------------------
// ファイル読み込み
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
// コンストラクタ
//-----------------------------------
GameDataBase::GameDataBase()
{
	Graphics& graphics = Graphics::Instance();
	// テクスチャ作成
	timing_texture = std::make_unique<Texture>();
	timing_texture->Create(static_cast<u_int>(graphics.GetScreenWidth()), static_cast<u_int>(graphics.GetScreenHeight()), DXGI_FORMAT_R8G8B8A8_UNORM);
	
	// バイナリファイルからステージオブジェクトデータ構築
	LoadStageObjectData();
	
	// バイナリファイルからワールドマップオブジェクトデータ構築
	LoadWorldMapObjectData();
	
	// バイナリファイルからバトルマップオブジェクトデータ構築
	LoadButtleMapObjectData();
	
	// バイナリファイルから敵データ構築
	LoadEnemyData();
	
	// バイナリファイルから敵生成位置データ構築
	LoadEnemySetPositionData();
	
	// バイナリファイルから敵のテリトリーデータ構築
	LoadEnemyTerritoryData();
	
	// バイナリファイルからコリジョンデータ構築
	LoadCollisionData();
	
	// バイナリファイルからカリングコリジョンのデータ構築
	LoadCullingCollisionData();
	
	// バイナリファイルから当たり判定を行う時間のデータ構築
	LoadCollisionTimeData();
	
	// バイナリファイルからアニメーションのデータ構築
	LoadAnimationData();
}

//-----------------------------------
// デストラクタ
//-----------------------------------
GameDataBase::~GameDataBase()
{
	// ステージオブジェクトdデータの破棄
	DeleteLoadData(stage_object_load_data);
	
	// ワールドマップのオブジェクトデータの破棄
	DeleteLoadData(world_map_object_load_data);
	
	// バトルマップのオブジェクトデータの破棄
	DeleteLoadData(buttle_map_object_load_data);
	
	// 敵データの破棄
	DeleteLoadData(enmy_load_data);
	
	// テリトリーデータの破棄
	DeleteLoadData(enemy_territory_load_data);
	
	// 敵の生成位置データの破棄
	DeleteLoadData(enemy_position_load_data);
	
	// カリングを行うための当たり判定データの破棄
	DeleteLoadData(culling_parameter_load_data);
	
	// 当たり判定を行うタイミングのデータ破棄
	DeleteLoadData(collision_time_load_data);
	
	// 当たり判定パラメータのデータ破棄
	DeleteLoadData(collision_parameter_load_data);
	
	// アニメーションデータ破棄
	DeleteLoadData(animation_load_data);
}

//------------------------------
// 読み込むデータの破棄
//------------------------------
void GameDataBase::DeleteLoadData(LoadData& load_data)
{
	// バッファの破棄
	if (load_data.buffer != nullptr)
	{
		delete[] load_data.buffer;
		load_data.buffer = nullptr;
	}
	
	// テキストバッファの破棄
	if (load_data.text_buffer != nullptr)
	{
		delete[] load_data.text_buffer;
		load_data.text_buffer = nullptr;
	}

	// データの数の初期化
	load_data.data_count = 0;
	load_data.headder.data_count = 0;

	// 文字列の長さを初期化
	load_data.headder.string_length = 0;
}

//-------------------------------
// 読み込むデータを作成する
//-------------------------------
void GameDataBase::CreateData(const char* filename, LoadData& load_data)
{
	// ファイル読み込み
	load_data.buffer = LoadBuffer(filename);

	// データヘッダ読み込み
	memcpy_s(&load_data.headder, sizeof(load_data.headder), load_data.buffer, sizeof(load_data.headder));

	// バトルマップのオブジェクトデータ数設定
	load_data.data_count = load_data.headder.data_count;

	// テキストバッファ生成
	load_data.text_buffer = new char[load_data.headder.string_length];

	// テキストバッファ読み込み
	memcpy_s(load_data.text_buffer, load_data.headder.string_length, &load_data.buffer[sizeof(load_data.headder)], load_data.headder.string_length);
}

//--------------------------------------------------
// ステージオブジェクトデータ読み込み
//--------------------------------------------------
void GameDataBase::LoadStageObjectData()
{
	// バイナリファイルから読み込むデータを作成
	CreateData("Data/GameData/StageObjectData.dat", stage_object_load_data);

	// マップのオブジェクトデータ生成
	for (int i = 0; i < stage_object_load_data.data_count; ++i)
	{
		stage_object_data.emplace_back(std::make_shared<StageObjectData>());
	}

	// バトルマップのオブジェクトデータ構築
	for (int i = 0; i < stage_object_load_data.data_count; ++i)
	{
		StageObjectDataReader* data = &((StageObjectDataReader*)&stage_object_load_data.buffer[sizeof(stage_object_load_data.headder) + stage_object_load_data.headder.string_length])[i];
		stage_object_data[i]->id = data->id;
		stage_object_data[i]->object_name = &stage_object_load_data.text_buffer[data->object_name];
		stage_object_data[i]->model_path = &stage_object_load_data.text_buffer[data->model_path];
		stage_object_data[i]->object_category = data->object_category;
		stage_object_data[i]->angle_x = data->angle_x;
		stage_object_data[i]->angle_y = data->angle_y;
		stage_object_data[i]->angle_z = data->angle_z;
		stage_object_data[i]->scale_x = data->scale_x;
		stage_object_data[i]->scale_y = data->scale_y;
		stage_object_data[i]->scale_z = data->scale_z;
	}
}

//-----------------------------------
// ワールドマップのオブジェクトデータ読み込み
//-----------------------------------
void GameDataBase::LoadWorldMapObjectData()
{
	// バイナリファイルから読み込むデータを作成
	CreateData("Data/GameData/WorldMapObjectPositionData.dat", world_map_object_load_data);

	// ワールドマップのオブジェクト座標データ生成
	for (int i = 0; i < world_map_object_load_data.data_count; ++i)
	{
		world_map_object_position_data.emplace_back(std::make_shared<WorldMapObjectPositionData>());
	}

	// ワールドマップのオブジェクト座標データ構築
	for (int i = 0; i < world_map_object_load_data.data_count; ++i)
	{
		WorldMapObjectPositionDataReader* data = &((WorldMapObjectPositionDataReader*)&world_map_object_load_data.buffer[sizeof(world_map_object_load_data.headder) + world_map_object_load_data.headder.string_length])[i];
		world_map_object_position_data[i]->id = data->id;
		world_map_object_position_data[i]->name = &world_map_object_load_data.text_buffer[data->name];
		world_map_object_position_data[i]->object_category = data->object_category;
		world_map_object_position_data[i]->position_x = data->position_x;
		world_map_object_position_data[i]->position_y = data->position_y;
		world_map_object_position_data[i]->position_z = data->position_z;
	}

}

//-----------------------------------
// バトルマップのオブジェクトデータ読み込み
//-----------------------------------
void GameDataBase::LoadButtleMapObjectData()
{
	// バイナリファイルから読み込むデータを作成
	CreateData("Data/GameData/ButtleMapObjectPositionData.dat", buttle_map_object_load_data);

	// バトルマップのオブジェクト座標データ生成
	for (int i = 0; i < buttle_map_object_load_data.data_count; ++i)
	{
		buttle_map_object_position_data.emplace_back(std::make_shared<ButtleMapObjectPositionData>());
	}


	// バトルマップのオブジェクト座標データ	構築
	for (int i = 0; i < buttle_map_object_load_data.data_count; ++i)
	{
		ButtleMapObjectPositionDataReader* data = &((ButtleMapObjectPositionDataReader*)&buttle_map_object_load_data.buffer[sizeof(buttle_map_object_load_data.headder) + buttle_map_object_load_data.headder.string_length])[i];
		buttle_map_object_position_data[i]->id = data->id;
		buttle_map_object_position_data[i]->name = &buttle_map_object_load_data.text_buffer[data->name];
		buttle_map_object_position_data[i]->object_category = data->object_category;
		buttle_map_object_position_data[i]->position_x = data->position_x;
		buttle_map_object_position_data[i]->position_y = data->position_y;
		buttle_map_object_position_data[i]->position_z = data->position_z;
	}

}

//-----------------------------------
// 敵データ読み込み
//-----------------------------------
void GameDataBase::LoadEnemyData()
{
	// バイナリファイルから読み込むデータを作成
	CreateData("Data/GameData/EnemyData.dat", enmy_load_data);

	// 敵のステータスデータ生成
	for (int i = 0; i < enmy_load_data.data_count; ++i)
	{
		enemy_data.emplace_back(std::make_shared<EnemyData>());
	}

	// 敵のステータスデータ構築
	for (int i = 0; i < enmy_load_data.data_count; ++i)
	{
		EnemyDataReader* data = &((EnemyDataReader*)&enmy_load_data.buffer[sizeof(enmy_load_data.headder) + enmy_load_data.headder.string_length])[i];
		enemy_data[i]->id = data->id;
		enemy_data[i]->name = &enmy_load_data.text_buffer[data->name];
		enemy_data[i]->category = data->category;
		enemy_data[i]->model_path = &enmy_load_data.text_buffer[data->path];
		enemy_data[i]->animation_node_name = &enmy_load_data.text_buffer[data->node_name];
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

}

//-----------------------------------
// バイナリファイルから敵の配置データ読み込み
//-----------------------------------
void GameDataBase::LoadEnemySetPositionData()
{
	// バイナリファイルから読み込むデータを作成
	CreateData("Data/GameData/WorldMapData.dat", enemy_position_load_data);

	// ワールドマップの敵生成データ生成
	for (int i = 0; i < enemy_position_load_data.data_count; ++i)
	{
		world_map_data.emplace_back(std::make_shared<WorldMapData>());
	}	

	// ワールドマップの敵生成データ構築
	for (int i = 0; i < enemy_position_load_data.data_count; ++i)
	{
		WorldMapDataReader* data = &((WorldMapDataReader*)&enemy_position_load_data.buffer[sizeof(enemy_position_load_data.headder) + enemy_position_load_data.headder.string_length])[i];
		world_map_data[i]->id = data->id;
		world_map_data[i]->enemy_id = data->enemy_id;
		world_map_data[i]->tag = data->tag;
	}

}

//-----------------------------------
// 敵のテリトリーデータ読み込み
//-----------------------------------
void GameDataBase::LoadEnemyTerritoryData()
{
	// バイナリファイルから読み込むデータを作成
	CreateData("Data/GameData/EnemyTerritoryPosition.dat", enemy_territory_load_data);

	// 敵のテリトリーデータ生成
	for (int i = 0; i < enemy_territory_load_data.data_count; ++i)
	{
		enemy_territory_data.emplace_back(std::make_shared<EnemyTerritoryPosition>());
	}

	// 敵のテリトリーデータ構築
	for (int i = 0; i < enemy_territory_load_data.data_count; ++i)
	{
		EnemyAppearancePositionReader* data = &((EnemyAppearancePositionReader*)&enemy_territory_load_data.buffer[sizeof(enemy_territory_load_data.headder) + enemy_territory_load_data.headder.string_length])[i];
		enemy_territory_data[i]->id = data->id;
		enemy_territory_data[i]->tag = data->tag;
		enemy_territory_data[i]->position_x = data->position_x;
		enemy_territory_data[i]->position_y = data->position_y;
		enemy_territory_data[i]->position_z = data->position_z;
		enemy_territory_data[i]->radius = data->radius;
	}

}

//-----------------------------------
// 当たり判定データ読み込み
//-----------------------------------
void GameDataBase::LoadCollisionData()
{
	// バイナリファイルから読み込むデータを作成
	CreateData("Data/GameData/CollisionParameterData.dat", collision_parameter_load_data);

	// 当たり判定のパラメータデータ生成
	for (int i = 0; i < collision_parameter_load_data.data_count; ++i)
	{
		collision_parameter_data.emplace_back(std::make_shared<CollisionParameterData>());
	}

	// コリジョンパラメータデータ構築
	for (int i = 0; i < collision_parameter_load_data.data_count; i++)
	{
		CollitionParameterReader* data = &((CollitionParameterReader*)&collision_parameter_load_data.buffer[sizeof(collision_parameter_load_data.headder) + collision_parameter_load_data.headder.string_length])[i];
		collision_parameter_data[i]->id = data->id;
		collision_parameter_data[i]->collision_name = &collision_parameter_load_data.text_buffer[data->collision_name];
		collision_parameter_data[i]->node_name = &collision_parameter_load_data.text_buffer[data->node_name];
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
		collision_parameter_data[i]->enemy_category = data->enemy_category;
	}

}

//-----------------------------------
// カリング用の当たり判定データ読み込み
//-----------------------------------
void GameDataBase::LoadCullingCollisionData()
{
	// バイナリファイルから読み込むデータを作成
	CreateData("Data/GameData/CullingCollisionParameterData.dat", culling_parameter_load_data);

	// 当たり判定のパラメータデータ生成
	for (int i = 0; i < culling_parameter_load_data.data_count; ++i)
	{
		culling_parameter_data.emplace_back(std::make_shared<CullingCollisionParameterData>());
	}
	// カリングコリジョンパラメータデータ構築
	for (int i = 0; i < culling_parameter_load_data.data_count; i++)
	{
		CullingCollisionParameterDataReader* data = &((CullingCollisionParameterDataReader*)&culling_parameter_load_data.buffer[sizeof(culling_parameter_load_data.headder) + culling_parameter_load_data.headder.string_length])[i];
		culling_parameter_data[i]->id = data->id;
		culling_parameter_data[i]->collision_name = &culling_parameter_load_data.text_buffer[data->collision_name];
		culling_parameter_data[i]->node_name = &culling_parameter_load_data.text_buffer[data->node_name];
		culling_parameter_data[i]->angle = data->angle;
		culling_parameter_data[i]->radius_x = data->radius_x;
		culling_parameter_data[i]->radius_y = data->radius_y;
		culling_parameter_data[i]->radius_z = data->radius_z;
		culling_parameter_data[i]->local_x = data->local_x;
		culling_parameter_data[i]->local_y = data->local_y;
		culling_parameter_data[i]->local_z = data->local_z;
		culling_parameter_data[i]->update_type = data->update_type;
		culling_parameter_data[i]->enemy_category = data->enemy_category;
	}

}

//-----------------------------------
// 攻撃の当たり判定を行う時間データ読み込み
//-----------------------------------
void GameDataBase::LoadCollisionTimeData()
{
	// バイナリファイルから読み込むデータを作成
	CreateData("Data/GameData/AttackCollitionTime.dat", collision_time_load_data);

	// プレイヤーの任意のアニメーション区間開始時間と終了時間(当たり判定に使う)データ生成
	for (int i = 0; i < collision_time_load_data.data_count; ++i)
	{
		collision_time_data.emplace_back(std::make_shared<AttackCollitionTime>());
	}

	// プレイヤーの任意のアニメーション区間開始時間と終了時間(当たり判定に使う)データ構築
	for (int i = 0; i < collision_time_load_data.data_count; ++i)
	{
		AttackCollitionTimeReader* data = &((AttackCollitionTimeReader*)&collision_time_load_data.buffer[sizeof(collision_time_load_data.buffer) + collision_time_load_data.headder.string_length])[i];
		collision_time_data[i]->id = data->id;
		collision_time_data[i]->attack_category = data->attack_category;
		collision_time_data[i]->attacker_category = data->attacker_category;
		collision_time_data[i]->start_time = data->start_time;
		collision_time_data[i]->end_time = data->end_time;
	}

}


//-----------------------------------
// バイナリファイルからアニメーションデータ読み込み
//-----------------------------------
void GameDataBase::LoadAnimationData()
{
	// バイナリファイルから読み込むデータを作成
	CreateData("Data/GameData/AnimationData.dat", animation_load_data);

	// アニメーションデータ構築
	for (int i = 0; i < animation_load_data.data_count; ++i)
	{
		AnimationDataReader* data = &((AnimationDataReader*)&animation_load_data.buffer[sizeof(animation_load_data.headder) + animation_load_data.headder.string_length])[i];
		std::shared_ptr<AnimationData> animation = std::make_unique<AnimationData>();
		animation->id = data->id;
		animation->animation_name = &animation_load_data.text_buffer[data->animation_name];
		animation->number = data->number;
		animation->roop_flag = data->roop_flag;
		animation->blend = data->blend;
		// マップに追加
		animation_data[animation->animation_name] = animation;
	}

}

//--------------------------------------------------
// エンカウントした敵のテリトリー内の敵のデータを集める
//--------------------------------------------------
void GameDataBase::EnemyFriendFromTerritory(EnemyTerritoryTag territory_tag)
{
	// 敵マネージャー取得
	EnemyManager& enemy_manager = EnemyManager::Instance();
	// すでに登録されていたら
	if (encount_enemy.size() > 0)
	{
		// クリアする
		encount_enemy.clear();
	}
	// 敵の数を取得
	int enemy_count = enemy_manager.GetEnemyCount();

	for (int i = 0; i < enemy_count; ++i)
	{
		std::shared_ptr<Enemy> enemy = enemy_manager.GetEnemy(i);
		// 所属テリトリーがエンカウントした敵のテリトリーと同じなら
		if (enemy->GetBelongingToTerritory() == territory_tag)
		{
			// エンカウントした敵のリストに追加する
			EncountEnemyTerritory enemy_data;
			enemy_data.id = enemy->GetEnemyDataID();
			enemy_data.tag = territory_tag;
			encount_enemy.emplace_back(enemy_data);
		}
	}
}

//---------------------------------------------------------------------------------
// ステージオブジェクトデータ取得
//---------------------------------------------------------------------------------
std::shared_ptr<StageObjectData> GameDataBase::GetButtleMapObjectData(const ActorType& category) const
{
	// ステージオブジェクトのデータから
	for (std::shared_ptr<StageObjectData> data : stage_object_data)
	{
		// 引数のカテゴリーと同じならそのデータを返す
		if (data->object_category == category) return data;
	}

	return nullptr;
}

//---------------------------------------------------------------------------------
// カテゴリーごとのパラメータデータを抽出したデータを渡す
//---------------------------------------------------------------------------------
std::vector<std::shared_ptr<CollisionParameterData>> GameDataBase::GetAttackCollitionParamterDataList(ActorType enemy_category) const
{
	std::vector<std::shared_ptr<CollisionParameterData>> collision_arameter;
	// 同じアクターテゴリーを持つデータを追加して配列を作成する
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
// カテゴリーからカリングコリジョンパラメータデータをデータを渡す
//---------------------------------------------------------------------------------
std::vector<std::shared_ptr<CullingCollisionParameterData>> GameDataBase::GetAttackCullingCollisionParameterDataList(ActorType enemy_category) const
{
	std::vector<std::shared_ptr<CullingCollisionParameterData>> culling_arameter;
	// 同じアクターテゴリーを持つデータを追加して配列を作成する
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
// 攻撃者のカテゴリーと攻撃カテゴリーから当たり判定を行うアニメーション区間を取得
//---------------------------------------------------------------------------------
std::shared_ptr<AttackCollitionTime> GameDataBase::GetAttackCollitionTimeData(AttackCategory attack_category, ActorType attacker_category) const
{
	// 同じ攻撃カテゴリーを持つデータを返す
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
// コリジョンを行う時間データ取得
//-----------------------------------
std::shared_ptr<AttackCollitionTime> GameDataBase::GetAttackCollitionTimeData(AttackCategory attack_category) const
{
	// 同じ攻撃カテゴリーを持つデータを返す
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
// 敵データIDから敵データ取得
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

//-----------------------------------
// テリトリーのタグからテリトリーデータを取得する
//-----------------------------------
std::shared_ptr<EnemyTerritoryPosition> GameDataBase::GetEnemyTerritoryData(EnemyTerritoryTag& tag) const
{
	// 同じタグを持つデータを返す
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
// アニメーションデータ取得
//-----------------------------------
std::shared_ptr<AnimationData> GameDataBase::GetAnimationData(std::string animation_name)
{
	// 同じアニメーションの名前を持つデータを返す
	AnimationMap::iterator it = animation_data.find(animation_name);
	if (it != animation_data.end())
	{
		return it->second;
	}
	return nullptr;
}
