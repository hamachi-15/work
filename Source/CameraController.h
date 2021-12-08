#pragma once
#include <DirectXMath.h>
#include "MessageData.h"

class CameraController
{
public:
	// �R���X�g���N�^
	CameraController();

	// �f�X�g���N�^
	~CameraController();

	// �X�V����
	void Update(float elapsed_time);

	// �^�[�Q�b�g�ʒu�ݒ�
	void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }

	// �J�����A���O���ݒ�
	void SetCameraAngle(DirectX::XMFLOAT3 angle) { this->angle = angle; }
private:
	// �t���[�J����
	void OnFreeMode(void* data);

	// ���b�N�I���J����
	void OnLockonMode(void* data);

	// ���[�V�����J����
	void OnMotionMode(void* data);

	// �t���[�J�����X�V����
	void UpdateFreeCamera(float elapsed_time);

	// ���b�N�I���J�����X�V����
	void UpdateLockonCamera(float elapsed_time);

	// ���[�V�����J�����X�V����
	void UpdateMotionCamera(float elapsed_time);

	// �����̃Y�������Z�o
	float CalcSide(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2);
private:
	// ���[�h
	enum class Mode
	{
		FreeCamera,		// �t���[�J����
		LockonCamera,	// ���b�N�I���J����
		MotionCamera,	// ���[�V�����J����
	};

private:
	Mode mode = Mode::FreeCamera;
	DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 target = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 angle = { 0.0f, 0.0f, 0.0f };
	float roll_speed = DirectX::XMConvertToRadians(90);
	float range = 40.0f;
	float max_angleX = DirectX::XMConvertToRadians(45);
	float min_angleX = DirectX::XMConvertToRadians(-45);
	// ���[�V�����J����
	float				motion_timer = 0;
	std::vector<CameraMotionData>	motion_data;

	// ���b�N�I������
	DirectX::XMFLOAT3	new_position = { 0, 0, 0 };
	DirectX::XMFLOAT3	new_target = { 0, 0, 0 };
	DirectX::XMFLOAT3	target_work[2] = { { 0, 0, 0 }, { 0, 0, 0 } };	// 0 : ���W, 1 : �����_
	float				length_limit[2] = { 90, range };
	float				side_value = 1;

	// ���b�Z�[�W�L�[
	uint64_t Camera_Change_FreeMode_key;
	uint64_t Camera_Change_LockonMode_key;
	uint64_t Camera_Change_MotionMode_key;
};