#include "FireBallManager.h"
#include "FireBall.h"

//-------------------------------
// 更新処理
//-------------------------------
void FireBallManager::Update(float elapsed_time)
{
	// 開始処理
	for (std::shared_ptr<FireBall>& fireball : start_fireballs)
	{
		fireball->Start();
		update_fireballs.emplace_back(fireball);
	}
	start_fireballs.clear();

	// 更新処理
	for (std::shared_ptr<FireBall>& fireball : update_fireballs)
	{
		fireball->Update(elapsed_time);
	}

	// 破棄配列に入っているときupdate_fireballs配列からそのアクターを破棄させる
	for (std::shared_ptr<FireBall>& fireball : remove_fireballs)
	{
		std::vector<std::shared_ptr<FireBall>>::iterator remove = std::find(update_fireballs.begin(), update_fireballs.end(), fireball);
		update_fireballs.erase(remove);
	}
	std::vector<std::shared_ptr<FireBall>>::iterator iterate_remove = remove_fireballs.begin();
	for (; iterate_remove != remove_fireballs.end(); iterate_remove = remove_fireballs.begin())
	{
		remove_fireballs.erase(iterate_remove);
	}

}

//-------------------------------
// 描画処理
//-------------------------------
void FireBallManager::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
	// 描画処理
	for (std::shared_ptr<FireBall>& fireball : update_fireballs)
	{
		fireball->Render(view, projection);
	}
}

//-------------------------------
// 火球作成
//-------------------------------
std::shared_ptr<FireBall> FireBallManager::Create(std::shared_ptr<Actor> actor, DirectX::XMFLOAT3 direction)
{
	std::shared_ptr<FireBall> fireball = std::make_shared<FireBall>(actor, direction);
	
	start_fireballs.emplace_back(fireball);

	return fireball;
}

//-------------------------------
// 火球削除
//-------------------------------
void FireBallManager::Unregister(std::shared_ptr<FireBall> fireball)
{
	remove_fireballs.emplace_back(fireball);
}

//-------------------------------
// 破棄処理
//-------------------------------
void FireBallManager::Destroy()
{
	std::vector<std::shared_ptr<FireBall>>::iterator iterate_start = start_fireballs.begin();
	for (; iterate_start != start_fireballs.end(); iterate_start = start_fireballs.begin())
	{
		start_fireballs.erase(iterate_start);
	}
	std::vector<std::shared_ptr<FireBall>>::iterator iterate_update = update_fireballs.begin();
	for (; iterate_update != update_fireballs.end(); iterate_update = update_fireballs.begin())
	{
		update_fireballs.erase(iterate_update);
	}
	std::vector<std::shared_ptr<FireBall>>::iterator iterate_remove = remove_fireballs.begin();
	for (; iterate_remove != remove_fireballs.end(); iterate_remove = remove_fireballs.begin())
	{
		remove_fireballs.erase(iterate_remove);
	}
}
