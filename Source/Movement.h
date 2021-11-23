#pragma once
#include "Component.h"
#include "Stage.h"

class Movement : public Component
{
public:
	// コンストラクタ・デストラクタ
	Movement();
	virtual ~Movement();

	// 名前取得
	const char* GetName() const override { return "Movement"; }

	// GUI描画
	void OnGUI() override;

	void Start() override;

	// 速力更新処理
	void UpdateVelocity(float elapsed_time);

	// 移動処理
	void Move(const DirectX::XMFLOAT3& direction, float speed);
	void MoveLocal(const DirectX::XMFLOAT3& direction, float speed);

	// 旋回処理
	void Turn(DirectX::XMFLOAT3& direction, float elapsed_time);

	// ジャンプ処理
	void Jump(float speed);

	// 空中ダッシュ処理
	void AirDush(float gravity_cut_time);

	// 衝撃を与える
	void AddImpulse(const DirectX::XMFLOAT3& impulse);

	// 垂直速力更新処理
	void UpdateVerticalVelocity(float elapsed_frame);

	// 垂直移動更新処理
	void UpdateVerticalMove(float elapsed_time);

	// 水平速力更新処理
	void UpdateHorizontalVelocity(float elapsed_frame);

	// 水平移動更新処理
	void UpdateHorizontalMove(float elapsed_time);

	// 接地判定取得
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