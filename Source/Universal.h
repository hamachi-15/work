#pragma once
#include <DirectXMath.h>
#include "GameDatabase.h"

class Actor;
class Enemy;
//**********************************
// 
// �ėp�֐��N���X
// 
//**********************************
class Universal
{
public:
	// �^�[�Q�b�g���O���ɂ��邩����
	static bool JudgementTargetInFront(const DirectX::XMFLOAT3& origine_position, const DirectX::XMFLOAT3& origine_angle, const DirectX::XMFLOAT3& target_position);

	// �����蔻����s�����Ԃ��̔���
	static bool JudgementCollisionTime(std::shared_ptr<Actor> actor, std::shared_ptr<AttackCollitionTime> collision_time_data);
};