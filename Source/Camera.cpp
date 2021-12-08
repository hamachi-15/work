#include "Camera.h"

//----------------------------------
// 指定方向を向く
//----------------------------------
void Camera::SetLookAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus, const DirectX::XMFLOAT3& up)
{
	// 視点、注視点、上方向からビュー行列を作成
	DirectX::XMVECTOR _eye = DirectX::XMLoadFloat3(&eye);
	DirectX::XMVECTOR _focus = DirectX::XMLoadFloat3(&focus);
	DirectX::XMVECTOR _up = DirectX::XMLoadFloat3(&up);
	DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(_eye, _focus, _up);
	DirectX::XMStoreFloat4x4(&this->view, view);

	// ビューを逆行列化し、ワールド行列に戻す
	DirectX::XMMATRIX mworld = DirectX::XMMatrixInverse(nullptr, view);
	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world, mworld);

	// カメラ座標を取り出す
	this->right.x = world._11;
	this->right.y = world._12;
	this->right.z = world._13;

	this->up.x = world._21;
	this->up.y = world._22;
	this->up.z = world._23;

	this->front.x = world._31;
	this->front.y = world._32;
	this->front.z = world._33;

	// 視点、注視点を保存
	this->eye = eye;
	this->focus = focus;
}

//------------------------------------
// パースペクティブ設定
//------------------------------------
void Camera::SetPerspectiveFov(float fovY, float aspect, float nearZ, float farZ)
{
	this->fov = fovY;
	this->aspect = aspect;
	this->nearz = nearZ;
	this->farz = farZ;
	// 画角、画面比率、クリップ距離からプロジェクション行列を作成
	DirectX::XMMATRIX projection = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, nearZ, farZ);
	DirectX::XMStoreFloat4x4(&this->projection, projection);
}
