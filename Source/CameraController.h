#pragma once
#include <DirectXMath.h>
#include "MessageData.h"
#include "DebugRenderer.h"
#include "Mathf.h"

//*************************************
// 
// カメラマネージャークラス
// 
//*************************************
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
	
	// カメラの四錐台の平面取得
	std::vector<Plane> GetFrustum() const { return frustum; }
	// カメラの四錐台
	DirectX::XMFLOAT3				near_position[4] = {};	// Nearの四角形の４頂点の座標
	DirectX::XMFLOAT3				far_position[4] = {}; 	// Farの四角形の４頂点の座標
	
private:
	// 錐台計算
	void CalculateFrustum();

	// 平面計算
	void CalculatePlane(DirectX::XMFLOAT3& position1, DirectX::XMFLOAT3& position2, DirectX::XMFLOAT3& position3, DirectX::XMFLOAT3& position4, DirectX::XMFLOAT3& position5);

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
	float CalcSide(DirectX::XMFLOAT3 position1, DirectX::XMFLOAT3 position2);
private:
	// モード
	enum class Mode
	{
		FreeCamera,		// フリーカメラ
		LockonCamera,	// ロックオンカメラ
		MotionCamera,	// モーションカメラ
	};

	// メッセージキー
	uint64_t Camera_Change_FreeMode_key;
	uint64_t Camera_Change_LockonMode_key;
	uint64_t Camera_Change_MotionMode_key;

private:
	// カメラのパラメータ
	Mode mode =						Mode::FreeCamera;
	DirectX::XMFLOAT3				position = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3				target = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3				angle = { 0.0f, 0.0f, 0.0f };
	float							roll_speed = Mathf::ConvartToRadian(60);
	float							range = 50.0f;
	float							max_angleX = Mathf::ConvartToRadian(45);
	float							min_angleX = Mathf::ConvartToRadian(-45);

	std::vector<Plane>				frustum;

	// モーションカメラ
	float							motion_timer = 0;
	std::vector<CameraMotionData>	motion_data;

	// ロックオン処理
	DirectX::XMFLOAT3				new_position = { 0, 0, 0 };
	DirectX::XMFLOAT3				new_target = { 0, 0, 0 };
	DirectX::XMFLOAT3				target_work[2] = { { 0, 0, 0 }, { 0, 0, 0 } };	// 0 : 座標, 1 : 注視点
	float							length_limit[2] = { 40, range };
	float							side_value = 1;
	bool							isground = false; // カメラが地面に設置しているかのフラグ
};