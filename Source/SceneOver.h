#pragma once
#include "Scene.h"

//********************************
// 
// �Q�[���I�[�o�[�V�[���N���X
// 
//********************************
class SceneOver :public Scene
{
public:
	// �R���X�g���N�^
	SceneOver() {}

	// �f�X�g���N�^
	~SceneOver() override {}

	// ����������
	void Initialize() override;

	// �I������
	void Finalize() override;

	// �X�V����
	void Update(float elapsed_time) override;

	// �`�揈��
	void Render() override;
};