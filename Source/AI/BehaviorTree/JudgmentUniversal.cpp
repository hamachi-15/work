#include "JudgmentUniversal.h"

#include "Mathf.h"
//-------------------------------------------
// �^�[�Q�b�g���O���ɂ��邩����
//-------------------------------------------
bool JudgmentUniversal::JudgementTargetInFront(const DirectX::XMFLOAT3& origine_position, const DirectX::XMFLOAT3& origine_angle, const DirectX::XMFLOAT3& target_position)
{
	// �G�ƃv���C���[�̃x�N�g���Z�o
	DirectX::XMVECTOR vector = Mathf::ReturnVectorSubtract(target_position, origine_position);

	// �G�̊p�x����O�������Z�o
	DirectX::XMVECTOR front = Mathf::ReturnVectorFront(origine_angle);
	// ���K��
	front = DirectX::XMVector3Normalize(front);

	// �G�̑O�����ƃx�N�g��������ώZ�o
	float dot = Mathf::ReturnFloatDot(front, vector);

	// ���ς̐����őO���ɂ��邩����
	return (dot > 0);
}

//-------------------------------------------
// �����蔻����s�����Ԃ��̔���
//-------------------------------------------
void JudgmentUniversal::JudgementCollisionTime(const char* collision_name)
{
}
