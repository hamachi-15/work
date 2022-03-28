#pragma once
#include <memory>
#include <vector>

class Actor;

//*************************************************
// 
// ステージオブジェクトのマネージャークラス
// 
//*************************************************
class StageManager
{
private:
	// コンストラクタ
	StageManager() {}

	// デストラクタ
	~StageManager() {}

public:
	// インスタンス取得
	static StageManager& Instance()
	{
		static StageManager instance;
		return instance;
	}

	// ワールドステージオブジェクト登録処理
	void WorldObjectRegister();

	// バトルステージオブジェクト登録処理
	void ButtleObjectRegister();

	// ステージオブジェクト登録解除処理
	void Unregister(std::shared_ptr<Actor> object);

	// 破棄処理
	void Destroy();

	// 更新処理
	void Update(float elapsed_time);

private:
	std::vector<std::shared_ptr<Actor>> stage_objects;
};