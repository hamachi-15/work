#pragma once
#include "Scene.h"

class SceneTutorial : public Scene
{
public:
	SceneTutorial() {};
	~SceneTutorial()override{}
private:
	// 初期化処理
	void Initialize()override;

	// 終了化処理
	void Finalize()override;

	// 更新処理
	void Update(float elapsed_time)override;

	// 描画処理
	void Render()override;

};