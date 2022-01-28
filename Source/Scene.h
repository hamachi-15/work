#pragma once
#include "Telegram.h"
#include "CameraController.h"
#include <string>
#include <memory>

class Scene
{
public:
	// �R���X�g���N�^
	Scene(){}
	// �f�X�g���N�^
	virtual ~Scene(){}

	// �V�[�����ݒ�
	void SetName(const char* scene_name) { this->scene_name = scene_name; }

	// �V�[�����擾
	std::string GetName() const { return scene_name.c_str(); }

	// ����������
	virtual void Initialize() = 0;
	
	// �I��������
	virtual void Finalize() = 0;

	// �X�V����
	virtual void Update(float elapsed_time) = 0;
	
	// �`�揈��
	virtual void Render() = 0;

	virtual bool OnMessages(const Telegram& telegram) { return true; }

	// ����������
	bool IsReady() const { return ready; }
	void SetReady() { ready = true; }

	// �V�[���`�F���W�t���O
	void SetSceneChangeFlag() { change_scene_flag = true; }
	bool IsSceneChangeFlag() const { return change_scene_flag; }
private:
	bool ready = false;
	std::string scene_name; // �V�[���̖��O
public: // TDDO
	std::unique_ptr<CameraController>	camera_controller;
	bool change_scene_flag = false;
};