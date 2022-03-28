#pragma once
#include <DirectXMath.h>
#include <string>
#include "Model.h"


// �O���錾
struct AttackCollitionTime;
class Actor;
class Enemy;
class CollisionObject;

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

	// �R���W�������W���w��̃m�[�h���W�֍X�V
	static void NodePositionUpdate(CollisionObject* collision_object, std::string node_name, Model* model);
	
	// �R���W�������W���w��̃��[�J�����W�֍X�V
	static void LocalPositionUpdate(CollisionObject* collision_object, Model::Node* node);
	
	// �R���W�������W���w��̃A�N�^�[���W�֍X�V
	static void ActorPositionUpdate(CollisionObject* collision_object, std::shared_ptr<Actor> actor);

	// �R���W�������W���w��̃A�N�^�[�A�w��̃��[�J�����W�ł͂Ȃ����W�֍X�V
	static void CastamPositionUpdate(CollisionObject* collision_object, std::shared_ptr<Actor> actor, std::string node_name, Model* model);
};