#pragma once
#include <DirectXMath.h>

// �R���W�������b�V���̎��
//enum class CollisionMeshType
//{
//	Sphere,
//	Cylinder,
//	AABB
//};

// �R���W�������W�̍X�V���@
//enum class CollisionUpdateType
//{
//	Update_Actor,			// �A�N�^�[���W�ɍX�V
//	Update_Node_Position,	// ���f���̓���m�[�h���W�ɍX�V
//	Update_Local_Position,	// ����m�[�h�̃��[�J���ʒu�����[���h���W�ɕϊ����čX�V
//	Update_Castam_poition, // ��L�̂ǂ�ł��Ȃ����@�ōX�V
//};

// �R���W�����������Ă���A�N�^�[�̃^�C�v
//enum class CollisionActorType
//{
//	Enemy,
//	Player,
//	None		// ���̑�
//};
enum class ActorType;
enum class CollisionUpdateType;

//-----------------------------------
// ���茋��
//-----------------------------------
struct HitResult
{
	DirectX::XMFLOAT3 position = { 0, 0, 0 }; // ���C�ƃ|���S���̌�_
	DirectX::XMFLOAT3 normal = { 0,0,0 };	  // �Փ˂����|���S���̖@���x�N�g��
	float			  distance = 0.0f;		  // ���C�̎��_�����_�܂ł̋���
	int				  material_index = -1;	  // �Փ˂����|���S���̃}�e���A���ԍ�
};

struct ObjectCollisionResult
{
	DirectX::XMFLOAT3 positionA = { 0, 0, 0 };
	DirectX::XMFLOAT3 positionB = { 0, 0, 0 };
	DirectX::XMVECTOR vector = { 0, 0, 0 };
};

struct CollisionParameter
{
	DirectX::XMFLOAT3		position = { 0, 0, 0 };
	DirectX::XMFLOAT3		local_position = { 0, 0, 0 };
	DirectX::XMFLOAT3		node_position = { 0, 0, 0 };
	DirectX::XMFLOAT3		xmfloat_radius = { 0, 0, 0 };
	std::string				actor_name;
	std::string				name;
	std::string				node_name;
	int						actor_id = -1;		// �R���W�����������Ă���A�N�^�[��ID
	float					angle = 0.0f;
	float					radius = 0.0f;
	float					height = 0.0f;
	float					weight = 1.0f;
	bool					collision_flg = false;
	ActorType				actor_type;
	CollisionUpdateType		update_type;
};
