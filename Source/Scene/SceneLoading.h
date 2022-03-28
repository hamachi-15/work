#pragma once
#include <memory>
#include "Scene.h"

class Sprite;
class Texture;

class SceneLoading : public Scene
{
public:
	// コンストラクタ・デストラクタ
	SceneLoading(Scene* next_scene);
	~SceneLoading() override {}

	// シーン名取得
	std::string GetName() const override { return "SceneLoading"; }

	// 初期化処理
	void Initialize() override;

	// 終了処理
	void Finalize() override;

	// 更新処理
	void Update(float elapsed_time) override;

	// 描画処理
	void Render() override;

private:
	// ローディングスレッド
	static void LoadingThread(SceneLoading* scene);
private:
	std::unique_ptr<Scene> next_scene;
	std::unique_ptr<Sprite> sprite;
	std::unique_ptr<Texture> tex;
	float angle = 0.0f;
};