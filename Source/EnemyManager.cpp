#include "EnemyManager.h"
#include "Mathf.h"
#include "Charactor.h"
#include "ActorManager.h"
#include "Movement.h"
#include "EnemySlime.h"
#include "EnemyMutant.h"
#include "EnemyPLT.h"
#include "EnemyLAT.h"
#include "Graphics.h"
#include "PhongVarianceShadowMap.h"

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
void EnemyManager::CreateEnemies(int i, std::string index_string)
{
	std::vector<std::shared_ptr<EnemyOccurPosition>> enemy_appearance_data = GameDataBase::Instance().GetEnemyOccurData();
	std::vector<std::shared_ptr<EnemyData>> data_list = GameDataBase::Instance().GetEnemyData();
	std::shared_ptr<EnemyData> enemy_data = data_list.at(i);
	std::shared_ptr<EnemyOccurPosition> appearance_data = enemy_appearance_data.at(i);
	std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
	std::string name = std::string(enemy_data->name) + index_string;
	actor->SetName(name.c_str());
	actor->SetUpModel(enemy_data->model_path);
	GetAppearancePosition(actor, { appearance_data->position_x, appearance_data->position_y, appearance_data->position_z });
	actor->SetScale({ enemy_data->scale_x, enemy_data->scale_y, enemy_data->scale_z });
	actor->SetAngle({ enemy_data->angle_x, enemy_data->angle_y, enemy_data->angle_z });
	actor->SetAnimationNodeName(enemy_data->animation_node_name);
	actor->AddShader<PhongVarianceShadowMap>(Graphics::Instance().GetDevice());

	switch (enemy_data->category)
	{
	case EnemyCategory::Slime:
		AddComponent(actor, enemy_data);
		actor->AddComponent<EnemySlime>();
		break;
	case EnemyCategory::LAT:
		AddComponent(actor, enemy_data);
		actor->AddComponent<EnemyLAT>();
		break;
	case EnemyCategory::PLT:
		AddComponent(actor, enemy_data);
		actor->AddComponent<EnemyPLT>();
		break;
	case EnemyCategory::Mutant:
		AddComponent(actor, enemy_data);
		actor->AddComponent<EnemyMutant>();
		break;
	case EnemyCategory::Robot:
		break;
	}
}

//-----------------------------------------------
// 出現位置を決める処理
//-----------------------------------------------
void EnemyManager::GetAppearancePosition(std::shared_ptr<Actor> actor, DirectX::XMFLOAT3 appearance_origin)
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
void EnemyManager::AddComponent(std::shared_ptr<Actor> actor, std::shared_ptr<EnemyData> data)
{
	actor->AddComponent<Movement>();
	std::shared_ptr<Charactor> charactor = actor->AddComponent<Charactor>();
	charactor->SetMaxHealth(data->hp);
	charactor->SetHealth(data->hp);
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
