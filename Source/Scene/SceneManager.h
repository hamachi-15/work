#pragma once
#include "Scene.h"
#include <memory>
class SceneManager
{
private:
	SceneManager(){}
	~SceneManager(){}
public:
	// インスタンス取得
	static SceneManager& Instance()
	{
		static SceneManager instance;
		return instance;
	}

	// 更新処理
	void Update(float elapsed_time);

	// 描画処理
	void Render();

	// クリア処理
	void Clear();

	// シーンチェンジ処理
	void ChangeScene(Scene* scene);

	// 現在のシーン取得
	Scene* GetCurrentScene() const { return current_scene.get(); }
private:
	std::unique_ptr<Scene> current_scene;
};