 #include "Graphics.h"

#include "Mathf.h"

// シェーダー系インクルード
#include "ShaderManager.h"
#include "PhongVarianceShadowMap.h"

// データベース系インクルード
#include "GameDatabase.h"
#include "EnemyTerritoryManager.h"

#include "SceneManager.h"
#include "UIManager.h"

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

#include "BossHealthUI.h"

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
	int appearance_data_count = GameDataBase::Instance().GetEnemyTerritoryCount();
	for (int appearance_position_index = 0; appearance_position_index <= GameDataBase::Buttle_Territory_index; ++appearance_position_index)
	{
		std::shared_ptr<EnemyTerritoryPosition> appearance_data = GameDataBase::Instance().GetEnemyTerritoryData(appearance_position_index);
		DirectX::XMFLOAT3 appearance_position = { appearance_data->position_x,appearance_data->position_y, appearance_data->position_z };
		std::shared_ptr<EnemyTerritory> territory = std::make_shared<EnemyTerritory>();
		territory->SetTerritoryOrigin(appearance_position);
		territory->SetTerritoryRange(appearance_data->radius);
		territory->SetTerritoryTag(appearance_data->tag);
		EnemyTerritoryManager::Instance().RegisterTerritory(territory);

		// テリトリーの敵が撃破されたか管理するフラグを追加
		defeat_teritory[appearance_data->tag] = false;
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
	int world_map_data_max_count = 3;
	for (int i = 0; i < world_map_data_max_count; ++i)
	{
		// ワールドマップデータを取得
		std::shared_ptr<WorldMapData> world_map_data = GameDataBase::Instance().GetWorldMapData(i);
		// ワールドマップデータのIDから敵データを取得
		std::shared_ptr<EnemyData> enemy_data = GameDataBase::Instance().GetEnemyDataFromID(world_map_data->enemy_id);
		// ワールドマップデータのタグからテリトリーデータを取得
		std::shared_ptr<EnemyTerritoryPosition> territory_data = GameDataBase::Instance().GetEnemyTerritoryData(world_map_data->tag);
		// アクター作成
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		// 敵ステータス設定
		SetEnemyStatus(actor, enemy_data, territory_data);
	}
}

//-----------------------------------------------
// エンカウントデータから敵を生成
//-----------------------------------------------
void EnemyManager::CreateEnemies(int id)
{
	// 敵データリストと敵の出現位置データリストを取得
	std::vector<std::shared_ptr<EnemyData>> enemy_data_list = GameDataBase::Instance().GetEnemyDataList();
	std::vector<std::shared_ptr<EnemyTerritoryPosition>> territory_potition_list = GameDataBase::Instance().GetEnemyTerritoryDataList();
	
	// テリトリーデータのインデックスをバトルシーンのところに設定
	int territory_index = GameDataBase::Buttle_Territory_index;

	// テリトリーデータ取得
	std::shared_ptr<EnemyTerritoryPosition> territory_data = territory_potition_list.at(territory_index);

	// 出現位置データのインデックスをバトルシーンのところに設定
	int appearance_index = GameDataBase::Buttle_Appearance_Position_Index;

	// 敵データを参照していって
	for (std::shared_ptr<EnemyData> data : enemy_data_list)
	{
		// データベースのIDと一致したら敵を生成する
		if (data->id == id)
		{
			// アクター作成
			std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
			// 出現位置のデータ取得
			std::shared_ptr<EnemyTerritoryPosition> appearance_data = territory_potition_list.at(appearance_index);
			// ステータス設定
			SetEnemyStatus(actor, data, territory_data, { appearance_data->position_x, appearance_data->position_y, appearance_data->position_z });
			// インデックスのインデント
			++appearance_index;
		}
	}
}

//-----------------------------------------
// スクリプトから敵情報を取得して敵を生成する
//-----------------------------------------
bool EnemyManager::CreateEnemyEncountData()
{
	appearance_ramdam_flag = false;
	std::vector<EncountEnemyTerritory> encount_enemies = GameDataBase::Instance().GetEncountEnemyList();
	for(EncountEnemyTerritory encount_enemy : encount_enemies)
	{
		// スクリプトの敵IDから敵を生成
		CreateEnemies(encount_enemy.id);
	}
	appearance_ramdam_flag = true;
	return true;
}

//-----------------------------------------
// 敵のステータスを設定する処理
//-----------------------------------------
void EnemyManager::SetEnemyStatus(std::shared_ptr<Actor> actor, std::shared_ptr<EnemyData> enemy_data, std::shared_ptr<EnemyTerritoryPosition> teritory_data, DirectX::XMFLOAT3 appearance_position)
{
	// 名前の設定
	std::string name = std::string(enemy_data->name) + std::to_string(teritory_data->id);
	actor->SetName(name.c_str());
	
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
	DirectX::XMFLOAT3 teritory_position = { teritory_data->position_x, teritory_data->position_y, teritory_data->position_z };
	// 出現位置のランダムフラグが立ったら
	if (appearance_ramdam_flag)
	{	// 出現位置を縄張り範囲内のランダムな位置に設定
		GetAppearancePosition(actor, { teritory_position.x, teritory_position.y, teritory_position.z }, teritory_data->radius);
	}
	else
	{	// 出現位置を指定の位置に設定
		actor->SetPosition(appearance_position);
	}

	// スケールの設定
	actor->SetScale({ enemy_data->scale_x, enemy_data->scale_y, enemy_data->scale_z });

	// アングルの設定
	actor->SetAngle({ enemy_data->angle_x, enemy_data->angle_y, enemy_data->angle_z });

	// シェーダーの設定
	actor->SetShaderType(ShaderManager::ShaderType::Lambert);

	// 各敵のコンポーネント追加
	AddComponent(actor, enemy_data, teritory_data->tag);
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
void EnemyManager::AddComponent(std::shared_ptr<Actor> actor, std::shared_ptr<EnemyData> enemy_data, EnemyTerritoryTag& territory_tag)
{
	std::shared_ptr<Enemy> enemy;
	// ムーブメントコンポーネント追加
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
		if (IsBattleScene()) {
			std::shared_ptr<BossHealthUI> ui = actor->AddComponent<BossHealthUI>();
			UIManager::Instance().RegisterUI(ui);
		}
		enemy = actor->AddComponent<EnemyDragonNightmare>();
		break;
	}
	// 所属テリトリー設定
	enemy->SetBelongingToTerritory(territory_tag);

	// 敵データIDの設定
	enemy->SetEnemyDataID(enemy_data->id);

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
