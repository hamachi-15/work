#pragma once
#include "Scene.h"

//********************************
// 
// �N���A�V�[���N���X
// 
//********************************
class SceneClear :public Scene
{
public:
	// �R���X�g���N�^
	SceneClear(){}

	// �f�X�g���N�^
	~SceneClear() override {}

	// ����������
	void Initialize() override;

	// �I������
	void Finalize() override;

	// �X�V����
	void Update(float elapsed_time) override;

	// �`�揈��
	void Render() override;
};