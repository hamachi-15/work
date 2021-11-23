#pragma once
#include "Component.h"
#include "Stage.h"

class Movement : public Component
{
public:
	// �R���X�g���N�^�E�f�X�g���N�^
	Movement();
	virtual ~Movement();

	// ���O�擾
	const char* GetName() const override { return "Movement"; }

	// GUI�`��
	void OnGUI() override;

	void Start() override;

	// ���͍X�V����
	void UpdateVelocity(float elapsed_time);

	// �ړ�����
	void Move(const DirectX::XMFLOAT3& direction, float speed);
	void MoveLocal(const DirectX::XMFLOAT3& direction, float speed);

	// ���񏈗�
	void Turn(DirectX::XMFLOAT3& direction, float elapsed_time);

	// �W�����v����
	void Jump(float speed);

	// �󒆃_�b�V������
	void AirDush(float gravity_cut_time);

	// �Ռ���^����
	void AddImpulse(const DirectX::XMFLOAT3& impulse);

	// �������͍X�V����
	void UpdateVerticalVelocity(float elapsed_frame);

	// �����ړ��X�V����
	void UpdateVerticalMove(float elapsed_time);

	// �������͍X�V����
	void UpdateHorizontalVelocity(float elapsed_frame);

	// �����ړ��X�V����
	void UpdateHorizontalMove(float elapsed_time);

	// �ڒn����擾
	bool GetIsGround() const { return isground; }
	float		max_move_speed = 100.0f;
private:
	float		turn_speed = 6.28f;
	float		step_offset = 2.5f;
	float		gravity = -1.0f;
	float		gravity_cut_time = 0.0f;
	DirectX::XMFLOAT3 velocity = { 0, 0, 0 };
	bool		isground = false;
	float		slope_rate = 1.0f;
	float		friction = 1.5f;
	float		acceleration = 1000.0f;
	float		move_vecX = 0.0f;
	float		move_vecZ = 0.0f;
	float		air_control = 0.3f;
};