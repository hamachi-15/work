#pragma once
#include <memory>
#include <map>
#include <vector>

#include "Collision.h"

class DebugRenderer;
struct Plane;
class CullingCollision;

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

	// ���R���W�������X�g�擾
	std::vector<std::shared_ptr<CollisionSphere>> GetCollisionSpheres() const { return spheres; }

	// �~���R���W�������X�g�擾
	std::vector<std::shared_ptr<CollisionCylinder>> GetCollisionCylinderes() const { return cylinderes; }
	
	// �J�����O�R���W�����o�^
	void RegisterCulling(std::shared_ptr<CullingCollision> culling);

	// �J�����O�R���W��������
	void UnregisterCulling(std::shared_ptr<CullingCollision> culling);

	//�@���R���W�����̓o�^
	void RegisterSphere(std::shared_ptr<CollisionSphere> collision);

	// ���R���W�����̉���
	void UnregisterSphere(std::shared_ptr<CollisionSphere> collision);

	//�@�~���R���W�����̓o�^
	void RegisterCylinder(std::shared_ptr<CollisionCylinder> collision);

	// �~���R���W�����̉���
	void UnregisterCylinder(std::shared_ptr<CollisionCylinder> collision);

	// �����o������
	void PushOutCollision(CollisionObject* collisionA, std::shared_ptr<Actor> collisionA_actor, CollisionObject* collisionB, std::shared_ptr<Actor> collisionB_actor, ObjectCollisionResult& result);

	// ���C�ƃ��f���̌�������
	bool IntersectRayVsModel(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, Model* model, HitResult& result);

	// ���O����J�����O�R���W�����擾
	std::shared_ptr<CullingCollision> GetCollisionCullingFromName(std::string name);

	// ���R���W�����̐��擾
	size_t GetCollisionSphereCount() const { return spheres.size(); }

	// �C���f�b�N�X���狅�R���W�����擾
	std::shared_ptr<CollisionSphere> GetCollisionSphere(int index) { return spheres.at(index); }
	
	// ���O���狅�R���W�����擾
	std::shared_ptr<CollisionSphere> GetCollisionSphereFromName(std::string name);

	// ID���狅�R���W�������擾
	std::vector<std::shared_ptr<CollisionSphere>> GetCollisionSphereFromID(int id);

	// ID�Ɩ��O���狅�R���W�������擾
	std::shared_ptr<CollisionSphere> GetCollisionSphereFromNameAndID(int id, std::string name);

	// �~���R���W�����̐��擾
	size_t GetCollisionCylinderCount() const { return cylinderes.size(); }
	
	// �C���f�b�N�X����~���R���W�����擾
	std::shared_ptr<CollisionCylinder> GetCollisionCylinder(int index) { return cylinderes.at(index); }
	
	// ���O����~���R���W�����擾
	std::shared_ptr<CollisionCylinder> GetCollisionCylinderFromName(std::string name);

	// ID�Ɩ��O����~���R���W�������擾
	std::shared_ptr<CollisionCylinder> GetCollisionCylinderFromNameAndID(int id, std::string name);
	
	// �J�����̐����AABB�̓��O����
	bool IntersectFrustumVsAABB(std::shared_ptr<CollisionBox> aabb, std::vector<Plane> frustum_list);

	// ���Ƌ��̌�������
	bool IntersectSphereVsSphere(CollisionSphere* sphereA, CollisionSphere* sphereB);

	// ���Ɖ~���̌�������
	bool IntersectSphereVsCylinder(CollisionSphere* sphere, CollisionCylinder* cylinder, ObjectCollisionResult& result);

	// �~���Ɖ~���̌�������
	/// ����------------------------------
	/// �~���R���W����A
	/// �~���R���W����A�������Ă���A�N�^�[
	/// �~���R���W����B
	/// �~���R���W����B�������Ă���A�N�^�[
	/// �����蔻��̃��U���g���i�[����
	/// ----------------------------------
	bool IntersectCylinderVsCylinder(CollisionCylinder* cylinderA, std::shared_ptr<Actor> collisionA_actor,
		CollisionCylinder* cylinderB, std::shared_ptr<Actor> collisionB_actor, 
		ObjectCollisionResult& result);


private:
	static const int Frustum_Plan_Max = 6; // �l����̕��ʂ̐�
private:
	CollisionRayCast raycast;

	std::vector<std::shared_ptr<CullingCollision>> cullings;//�J�����O�R���W�����z��
	std::vector<std::shared_ptr<CullingCollision>> remove_cullings;// �}�l�[�W���[����폜����p�̃J�����O�R���W�����z��

	std::vector<std::shared_ptr<CollisionSphere>>	spheres;// �X�V�Ȃǂ�����p�̋��R���W�����z��
	std::vector<std::shared_ptr<CollisionSphere>>	remove_spheres;// �}�l�[�W���[����폜����p�̋��R���W�����z��

	std::vector<std::shared_ptr<CollisionCylinder>> cylinderes;// �X�V�Ȃǂ�����p�̉~���R���W�����z��
	std::vector<std::shared_ptr<CollisionCylinder>> remove_cylinderes;// �}�l�[�W���[����폜����p�̉~���R���W�����z��
};
