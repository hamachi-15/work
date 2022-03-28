#include "Camera.h"

//----------------------------------
// �w�����������
//----------------------------------
void Camera::SetLookAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus, const DirectX::XMFLOAT3& up)
{
	// ���_�A�����_�A���������r���[�s����쐬
	DirectX::XMVECTOR _eye = DirectX::XMLoadFloat3(&eye);
	DirectX::XMVECTOR _focus = DirectX::XMLoadFloat3(&focus);
	DirectX::XMVECTOR _up = DirectX::XMLoadFloat3(&up);
	DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(_eye, _focus, _up);
	DirectX::XMStoreFloat4x4(&this->view, view);

	// �r���[���t�s�񉻂��A���[���h�s��ɖ߂�
	DirectX::XMMATRIX mworld = DirectX::XMMatrixInverse(nullptr, view);
	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world, mworld);

	// �J�������W�����o��
	this->right.x = world._11;
	this->right.y = world._12;
	this->right.z = world._13;

	this->up.x = world._21;
	this->up.y = world._22;
	this->up.z = world._23;

	this->front.x = world._31;
	this->front.y = world._32;
	this->front.z = world._33;

	// ���_�A�����_��ۑ�
	this->eye = eye;
	this->focus = focus;
}

//------------------------------------
// �p�[�X�y�N�e�B�u�ݒ�
//------------------------------------
void Camera::SetPerspectiveFov(float fovY, float aspect, float nearZ, float farZ)
{
	this->fov = fovY;
	this->aspect = aspect;
	this->nearz = nearZ;
	this->farz = farZ;
	// ��p�A��ʔ䗦�A�N���b�v��������v���W�F�N�V�����s����쐬
	DirectX::XMMATRIX projection = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, nearZ, farZ);
	DirectX::XMStoreFloat4x4(&this->projection, projection);
}
