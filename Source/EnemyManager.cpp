 #include "Graphics.h"

#include "Mathf.h"

// シェーダー系インクルード
#include "ShaderManager.h"
#include "PhongVarianceShadowMap.h"

// データベース系インクルード
#include "GameDatabase.h"
#include "EnemyCategory.h"

// マネージャーインクルード
#include "EnemyTerritoryManager.h"
#include "SceneManager.h"
#include "UIManager.h"
#include "CollisionManager.h"

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

#include "ActorType.h"
#include "CollisionMeshType.h"
#include "EnemyCollision.h"
#include "CullingCollision.h"

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
		// 出現位置のデータを取得
		std::shared_ptr<EnemyTerritoryPosition> appearance_data = GameDataBase::Instance().GetEnemyTerritoryData(appearance_position_index);
		DirectX::XMFLOAT3 appearance_position = { appearance_data->position_x,appearance_data->position_y, appearance_data->position_z };
		// テリトリーを作成
		std::shared_ptr<EnemyTerritory> territory = std::make_shared<EnemyTerritory>();
		// 出現位置のデータからテリトリーを設定
		territory->SetTerritoryOrigin(appearance_position);
		territory->SetTerritoryRange(appearance_data->radius);
		territory->SetTerritoryTag(appearance_data->tag);
		// テリトリーマネージャーに設定
		EnemyTerritoryManager::Instance().RegisterTerritory(territory);

		// テリトリーの敵が撃破されたか管理するフラグを追加
		defeat_territory[appearance_data->tag] = false;
	}
}

//-----------------------------------------------
// 撃破フラグをクリア
//-----------------------------------------------
void EnemyManager::CrearDefeat()
{
	// 撃破フラグをクリア
	defeat_territory.clear();

	// 縄張りの設定
	CreateTerritory();
}

//-----------------------------------------------
// 敵を登録
//-----------------------------------------------
void EnemyManager::EnemyRegister(std::shared_ptr<Enemy> enemy)
{
	// IDを設定
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
	
	int world_map_data_max_count = world_map_data.size();
	for (int i = 0; i < world_map_data_max_count; ++i)
	{
		// ワールドマップデータを取得
		std::shared_ptr<WorldMapData> world_map_data = GameDataBase::Instance().GetWorldMapData(i);
		
		// ワールドマップデータのタグからテリトリーデータを取得
		std::shared_ptr<EnemyTerritoryPosition> territory_data = GameDataBase::Instance().GetEnemyTerritoryData(world_map_data->tag);
		
		// 敵が撃破されていれば飛ばす
		if (defeat_territory[world_map_data->tag]) continue;

		// ワールドマップデータのIDから敵データを取得
		std::shared_ptr<EnemyData> enemy_data = GameDataBase::Instance().GetEnemyDataFromID(world_map_data->enemy_id);
		
		// アクター作成
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		
		// 敵ステータス設定
		SetEnemyStatus(actor, enemy_data, territory_data);
		
		// 各敵のコンポーネント追加
		AddComponent(actor, enemy_data, world_map_data->tag);
	}
}

//-----------------------------------------------
// エンカウントデータから敵を生成
//-----------------------------------------------
void EnemyManager::CreateEnemies(int id, EnemyTerritoryTag territory_tag)
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
			// 各敵のコンポーネント追加
			AddComponent(actor, data, territory_tag);
			// インデックスのインデント
			++appearance_index;
		}
	}
}

//-----------------------------------------
// エンカウントデータから敵情報を取得して敵を生成する
//-----------------------------------------
bool EnemyManager::CreateEnemyEncountData()
{
	appearance_ramdam_flag = false;
	std::vector<EncountEnemyTerritory> encount_enemies = GameDataBase::Instance().GetEncountEnemyList();
	for(EncountEnemyTerritory encount_enemy : encount_enemies)
	{
		// スクリプトの敵IDから敵を生成
		CreateEnemies(encount_enemy.id, encount_enemy.tag);
	}
	appearance_ramdam_flag = true;
	return true;
}

//-----------------------------------------
// 敵のステータスを設定する処理
//-----------------------------------------
void EnemyManager::SetEnemyStatus(std::shared_ptr<Actor> actor, std::shared_ptr<EnemyData> enemy_data, std::shared_ptr<EnemyTerritoryPosition> territory_data, DirectX::XMFLOAT3 appearance_position)
{
	//設定したらインクリメントする
	identity++;
	// 名前の設定
	std::string name = std::string(enemy_data->name) + std::to_string(identity);
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
	DirectX::XMFLOAT3 territory_position = { territory_data->position_x, territory_data->position_y, territory_data->position_z };
	// 出現位置のランダムフラグが立ったら
	if (appearance_ramdam_flag)
	{	// 出現位置を縄張り範囲内のランダムな位置に設定
		GetAppearancePosition(actor, { territory_position.x, territory_position.y, territory_position.z }, territory_data->radius);
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
	actor->SetShaderType(ShaderManager::ShaderType::Phong);
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
	std::shared_ptr<Charactor> charactor = actor->AddComponent<Charactor>(static_cast<int>(MetaAI::Identity::Enemy));
	charactor->SetMaxHealth(enemy_data->hp);
	charactor->SetHealth(enemy_data->hp);

	std::shared_ptr<Enemy> enemy;
	// ムーブメントコンポーネント追加
	actor->AddComponent<Movement>();

	// 敵の種類ごとのコンポーネントを追加
	switch (enemy_data->category)
	{
	case EnemyCategory::Slime:
		enemy = actor->AddComponent<EnemySlime>();
		actor->AddComponent<EnemyCollision>(EnemyCategory::Slime, identity);

		// カリング用コリジョンを追加
		CollisionManager::Instance().RegisterCulling(
			std::make_shared<CullingCollision>(EnemyCategory::Slime, actor));
		break;
	case EnemyCategory::LAT:
		enemy = actor->AddComponent<EnemyLAT>();
		actor->AddComponent<EnemyCollision>(EnemyCategory::LAT, identity);

		// カリング用コリジョンを追加
		CollisionManager::Instance().RegisterCulling(
			std::make_shared<CullingCollision>(EnemyCategory::LAT, actor));
		break;
	case EnemyCategory::PLT:
		enemy = actor->AddComponent<EnemyPLT>();
		actor->AddComponent<EnemyCollision>(EnemyCategory::PLT, identity);

		// カリング用コリジョンを追加
		CollisionManager::Instance().RegisterCulling(
			std::make_shared<CullingCollision>(EnemyCategory::PLT, actor));
		break;
	case EnemyCategory::SoulEaterDragon:
	case EnemyCategory::DragonUsurper:
		if (IsBattleScene()) {
			std::shared_ptr<BossHealthUI> ui = actor->AddComponent<BossHealthUI>();
			UIManager::Instance().RegisterUI(ui);
		}
		enemy = actor->AddComponent<EnemyDragonSoulEater>();
		actor->AddComponent<EnemyCollision>(EnemyCategory::SoulEaterDragon, identity);
		// カリング用コリジョンを追加
		//CollisionManager::Instance().RegisterCulling(
		//	std::make_shared<CullingCollision>(EnemyCategory::NightmareDragon, actor));
		break;
	case EnemyCategory::NightmareDragon:
		if (IsBattleScene()) {
			std::shared_ptr<BossHealthUI> ui = actor->AddComponent<BossHealthUI>();
			UIManager::Instance().RegisterUI(ui);
		}
		enemy = actor->AddComponent<EnemyDragonNightmare>();
		actor->AddComponent<EnemyCollision>(EnemyCategory::NightmareDragon, identity);
		// カリング用コリジョンを追加
		CollisionManager::Instance().RegisterCulling(
			std::make_shared<CullingCollision>(EnemyCategory::NightmareDragon, actor));
		break;
	}
	// アイデンティティ設定
	enemy->SetIdentity(identity);

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
