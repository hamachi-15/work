#pragma once
#include <DirectXMath.h>

class JudgmentUniversal
{
public:
	// �^�[�Q�b�g���O���ɂ��邩����
	static bool JudgementTargetInFront(const DirectX::XMFLOAT3& origine_position, const DirectX::XMFLOAT3& origine_angle, const DirectX::XMFLOAT3& target_position);

	// �����蔻����s�����Ԃ��̔���
	static void JudgementCollisionTime(const char* collision_name);
};