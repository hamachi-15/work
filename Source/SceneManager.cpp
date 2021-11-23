#include "SceneManager.h"

void SceneManager::Update(float elapsed_time)
{
	if (current_scene != nullptr)
	{
		current_scene->Update(elapsed_time);
	}
}

void SceneManager::Render()
{
	if (current_scene != nullptr)
	{
		current_scene->Render();
	}
}

void SceneManager::Clear()
{
	if (current_scene != nullptr)
	{
		current_scene->Finalize();
	}
}

void SceneManager::ChangeScene(Scene* scene)
{
	// 古いシーンの終了処理
	Clear();

	// 新しいシーンを設定
	current_scene.reset(scene);

	// シーン初期化処理
	if (!current_scene->IsReady())
	{
		current_scene->Initialize();
		current_scene->SetReady();
	}
}