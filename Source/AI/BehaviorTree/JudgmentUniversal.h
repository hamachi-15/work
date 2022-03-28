#pragma once
#include <DirectXMath.h>

class JudgmentUniversal
{
public:
	// ターゲットが前方にいるか判定
	static bool JudgementTargetInFront(const DirectX::XMFLOAT3& origine_position, const DirectX::XMFLOAT3& origine_angle, const DirectX::XMFLOAT3& target_position);

	// 当たり判定を行う時間かの判定
	static void JudgementCollisionTime(const char* collision_name);
};