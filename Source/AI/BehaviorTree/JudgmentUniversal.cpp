#include "JudgmentUniversal.h"

#include "Mathf.h"
//-------------------------------------------
// ターゲットが前方にいるか判定
//-------------------------------------------
bool JudgmentUniversal::JudgementTargetInFront(const DirectX::XMFLOAT3& origine_position, const DirectX::XMFLOAT3& origine_angle, const DirectX::XMFLOAT3& target_position)
{
	// 敵とプレイヤーのベクトル算出
	DirectX::XMVECTOR vector = Mathf::ReturnVectorSubtract(target_position, origine_position);

	// 敵の角度から前方向を算出
	DirectX::XMVECTOR front = Mathf::ReturnVectorFront(origine_angle);
	// 正規化
	front = DirectX::XMVector3Normalize(front);

	// 敵の前方向とベクトルから内積算出
	float dot = Mathf::ReturnFloatDot(front, vector);

	// 内積の正負で前方にいるか判定
	return (dot > 0);
}

//-------------------------------------------
// 当たり判定を行う時間かの判定
//-------------------------------------------
void JudgmentUniversal::JudgementCollisionTime(const char* collision_name)
{
}
