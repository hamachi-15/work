#pragma once
#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "FireBall.h"
class Actor;

class FireBallManager
{
private:
	FireBallManager() {}
	~FireBallManager() {}
public:
	// インスタンス取得
	static FireBallManager& Instance()
	{
		static FireBallManager instance;
		return instance;
	}

	// 更新処理
	void Update(float elapsed_time);
	
	// 描画処理
	void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

	// 火球作成
	std::shared_ptr<FireBall> Create(std::shared_ptr<Actor> actor, DirectX::XMFLOAT3 direction);

	// 火球削除
	void Unregister(std::shared_ptr<FireBall> fireball);

	// 破棄処理
	void Destroy();
private:
	std::vector<std::shared_ptr<FireBall>> start_fireballs;
	std::vector<std::shared_ptr<FireBall>> update_fireballs;
	std::vector<std::shared_ptr<FireBall>> remove_fireballs;
};