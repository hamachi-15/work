 #include "Graphics.h"

#include "Mathf.h"

// シェーダー系インクルード
#include "ShaderManager.h"
#include "PhongVarianceShadowMap.h"

// データベース系インクルード
#include "Script.h"
#include "EnemyTerritoryManager.h"

// アクター系インクルード
#include "Charactor.h"
#include "Movement.h"
#include "ActorManager.h"
#include "EnemyManager.h"
#include "EnemySlime.h"
#include "EnemyPLT.h"
#include "EnemyLAT.h"
#include "EnemyDragonNightmare.h"
#include "EnemyDragonSoulEater.h"

//-----------------------------------------------
// コンストラクタ
//-----------------------------------------------
EnemyManager::EnemyManager()
{
}

//-----------------------------------------------
// デストラクタ
//-----------------------------------------------
EnemyManager::~EnemyManager()
{
}

//-----------------------------------------------
// 縄張りの設定
//-----------------------------------------------
void EnemyManager::CreateTerritory()
{
	int appearance_data_count = GameDataBase::Instance().GetEnemyOccurCount();
	for (int appearance_position_index = 0; appearance_position_index < appearance_data_count; ++appearance_position_index)
	{
		std::shared_ptr<EnemyAppearancePosition> appearance_data = GameDataBase::Instance().GetEnemyAppearanceData(appearance_position_index);
		DirectX::XMFLOAT3 appearance_position = { appearance_data->position_x,appearance_data->position_y, appearance_data->position_z };
		std::shared_ptr<EnemyTerritory> territory = std::make_shared<EnemyTerritory>();
		territory->SetTerritoryOrigin(appearance_position);
		territory->SetTerritoryRange(appearance_data->radius);
		territory->SetTerritoryTag(appearance_data->tag);
		EnemyTerritoryManager::Instance().RegisterTerritory(territory);
	}
}

//-----------------------------------------------
// 敵を登録
//-----------------------------------------------
void EnemyManager::EnemyRegister(std::shared_ptr<Enemy> enemy)
{
	// IDを設定
	enemy->GetCharactor()->SetID(static_cast<int>(MetaAI::Identity::Enemy));
	enemy->SetIdentity(identity);
	//設定したらインクリメントする
	identity++;		
	// 敵リストに追加
	enemies.emplace_back(enemy);
}

//-----------------------------------------------
// 敵を破棄
//-----------------------------------------------
void EnemyManager::EnemyRemove(std::shared_ptr<Enemy> enemy)
{
	std::vector<std::shared_ptr<Enemy>>::iterator iterate_enemy = std::find(enemies.begin(), enemies.end(), enemy);
	enemies.erase(iterate_enemy);
}

//-----------------------------------------------
// 全ての敵を破棄
//-----------------------------------------------
void EnemyManager::AllRemove()
{
	std::vector<std::shared_ptr<Enemy>>::iterator iterate_enemy = enemies.begin();
	for (; iterate_enemy != enemies.end(); iterate_enemy = enemies.begin())
	{
		enemies.erase(iterate_enemy);
	}
	enemies.clear();
}

//-----------------------------------------------
// デバッグプリミティブ描画
//-----------------------------------------------
void EnemyManager::DrawDebugPrimitive()
{
	for (std::shared_ptr<Enemy> enemy : enemies)
	{
		enemy->DrawDebugPrimitive();
	}
}

//-----------------------------------------------
// 敵を生成
//-----------------------------------------------
void EnemyManager::CreateEnemies()
{
	std::vector<std::shared_ptr<WorldMapData>> world_map_data = GameDataBase::Instance().GetWorldMapDataList();
	int world_map_data_max_count = GameDataBase::Instance().GetWorldMapDataCount();
	for (int i = 0; i < world_map_data_max_count; ++i)
	{
		// ワールドマップデータを取得
		std::shared_ptr<WorldMapData> world_map_data = GameDataBase::Instance().GetWorldMapData(i);
		// ワールドマップデータのIDから敵データを取得
		std::shared_ptr<EnemyData> enemy_data = GameDataBase::Instance().GetEnemyDataFromID(world_map_data->enemy_id);
		// ワールドマップデータのタグから敵の出現位置データを取得
		std::shared_ptr<EnemyAppearancePosition> appearance_data = GameDataBase::Instance().GetEnemyAppearanceData(world_map_data->tag);
		// アクター作成
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		// 敵ステータス設定
		SetEnemyStatus(actor, enemy_data, appearance_data);
	}
}

//-----------------------------------------------
// スクリプトから敵を生成
//-----------------------------------------------
void EnemyManager::CreateEnemies(int id)
{
	// 敵データリストと敵の出現位置データリストを取得
	std::vector<std::shared_ptr<EnemyData>> data_list = GameDataBase::Instance().GetEnemyDataList();
	std::vector<std::shared_ptr<EnemyAppearancePosition>> appearance_potition_list = GameDataBase::Instance().GetEnemyAppearanceDataList();
	int index = 0;
	for (std::shared_ptr<EnemyData> data : data_list)
	{
		// データベースのIDと一致したら敵を生成する
		if (data->id == id)
		{
			// アクター作成
			std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
			// 出現位置のデータ取得
			std::shared_ptr<EnemyAppearancePosition> appearance_data = appearance_potition_list.at(index);
			// ステータス設定
			SetEnemyStatus(actor, data, appearance_data);
		}
		++index;
	}
}

//-----------------------------------------
// スクリプトから敵情報を取得して敵を生成する
//-----------------------------------------
bool EnemyManager::CreateEnemyScriptData()
{
	Script* script = new Script("./Data/Script/SendBattleSceneScript.txt");
	char	strwork[256] = {};

	while (1)
	{
		if (!script->SearchTop())
			break;
		if (script == nullptr)
		{
			return false;
		}
		//	文字列をファイルから取り出す
		script->GetParamString(strwork);

		//	終了コマンド
		if (lstrcmpA(strwork, "END") == 0)
		{
			break;
		}

		if (lstrcmpA(strwork, "EnemyID") == 0)
		{
			// スクリプトの敵IDから敵を生成
			CreateEnemies(script->GetParamInt());
		}
	}
	delete script;
	return true;
}

//-----------------------------------------
// 敵のステータスを設定する処理
//-----------------------------------------
void EnemyManager::SetEnemyStatus(std::shared_ptr<Actor> actor, std::shared_ptr<EnemyData> enemy_data, std::shared_ptr<EnemyAppearancePosition> appearance_data)
{
	// 名前の設定
	std::string name = std::string(enemy_data->name) + std::to_string(appearance_data->id);
	actor->SetName(name.c_str());

	// 敵データIDの設定
	actor->SetEnemyDataID(enemy_data->id);
	
	// モデルのセットアップ
	if (strcmp(enemy_data->animation_node_name, "NULL") == 0)
	{
		actor->SetUpModel(enemy_data->model_path, nullptr);
	}
	else
	{
		actor->SetUpModel(enemy_data->model_path, enemy_data->animation_node_name);
	}
	// 出現位置の設定
	DirectX::XMFLOAT3 appearance_position = { appearance_data->position_x,appearance_data->position_y, appearance_data->position_z };
	GetAppearancePosition(actor, { appearance_position.x, appearance_position.y, appearance_position.z }, appearance_data->radius);
	
	// スケールの設定
	actor->SetScale({ enemy_data->scale_x, enemy_data->scale_y, enemy_data->scale_z });

	// アングルの設定
	actor->SetAngle({ enemy_data->angle_x, enemy_data->angle_y, enemy_data->angle_z });

	// シェーダーの設定
	actor->SetShaderType(ShaderManager::ShaderType::Lambert);

	// 各敵のコンポーネント追加
	AddComponent(actor, enemy_data, appearance_data->tag);
}

void EnemyManager::SetEnemyStatus(std::shared_ptr<Actor> actor, std::shared_ptr<EnemyData> enemy_data, int& string_id, DirectX::XMFLOAT3& appearance_position)
{
	// 名前の設定
	std::string name = std::string(enemy_data->name) + std::to_string(string_id);
	actor->SetName(name.c_str());

	// 敵データIDの設定
	actor->SetEnemyDataID(enemy_data->id);

	// モデルのセットアップ
	actor->SetUpModel(enemy_data->model_path, enemy_data->animation_node_name);

	// 出現位置の設定
	GetAppearancePosition(actor, { appearance_position.x, appearance_position.y, appearance_position.z }, 2);

	// スケールの設定
	actor->SetScale({ enemy_data->scale_x, enemy_data->scale_y, enemy_data->scale_z });

	// アングルの設定
	actor->SetAngle({ enemy_data->angle_x, enemy_data->angle_y, enemy_data->angle_z });

	// シェーダーの設定
	actor->SetShaderType(ShaderManager::ShaderType::Lambert);

	// 各敵のコンポーネント追加
	std::shared_ptr<Enemy> enemy;
	actor->AddComponent<Movement>();
	std::shared_ptr<Charactor> charactor = actor->AddComponent<Charactor>();
	
	// 最大HP設定
	charactor->SetMaxHealth(enemy_data->hp);

	// HP設定
	charactor->SetHealth(enemy_data->hp);

	// 敵の種類ごとのコンポーネントを追加
	// TODO 敵を作り終えたら対応する
	switch (enemy_data->category)
	{
	case EnemyCategory::LAT:
		enemy = actor->AddComponent<EnemyLAT>();
		break;
	case EnemyCategory::Slime:
		enemy = actor->AddComponent<EnemySlime>();
		break;
	case EnemyCategory::PLT:
		enemy = actor->AddComponent<EnemyPLT>();
		break;
	case EnemyCategory::NightmareDragon:
		enemy = actor->AddComponent<EnemyDragonNightmare>();
		break;
	case EnemyCategory::SoulEaterDragon:
		enemy = actor->AddComponent<EnemyDragonSoulEater>();
		break;
	case EnemyCategory::DragonUsurper:
		break;
	}

}

//-----------------------------------------------
// 出現位置を決める処理
//-----------------------------------------------
void EnemyManager::GetAppearancePosition(std::shared_ptr<Actor> actor, DirectX::XMFLOAT3 appearance_origin, float appearance_range)
{
	float theta = Mathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
	float range = Mathf::RandomRange(0.0f, appearance_range);

	DirectX::XMFLOAT3 appearance_position;
	appearance_position.x = appearance_origin.x + sinf(theta) * range;
	appearance_position.y = appearance_origin.y;
	appearance_position.z = appearance_origin.z + cosf(theta) * range;
	actor->SetPosition(appearance_position);
}

//-----------------------------------------------
// スライムのコンポーネントを追加する処理
//-----------------------------------------------
void EnemyManager::AddComponent(std::shared_ptr<Actor> actor, std::shared_ptr<EnemyData> enemy_data, EnemyTerritoryTag& tag)
{
	std::shared_ptr<Enemy> enemy;
	actor->AddComponent<Movement>();
	std::shared_ptr<Charactor> charactor = actor->AddComponent<Charactor>();
	charactor->SetMaxHealth(enemy_data->hp);
	charactor->SetHealth(enemy_data->hp);

	// 敵の種類ごとのコンポーネントを追加
	switch (enemy_data->category)
	{
	case EnemyCategory::Slime:
		enemy = actor->AddComponent<EnemySlime>();
		break;
	case EnemyCategory::LAT:
		enemy = actor->AddComponent<EnemyLAT>();
		break;
	case EnemyCategory::PLT:
		enemy = actor->AddComponent<EnemyPLT>();
		break;
	case EnemyCategory::NightmareDragon:
		//break;
	case EnemyCategory::SoulEaterDragon:
		//break;
	case EnemyCategory::DragonUsurper:
		enemy = actor->AddComponent<EnemyDragonNightmare>();
		break;
	}
	std::shared_ptr<EnemyTerritory> territory = EnemyTerritoryManager::Instance().GetTerritory(tag);
	enemy->SetTerritoryOrigin(territory->GetTerritoryOrigin());
	enemy->SetTerritoryRange(territory->GetTerritoryRange());
}

//-----------------------------------------------
// IDで敵を取得
//-----------------------------------------------
std::shared_ptr<Enemy> EnemyManager::GetEnemyFromID(int id)
{
	int enemy_count = GetEnemyCount();
	for (int i = 0; i < enemy_count; ++i)
	{
		std::shared_ptr<Enemy> enemy = enemies.at(i);
		// idとアイデンティティから特定の敵を取得する
		if (enemy->GetCharactor()->GetID() + enemy->GetIdentity() ==  id)
		{
			return enemy;
		}
	}
	return nullptr;
}
