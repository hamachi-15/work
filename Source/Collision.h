#pragma once
#include <DirectXMath.h>
#include "Actor.h"
#include "ModelResource.h"
#include "MetaAI.h"
#include "Component.h"
class Model;
class Telegram;

// �A�N�^�[�̍��W�ōX�V���邩�N���X�����o�̍��W�ōX�V���邩
enum class CollisionPositionMask
{
	Collision_Mask_Actor_Position,
	Collision_Mask_Member_Position,
	Collision_Mask_Local_Member_Position,
};

// �R���W�������b�V���̎��
enum class CollisionMeshType
{
	Sphere,
	Cylinder,
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
	DirectX::XMVECTOR vector = {0, 0, 0};
};

struct CollisionParameter
{
	DirectX::XMFLOAT3		position = { 0, 0, 0 };
	DirectX::XMFLOAT3		local_position = { 0, 0, 0 };
	DirectX::XMFLOAT3		node_position = { 0, 0, 0 };
	const char*				name;
	const char*				node_name;
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

//-----------------------------------
// ���C�L���X�g
//-----------------------------------
class CollisionRayCast
{
public:
	CollisionRayCast() {}
	~CollisionRayCast() {}

	// ���C�ƃ��f���̌�������
	bool IntersectRayVsModel(
		const DirectX::XMFLOAT3& start,
		const DirectX::XMFLOAT3& end,
		const Model* model,
		HitResult& hit_result);

	// �ʏ�p
	int IntersectRayVsModel(const DirectX::XMVECTOR& start,
		const DirectX::XMVECTOR& dir,
		float neart,
		const ModelResource::Mesh& mesh,
		DirectX::XMVECTOR& hitposition,
		DirectX::XMVECTOR& hitnormal);

	// x����p
	int IntersectRayVsModelX(const DirectX::XMVECTOR& start,
		const DirectX::XMVECTOR& dir,
		float neart,
		const ModelResource::Mesh& mesh,
		DirectX::XMVECTOR& hitposition,
		DirectX::XMVECTOR& hitnormal);

	// y����p
	int IntersectRayVsModelY(const DirectX::XMVECTOR& start,
		const DirectX::XMVECTOR& dir,
		float neart,
		const ModelResource::Mesh& mesh,
		DirectX::XMVECTOR& hitposition,
		DirectX::XMVECTOR& hitnormal);

	// z����p
	int IntersectRayVsModelZ(const DirectX::XMVECTOR& start,
		const DirectX::XMVECTOR& dir,
		float neart,
		const ModelResource::Mesh& mesh,
		DirectX::XMVECTOR& hitposition,
		DirectX::XMVECTOR& hitnormal);

};

//-----------------------------------
// �R���W�����I�u�W�F�N�g�̊��N���X
//-----------------------------------
class CollisionObject : public Component
{
public:
	CollisionObject(){}
	~CollisionObject() override{}

	// GUI�`��
	void OnGUI() override;

	// �X�V����
	void Update(float elapsed_time) override {}

	// ���O�擾
	const char* GetName() const override { return name.c_str(); }
	// ���O�ݒ�
	void SetName(const char* name) { this->name = name; }

	// �m�[�h�̖��O�擾
	const char* GetNodeName() const { return node_name.c_str(); }
	// �m�[�h�̖��O�ݒ�
	void SetNodeName(const char* node_name) { this->node_name = node_name; }

	// �ʒu
	DirectX::XMFLOAT3 GetPosition() const { return position; }
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

	DirectX::XMFLOAT3 GetLocalPosition() const { return local_position; }
	void SetLocalPosition(const DirectX::XMFLOAT3& local_position) { this->local_position = local_position; }

	// �d��
	void SetWeight(float weight) { this->weight = weight; }
	float GetWeight() const { return weight; }

	// ���a
	void SetRadius(float radius) { this->radius = radius; }
	float GetRadius() const { return radius; }
	
	// ID���擾
	int GetActorID() const { return actor_id; }
	// ID��ݒ�
	void SetActorID(int id) { actor_id = id; }
	
	// �R���W�����̑����擾
	CollisionElement GetCollisionElement() { return element; }
	// �R���W�����̑����ݒ�
	void SetCollisionElement(CollisionElement element) { this->element = element; }
	
	// �R���W�������b�V���̎�ގ擾
	CollisionPositionMask GetPositionMask() { return position_mask; }
	// �R���W�������b�V���̎�ސݒ�
	void SetPositionMask(CollisionPositionMask mask) { this->position_mask = mask; }

	// �R���W�����t���O�擾
	bool GetCollisionFlag() { return collision_flag; }
	// �R���W�����t���O�ݒ�
	void SetCollisionFlag(bool flag) { this->collision_flag = flag; }
private:
	std::string				name;			// �R���W�����̖��O
	std::string				node_name;		// ���f���m�[�h�����p�B�R���W�������W��Actor�̍��W�ł͂Ȃ����Ɏg��
	DirectX::XMFLOAT3		position;
	DirectX::XMFLOAT3		local_position; //�@���f���m�[�h�̂Ȃ����W�ɃR���W������u���Ƃ��Ɏg��
	float					radius = 0.5f;
	float					weight = 0.0f;
	int						actor_id;		// �R���W�����������Ă���A�N�^�[��ID
	bool					collision_flag; // �����蔻����s�����̃t���O
	CollisionActorType		actor_type;
	CollisionElement		element;
	CollisionPositionMask	position_mask;
};

//-----------------------------------
// ����Փ˃I�u�W�F�N�g
//-----------------------------------
class CollisionSphere : public CollisionObject
{
public:
	CollisionSphere() {}
	~CollisionSphere()override {}

	// �X�V����
	void Update(float elapsed_time) override;
	
	// �`�揈��
	void Draw();

private:
	float				radius = 0.5f;
};

//-----------------------------------
// �~���R���W����
//-----------------------------------
class CollisionCylinder : public CollisionObject
{
public:
	CollisionCylinder() {}
	~CollisionCylinder()override {}

	// ����
	void SetHeight(float height) { this->height = height; }
	float GetHeight() const {return height; }

	// �X�V����
	void Update(float elapsed_time) override;

	// �`�揈��
	void Draw();

private:
	float				height = 0.5f;
};

//-----------------------------------
// �R���W�����}�l�[�W���[
//-----------------------------------
class CollisionManager
{
private:
	CollisionManager() {}
	~CollisionManager() {}

public:
	// �C���X�^���X�擾
	static CollisionManager& Instance()
	{
		static CollisionManager instance;
		return instance;
	}

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

	// ���b�Z�[�W����M�����Ƃ��̏���
	bool OnMessage(const Telegram& message);

	// �R���W�������X�g��S�j��
	void Destroy();

	//�@���R���W�����̓o�^
	void ReregisterSphere(std::shared_ptr<CollisionSphere> collision);

	// ���R���W�����̉���
	void UnregisterSphere(std::shared_ptr<CollisionSphere> collision);

	//�@�~���R���W�����̓o�^
	void ReregisterCylinder(std::shared_ptr<CollisionCylinder> collision);

	// �~���R���W�����̉���
	void UnregisterCylinder(std::shared_ptr<CollisionCylinder> collision);

	// �����o������
	void PushOutCollision(CollisionObject* collisionA, CollisionObject* collisionB, ObjectCollisionResult& result);

	// ���C�ƃ��f���̌�������
	bool IntersectRayVsModel(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, Model* model, HitResult& result);

	// ���O���狅�R���W�����擾
	std::shared_ptr<CollisionSphere> GetCollisionSphereFromName(const char* name);

	// ID���狅�R���W�������擾
	std::vector<std::shared_ptr<CollisionSphere>> GetCollisionSphereFromID(int id);
	// ���O����~���R���W�����擾
	std::shared_ptr<CollisionCylinder> GetCollisionCylinderFromName(const char* name);

private:
	// ���Ƌ��̌�������
	bool IntersectSphereVsSphere(CollisionSphere* sphereA, CollisionSphere* sphereB);
	// ���Ɖ~���̌�������
	bool IntersectSphereVsCylinder(CollisionSphere* sphere, CollisionCylinder* cylinder);
	// �~���Ɖ~���̌�������
	bool IntersectCylinderVsCylinder(CollisionCylinder* cylinderA, CollisionCylinder* cylinderB, ObjectCollisionResult& result);

	
private:
	CollisionRayCast raycast;
	
	std::vector<std::shared_ptr<CollisionSphere>>	spheres;// �X�V�Ȃǂ�����p�̋��R���W�����z��
	std::vector<std::shared_ptr<CollisionSphere>>	remove_spheres;// �}�l�[�W���[����폜����p�̋��R���W�����z��

	std::vector<std::shared_ptr<CollisionCylinder>> cylinderes;// �X�V�Ȃǂ�����p�̉~���R���W�����z��
	std::vector<std::shared_ptr<CollisionCylinder>> remove_cylinderes;// �}�l�[�W���[����폜����p�̉~���R���W�����z��
};
