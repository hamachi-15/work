#pragma once
#include <DirectXMath.h>

// �A�N�^�[�̍��W�ōX�V���邩�N���X�����o�̍��W�ōX�V���邩
enum class CollisionPositionMask
{
	Collision_Mask_Actor_Position,
	Collision_Mask_Member_Position,
	Collision_Mask_Local_Member_Position,
	Collision_Mask_Castam_poition,
};

// �R���W�������b�V���̎��
enum class CollisionMeshType
{
	Sphere,
	Cylinder,
	AABB
};

// �R���W�����̑���
enum class CollisionElement
{
	Weppon,
	Body,
};

// �R���W�����������Ă���A�N�^�[�̃^�C�v
enum class CollisionActorType
{
	Enemy,
	Player,
	None		// ���̑�
};

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
	DirectX::XMFLOAT3		float3_radius = { 0, 0, 0 };
	const char* name;
	const char* node_name;
	int						actor_id = -1;		// �R���W�����������Ă���A�N�^�[��ID
	float					angle = 0.0f;
	float					radius = 0.0f;
	float					height = 0.0f;
	float					weight = 0.0f;
	bool					collision_flg = false;
	CollisionActorType		actor_type;
	CollisionElement		element;
	CollisionPositionMask	position_mask;
};
