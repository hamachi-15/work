// マネージャーインクルード
#include "StageManager.h"
#include "ActorManager.h"
#include "CollisionManager.h"

// コリジョンインクルード
#include "CullingCollision.h"

// アクター・コンポーネントインクルード
#include "Actor.h"
#include "Stage.h"
#include "StageObjectCollision.h"

// 計算インクルード
#include "Mathf.h"

// データベースインクルード
#include "GameDatabase.h"
#include "StageObjectData.h"
#include "WorldMapObjectPositionData.h"
#include "ButtleMapObjectPositionData.h"
#include "ActorType.h"

//*************************************************
// 
// ステージオブジェクトのマネージャークラス
// 
//*************************************************
//--------------------------------
// ステージオブジェクト登録処理
//--------------------------------
void StageManager::ButtleObjectRegister()
{
	// データベースインスタンス取得
	GameDataBase& data_base = GameDataBase::Instance();
	// ステージオブジェクトデータを取得
	std::vector<std::shared_ptr<StageObjectData>> stage_object_list = data_base.GetButtleMapObjectDataList();
	// バトルマップのオブジェクト配置位置データを取得
	std::vector<std::shared_ptr<ButtleMapObjectPositionData>> stage_object_position_list = data_base.GetButtleMapObjectPositionDataList();
	// データのサイズ取得
	int data_max_count = data_base.GetButtleMapObjectPositionDataCount();
	
	// オブジェクト生成処理
	for (int i = 0; i < data_max_count; ++i)
	{
		// オブジェクト配置データを取得
		std::shared_ptr<ButtleMapObjectPositionData> object_position_data = stage_object_position_list.at(i);
		// オブジェクトの配置データからオブジェクトデータを取得する
		std::shared_ptr<StageObjectData> object_data = data_base.GetButtleMapObjectData(object_position_data->object_category);
		// アクターを作成
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		// モデル読み込み
		actor->SetUpModel(object_data->model_path, nullptr);
		// 角度設定
		actor->SetAngle({ object_data->angle_x, object_data->angle_y, object_data->angle_z });
		// スケール設定
		actor->SetScale({ object_data->scale_x, object_data->scale_y, object_data->scale_z });
		// 名前設定
		actor->SetName(object_position_data->name);
		// 位置設定
		actor->SetPosition({ object_position_data->position_x, object_position_data->position_y, object_position_data->position_z });
		// コンポーネント追加
		actor->AddComponent<Stage>();
		actor->AddComponent<StageObjectCollision>(ActorType::Conifer);
		// プレイヤーのカリングコリジョンを追加
		CollisionManager::Instance().RegisterCulling(std::make_shared<CullingCollision>(object_data->object_category, actor));
		// シェーダー設定
		actor->SetShaderType(ShaderManager::ShaderType::Phong);
	}


}

//--------------------------------
// ステージオブジェクト登録処理
//--------------------------------
void StageManager::WorldObjectRegister()
{
	// データベースインスタンス取得
	GameDataBase& data_base = GameDataBase::Instance();
	// ステージオブジェクトデータを取得
	std::vector<std::shared_ptr<StageObjectData>> stage_object_list = data_base.GetButtleMapObjectDataList();
	// バトルマップのオブジェクト配置位置データを取得
	std::vector<std::shared_ptr<WorldMapObjectPositionData>> object_position_list = data_base.GetWorldMapObjectPositionDataList();
	// データのサイズ取得
	int data_max_count = data_base.GetWorldMapDataCount();

	// オブジェクト生成処理
	for (int i = 0; i < data_max_count; ++i)
	{
		// オブジェクト配置データを取得
		std::shared_ptr<WorldMapObjectPositionData> object_position_data = object_position_list.at(i);
		// オブジェクトの配置データからオブジェクトデータを取得する
		std::shared_ptr<StageObjectData> object_data = data_base.GetButtleMapObjectData(object_position_data->object_category);
		// アクターを作成
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		// モデル読み込み
		actor->SetUpModel(object_data->model_path, nullptr);
		// 角度設定
		actor->SetAngle({ object_data->angle_x, object_data->angle_y, object_data->angle_z });
		// スケール設定
		actor->SetScale({ object_data->scale_x, object_data->scale_y, object_data->scale_z });
		// 名前設定
		actor->SetName(object_position_data->name);
		// 位置設定
		actor->SetPosition({ object_position_data->position_x, object_position_data->position_y, object_position_data->position_z });
		// コンポーネント追加
		actor->AddComponent<Stage>();
		actor->AddComponent<StageObjectCollision>(ActorType::Conifer);
		// プレイヤーのカリングコリジョンを追加
		CollisionManager::Instance().RegisterCulling(std::make_shared<CullingCollision>(object_data->object_category, actor));
		// シェーダー設定
		actor->SetShaderType(ShaderManager::ShaderType::Phong);
	}


}

//---------------------------------
// ステージオブジェクト登録解除処理
//---------------------------------
void StageManager::Unregister(std::shared_ptr<Actor> object)
{
	// イテレーター配列からオブジェクトを検索
	std::vector<std::shared_ptr<Actor>>::iterator iterator_object = std::find(stage_objects.begin(), stage_objects.end(), object);
	// イテレーターを取得出来たら削除
	if (iterator_object != stage_objects.end())
	{
		stage_objects.erase(iterator_object);
	}
}

//--------------------------------
// 破棄処理
//--------------------------------
void StageManager::Destroy()
{
	// 配列の全てを破棄
	std::vector<std::shared_ptr<Actor>>::iterator iterator_object = stage_objects.begin();
	for (; iterator_object != stage_objects.end(); iterator_object = stage_objects.begin())
	{
		stage_objects.erase(iterator_object);
	}
}

//--------------------------------
// 更新処理
//--------------------------------
void StageManager::Update(float elapsed_time)
{
}

