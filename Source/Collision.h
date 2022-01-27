#pragma once
#include <DirectXMath.h>
#include "Actor.h"
#include "ModelResource.h"
#include "MetaAI.h"
#include "Component.h"
#include "CollisionDataHeader.h"
class Model;
class Telegram;


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
	void Update(float elapsed_time) override;

	// ���O�擾
	std::string GetName() const override { return name.c_str(); }

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

//*************************************
// 
// �{�b�N�X�Փ˃I�u�W�F�N�g
// 
//*************************************
class CollisionBox : public CollisionObject
{
public:
	CollisionBox() {}
	~CollisionBox() override {}

	// �X�V����
	//void Update(float elapsed_time) override;

	// �`�揈��
	void Draw();

	// ���a�ݒ�
	void SetRadius(const DirectX::XMFLOAT3 radius) { this->radius = radius; }

	// ���a�擾
	const DirectX::XMFLOAT3& GetRadius() const { return radius; }
private:
	DirectX::XMFLOAT3 radius;
};

//*************************************
// 
// ����Փ˃I�u�W�F�N�g
// 
//*************************************
class CollisionSphere : public CollisionObject
{
public:
	CollisionSphere() {}
	~CollisionSphere()override {}

	// �X�V����
	//void Update(float elapsed_time) override;
	
	// �`�揈��
	void Draw();

};

//*************************************
// 
// �~���Փ˃I�u�W�F�N�g
// 
//*************************************
class CollisionCylinder : public CollisionObject
{
public:
	CollisionCylinder() {}
	~CollisionCylinder()override {}

	// ����
	void SetHeight(float height) { this->height = height; }
	float GetHeight() const {return height; }

	// �X�V����
	//void Update(float elapsed_time) override;

	// �`�揈��
	void Draw();

private:
	float				height = 0.5f;
};
