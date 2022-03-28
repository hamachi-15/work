#pragma once
#include <DirectXMath.h>
#include "Actor.h"
#include "ModelResource.h"
#include "MetaAI.h"
#include "Component.h"
#include "CollisionDataHeader.h"

enum class CollisionUpdateType;
enum class ActorCategory;
class Model;
class Telegram;
class DebugRenderer;

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
class CollisionObject
{
public:
	// �R���X�g���N�^
	CollisionObject(const CollisionParameter& parameter) : collision_name(parameter.name), actor_name(parameter.actor_name), position(parameter.position), node_name(parameter.node_name),
		local_position(parameter.local_position), weight(parameter.weight), height(parameter.height), 
		radius(parameter.radius), xmfloat_radius(parameter.xmfloat_radius), actor_id(parameter.actor_id), actor_type(parameter.actor_type), update_type(parameter.update_type){}
	
	// �f�X�g���N�^
	virtual ~CollisionObject() {}

	// ���O�擾
	const char* GetName() const { return collision_name.c_str(); }

	// �m�[�h�̖��O�擾
	const char* GetNodeName() const { return node_name.c_str(); }

	// �R���W���������A�N�^�[�̖��O�擾
	const char* GetActorName() const { return actor_name.c_str(); }

	// �ʒu
	DirectX::XMFLOAT3 GetPosition() const { return position; }
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

	DirectX::XMFLOAT3 GetLocalPosition() const { return local_position; }
	void SetLocalPosition(const DirectX::XMFLOAT3& local_position) { this->local_position = local_position; }

	// �F�擾
	DirectX::XMFLOAT4 GetColor() const { return color; }

	// �F�ݒ�
	void SetColor() { this->color = color; }

	// �d�ݎ擾
	float GetWeight() const { return weight; }
	
	// �d�ݐݒ�
	void SetWeight(float weight) { this->weight = weight; }

	// ���a�擾
	float GetRadius() const { return radius; }
	const DirectX::XMFLOAT3& GetXMFloatRadius() const { return xmfloat_radius; }
	
	// ���a�ݒ�
	void SetRadius(float radius) { this->radius = radius; }
	void SetXMFloatRadius(const DirectX::XMFLOAT3 radius) { this->xmfloat_radius = radius; }

	// �����擾
	float GetHeight() const { return height; }

	// �����ݒ�
	void SetHeight() { this->height = height; }

	// ID���擾
	int GetActorID() const { return actor_id; }
	
	// �A�N�^�[�̎�ގ擾
	const ActorCategory& GetActorType() { return actor_type; }

	// �X�V���@�擾
	const CollisionUpdateType& GetUpdateType() const { return update_type; }

	// �R���W�����t���O�擾
	bool GetCollisionFlag() { return collision_flag; }

	// �R���W�����t���O�ݒ�
	void SetCollisionFlag(bool flag) { this->collision_flag = flag; }

	// �Փ˃t���O�擾
	bool GetHitFlag() const { return hit_flag; }
	
	//�Փ˃t���O�ݒ�
	bool SetHitFlag(bool hit_flag) { this->hit_flag = hit_flag; }

	// �Փ˃t���O�擾
	bool GetAttackFlag() const { return attack_falg; }

	//�Փ˃t���O�ݒ�
	void SetAttackFlag(bool attack_falg) { this->attack_falg = attack_falg; }

private:
	std::string				collision_name = "";
	std::string				node_name = "";		// ���f���m�[�h�����p�B�R���W�������W��Actor�̍��W�ł͂Ȃ����Ɏg��
	std::string				actor_name = "";				// �R���W���������A�N�^�[�̖��O
	DirectX::XMFLOAT3		position = {};
	DirectX::XMFLOAT3		local_position = {}; //�@���f���m�[�h�̂Ȃ����W�ɃR���W������u���Ƃ��Ɏg��
	DirectX::XMFLOAT3		xmfloat_radius = {0.0f, 0.0f, 0.0f};
	DirectX::XMFLOAT4		color = { 1.0f, 1.0f,0.0f, 1.0f };
	float					radius = 0.5f;
	float					height = 0.0f;
	float					weight = 0.0f;
	int						actor_id = 0;		// �R���W�����������Ă���A�N�^�[��ID
	bool					collision_flag = true; // �����蔻����s�����̃t���O
	bool					attack_falg = false; // �U���̓����蔻����s����
	bool					hit_flag = false;
	ActorCategory				actor_type;
	CollisionUpdateType		update_type;
};

//*************************************
// 
// �{�b�N�X�Փ˃I�u�W�F�N�g
// 
//*************************************
class CollisionBox : public CollisionObject
{
public:
	CollisionBox(CollisionParameter parameter) : CollisionObject(parameter) {}
	~CollisionBox() override {}

	// �`�揈��
	virtual void Render(DebugRenderer* renderer);
};

//*************************************
// 
// ����Փ˃I�u�W�F�N�g
// 
//*************************************
class CollisionSphere : public CollisionObject
{
public:
	CollisionSphere(CollisionParameter parameter) : CollisionObject(parameter) {}
	~CollisionSphere()override {}
	
	// �`�揈��
	virtual void Render(DebugRenderer* renderer);
};

//*************************************
// 
// �~���Փ˃I�u�W�F�N�g
// 
//*************************************
class CollisionCylinder : public CollisionObject
{
public:
	CollisionCylinder(CollisionParameter parameter) : CollisionObject(parameter) {}
	~CollisionCylinder()override {}

	// �`�揈��
	virtual void Render(DebugRenderer* renderer);
};
