// マネージャーインクルード
#include "StageManager.h"
#include "ActorManager.h"

// アクター・コンポーネントインクルード
#include "Actor.h"
#include "Stage.h"

// 計算インクルード
#include "Mathf.h"

// データベースインクルード
#include "GameDatabase.h"

//*************************************************
// 
// ステージオブジェクトのマネージャークラス
// 
//*************************************************
//--------------------------------
// ステージオブジェクト登録処理
//--------------------------------
void StageManager::Register()
{
	// データベースインスタンス取得
	GameDataBase& data_base = GameDataBase::Instance();
	std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
	actor->SetUpModel("Data/Model/Filde/StageObjects.mdl", nullptr);
	actor->SetName("FildeObjects");
	actor->SetPosition(DirectX::XMFLOAT3(0, 0, 0));
	actor->SetAngle(DirectX::XMFLOAT3(0, Mathf::ConvartToRadian(-90), 0));
	actor->SetScale(DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f));
	actor->AddComponent<Stage>();
	actor->SetShaderType(ShaderManager::ShaderType::Lambert);

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

