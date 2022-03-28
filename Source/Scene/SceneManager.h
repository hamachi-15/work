#pragma once
#include "Scene.h"
#include <memory>
class SceneManager
{
private:
	SceneManager(){}
	~SceneManager(){}
public:
	// �C���X�^���X�擾
	static SceneManager& Instance()
	{
		static SceneManager instance;
		return instance;
	}

	// �X�V����
	void Update(float elapsed_time);

	// �`�揈��
	void Render();

	// �N���A����
	void Clear();

	// �V�[���`�F���W����
	void ChangeScene(Scene* scene);

	// ���݂̃V�[���擾
	Scene* GetCurrentScene() const { return current_scene.get(); }
private:
	std::unique_ptr<Scene> current_scene;
};