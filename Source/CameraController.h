#pragma once
#include <DirectXMath.h>
#include "MessageData.h"

class CameraController
{
public:
	// コンストラクタ
	CameraController();

	// デストラクタ
	~CameraController();

	// 更新処理
	void Update(float elapsed_time);

	// ターゲット位置設定
	void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }

	// カメラアングル設定
	void SetCameraAngle(DirectX::XMFLOAT3 angle) { this->angle = angle; }
private:
	// フリーカメラ
	void OnFreeMode(void* data);

	// ロックオンカメラ
	void OnLockonMode(void* data);

	// モーションカメラ
	void OnMotionMode(void* data);

	// フリーカメラ更新処理
	void UpdateFreeCamera(float elapsed_time);

	// ロックオンカメラ更新処理
	void UpdateLockonCamera(float elapsed_time);

	// モーションカメラ更新処理
	void UpdateMotionCamera(float elapsed_time);

	// 横軸のズレ方向算出
	float CalcSide(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2);
private:
	// モード
	enum class Mode
	{
		FreeCamera,		// フリーカメラ
		LockonCamera,	// ロックオンカメラ
		MotionCamera,	// モーションカメラ
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
	// モーションカメラ
	float				motion_timer = 0;
	std::vector<CameraMotionData>	motion_data;

	// ロックオン処理
	DirectX::XMFLOAT3	new_position = { 0, 0, 0 };
	DirectX::XMFLOAT3	new_target = { 0, 0, 0 };
	DirectX::XMFLOAT3	target_work[2] = { { 0, 0, 0 }, { 0, 0, 0 } };	// 0 : 座標, 1 : 注視点
	float				length_limit[2] = { 90, range };
	float				side_value = 1;

	// メッセージキー
	uint64_t Camera_Change_FreeMode_key;
	uint64_t Camera_Change_LockonMode_key;
	uint64_t Camera_Change_MotionMode_key;
};