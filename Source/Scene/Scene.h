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

	// �V�[�����擾
	virtual std::string GetName() const = 0;

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

	// �J�����R���g���[���[�擾
	CameraController* GetCameraController() const { return camera_controller.get(); }
private:
	bool ready = false;
	std::string scene_name; // �V�[���̖��O
public: // TDDO
	std::unique_ptr<CameraController>	camera_controller;
	bool change_scene_flag = false;
};