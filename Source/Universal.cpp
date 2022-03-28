#include "Universal.h"
#include "Enemy.h"
#include "Charactor.h"
#include "Mathf.h"

#include "Model.h"
#include "Collision.h"

// �Q�[���f�[�^
#include "AttackCollitionTime.h"
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
	bool collision_time_flag = charactor->SearchAnimationTime(actor, collision_time_data->start_time, collision_time_data->end_time);

	// �R���W�����t���O���ω�������
	if (old_collision_time_flag != collision_time_flag)
	{
		// �O�t���[���̃R���W�����t���O�����B�R���W�����t���O���ω������^�C�~���O�𒲂ׂ�p
		charactor->SetOldCollisionTimeFlag(collision_time_flag);
		return true;
	}

	return false;
}

//-------------------------------------------
// �R���W�������W���w��̃m�[�h���W�֍X�V
//-------------------------------------------
void Universal::NodePositionUpdate(CollisionObject* collision_object, std::string node_name, Model* model)
{
	// �m�[�h���W�擾
	DirectX::XMFLOAT3 position;
	Mathf::GetNodePosition(node_name.c_str(), position, model);

	// ���W�ݒ�
	collision_object->SetPosition(position);
}

//-------------------------------------------
// �R���W�������W���w��̃��[�J�����W�֍X�V
//-------------------------------------------
void Universal::LocalPositionUpdate(CollisionObject* collision_object, Model::Node* node)
{

	DirectX::XMMATRIX world_transform_matrix = DirectX::XMLoadFloat4x4(&node->world_transform);
	DirectX::XMFLOAT3 local_position = collision_object->GetLocalPosition();
	
	DirectX::XMVECTOR position = DirectX::XMVector3TransformCoord(
		DirectX::XMLoadFloat3(&local_position), world_transform_matrix);

	DirectX::XMFLOAT3 collision_position;
	DirectX::XMStoreFloat3(&collision_position, position);
	
	// ���W�ݒ�
	collision_object->SetPosition(collision_position);
}

//-------------------------------------------
// �R���W�������W���w��̃A�N�^�[���W�֍X�V
//-------------------------------------------
void Universal::ActorPositionUpdate(CollisionObject* collision_object, std::shared_ptr<Actor> actor)
{
	// ���W�ݒ�
	collision_object->SetPosition(actor->GetPosition());
}

//-------------------------------------------
// �R���W�������W���w��̃A�N�^�[�A
// �w��̃��[�J�����W�ł͂Ȃ����W�֍X�V
//-------------------------------------------
void Universal::CastamPositionUpdate(CollisionObject* collision_object, std::shared_ptr<Actor> actor, std::string node_name, Model* model)
{
	DirectX::XMFLOAT3 position;
	Mathf::GetNodePosition(node_name.c_str(), position, model);
	DirectX::XMFLOAT3 actor_position = actor->GetPosition();
	DirectX::XMFLOAT3 castam_poition = { actor_position.x, position.y - collision_object->GetLocalPosition().y, actor_position.z };
	collision_object->SetPosition(castam_poition);
}
