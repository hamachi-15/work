#pragma once
#include <memory>
#include "Scene.h"
#include "Sprite.h"
#include "Texture.h"

class SceneLoading : public Scene
{
public:
	// �R���X�g���N�^�E�f�X�g���N�^
	SceneLoading(Scene* next_scene);
	~SceneLoading() override {}

	// ����������
	void Initialize() override;

	// �I������
	void Finalize() override;

	// �X�V����
	void Update(float elapsed_time) override;

	// �`�揈��
	void Render() override;

private:
	// ���[�f�B���O�X���b�h
	static void LoadingThread(SceneLoading* scene);
private:
	std::unique_ptr<Scene> next_scene;
	std::unique_ptr<Sprite> sprite;
	std::unique_ptr<Texture> tex;
	float angle = 0.0f;
};