#pragma once
#include <memory>
#include <vector>

#include "Collision.h"

//*************************************
// 
// �R���W�����}�l�[�W���[
// 
//*************************************
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

	// AABB�R���W�����o�^
	void RegisterBox(std::shared_ptr<CollisionBox> collision);

	// AABB�R���W��������
	void UnregisterBox(std::shared_ptr<CollisionBox> collision);

	//�@���R���W�����̓o�^
	void RegisterSphere(std::shared_ptr<CollisionSphere> collision);

	// ���R���W�����̉���
	void UnregisterSphere(std::shared_ptr<CollisionSphere> collision);

	//�@�~���R���W�����̓o�^
	void RegisterCylinder(std::shared_ptr<CollisionCylinder> collision);

	// �~���R���W�����̉���
	void UnregisterCylinder(std::shared_ptr<CollisionCylinder> collision);

	// �����o������
	void PushOutCollision(CollisionObject* collisionA, CollisionObject* collisionB, ObjectCollisionResult& result);

	// ���C�ƃ��f���̌�������
	bool IntersectRayVsModel(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, Model* model, HitResult& result);

	// ���O���痧���̃R���W�����擾
	std::shared_ptr<CollisionBox> GetCollisionBoxFromName(std::string name);

	// ���O���狅�R���W�����擾
	std::shared_ptr<CollisionSphere> GetCollisionSphereFromName(std::string name);

	// ID���狅�R���W�������擾
	std::vector<std::shared_ptr<CollisionSphere>> GetCollisionSphereFromID(int id);

	// ID�Ɩ��O���狅�R���W�������擾
	std::shared_ptr<CollisionSphere> GetCollisionSphereFromNameAndID(int id, std::string name);

	// ���O����~���R���W�����擾
	std::shared_ptr<CollisionCylinder> GetCollisionCylinderFromName(std::string name);

	// ID�Ɩ��O����~���R���W�������擾
	std::shared_ptr<CollisionCylinder> GetCollisionCylinderFromNameAndID(int id, std::string name);

private:
	// ���Ƌ��̌�������
	bool IntersectSphereVsSphere(CollisionSphere* sphereA, CollisionSphere* sphereB);
	// ���Ɖ~���̌�������
	bool IntersectSphereVsCylinder(CollisionSphere* sphere, CollisionCylinder* cylinder);
	// �~���Ɖ~���̌�������
	bool IntersectCylinderVsCylinder(CollisionCylinder* cylinderA, CollisionCylinder* cylinderB, ObjectCollisionResult& result);


private:
	CollisionRayCast raycast;

	std::vector<std::shared_ptr<CollisionBox>> boxes;// �X�V�Ȃǂ�����p�̉~���R���W�����z��
	std::vector<std::shared_ptr<CollisionBox>> remove_boxes;// �}�l�[�W���[����폜����p�̉~���R���W�����z��

	std::vector<std::shared_ptr<CollisionSphere>>	spheres;// �X�V�Ȃǂ�����p�̋��R���W�����z��
	std::vector<std::shared_ptr<CollisionSphere>>	remove_spheres;// �}�l�[�W���[����폜����p�̋��R���W�����z��

	std::vector<std::shared_ptr<CollisionCylinder>> cylinderes;// �X�V�Ȃǂ�����p�̉~���R���W�����z��
	std::vector<std::shared_ptr<CollisionCylinder>> remove_cylinderes;// �}�l�[�W���[����폜����p�̉~���R���W�����z��
};
