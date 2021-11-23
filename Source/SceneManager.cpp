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
	// �Â��V�[���̏I������
	Clear();

	// �V�����V�[����ݒ�
	current_scene.reset(scene);

	// �V�[������������
	if (!current_scene->IsReady())
	{
		current_scene->Initialize();
		current_scene->SetReady();
	}
}