#pragma once
#include "Scene.h"

//********************************
// 
// ゲームオーバーシーンクラス
// 
//********************************
class SceneOver :public Scene
{
public:
	// コンストラクタ
	SceneOver() {}

	// デストラクタ
	~SceneOver() override {}

	// 初期化処理
	void Initialize() override;

	// 終了処理
	void Finalize() override;

	// 更新処理
	void Update(float elapsed_time) override;

	// 描画処理
	void Render() override;
};