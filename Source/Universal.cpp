#include "Universal.h"
#include "Enemy.h"
#include "Charactor.h"
#include "Mathf.h"
//**********************************
// 
// �ėp�֐��N���X
// 
//**********************************
//-------------------------------------------
// �^�[�Q�b�g���O���ɂ��邩����
//-------------------------------------------
bool Universal::JudgementTargetInFront(const DirectX::XMFLOAT3& origine_position, const DirectX::XMFLOAT3& origine_angle, const DirectX::XMFLOAT3& target_position)
{
	// �G�ƃv���C���[�̃x�N�g���Z�o
	DirectX::XMVECTOR vector = Mathf::ReturnVectorSubtract(target_position, origine_position);

	// �G�̊p�x����O�������Z�o
	DirectX::XMVECTOR front = Mathf::ReturnVectorFront(origine_angle);

	// �G�̑O�����ƃx�N�g��������ώZ�o
	float dot = Mathf::ReturnFloatDot(front, vector);

	// ���ς̐����őO���ɂ��邩����
	return (dot < 0);
}

//-------------------------------------------
// �����蔻����s�����Ԃ��̔���
//-------------------------------------------
bool Universal::JudgementCollisionTime(std::shared_ptr<Actor> actor, std::shared_ptr<AttackCollitionTime> collision_time_data)
{
	std::shared_ptr<Charactor> charactor = actor->GetComponent<Charactor>();

	// ���݂̃R���W�����t���O��1�t���[���O�̃R���W�����t���O���擾
	bool old_collision_time_flag = charactor->GetOldCollisionTimeFlag();

	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	bool collision_time_flag = charactor->SearchAnimationTime(actor, 0.2f, 0.6f);

	// �R���W�����t���O���ω�������
	if (old_collision_time_flag != collision_time_flag)
	{
		// �O�t���[���̃R���W�����t���O�����B�R���W�����t���O���ω������^�C�~���O�𒲂ׂ�p
		charactor->SetOldCollisionTimeFlag(collision_time_flag);
		return collision_time_flag;
	}

	return NULL;
}
