#pragma once
#include <DirectXMath.h>
#include "GameDatabase.h"

class Actor;
class Enemy;
//**********************************
// 
// 汎用関数クラス
// 
//**********************************
class Universal
{
public:
	// ターゲットが前方にいるか判定
	static bool JudgementTargetInFront(const DirectX::XMFLOAT3& origine_position, const DirectX::XMFLOAT3& origine_angle, const DirectX::XMFLOAT3& target_position);

	// 当たり判定を行う時間かの判定
	static bool JudgementCollisionTime(std::shared_ptr<Actor> actor, std::shared_ptr<AttackCollitionTime> collision_time_data);
};