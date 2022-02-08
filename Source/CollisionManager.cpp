#include "Graphics.h"

#include "Mathf.h"
#include "CollisionManager.h"
#include "SceneManager.h"

#include "Charactor.h"
#include "Enemy.h"

#include "CullingCollision.h"

#include "ActorManager.h"
//-----------------------------------------
// �X�V����
//-----------------------------------------
void CollisionManager::Update()
{
    // �J�����O�R���W�����j������
    for (std::shared_ptr<CullingCollision> box : remove_cullings)
    {
        std::vector<std::shared_ptr<CullingCollision>>::iterator remove = std::find(cullings.begin(), cullings.end(), box);
        cullings.erase(remove);
    }
    std::vector<std::shared_ptr<CullingCollision>>::iterator iterate_culling_remove = remove_cullings.begin();
    for (; iterate_culling_remove != remove_cullings.end(); iterate_culling_remove = remove_cullings.begin())
    {
        remove_cullings.erase(iterate_culling_remove);
    }
    remove_cullings.clear();

    // ���j������
    for (std::shared_ptr<CollisionSphere> sphere : remove_spheres)
    {
        std::vector<std::shared_ptr<CollisionSphere>>::iterator remove = std::find(spheres.begin(), spheres.end(), sphere);
        spheres.erase(remove);
    }
    std::vector<std::shared_ptr<CollisionSphere>>::iterator iterate_sphere_remove = remove_spheres.begin();
    for (; iterate_sphere_remove != remove_spheres.end(); iterate_sphere_remove = remove_spheres.begin())
    {
        remove_spheres.erase(iterate_sphere_remove);
    }
    remove_spheres.clear();

    // �~���j������
    for (std::shared_ptr<CollisionCylinder> cylinder : remove_cylinderes)
    {
        std::vector<std::shared_ptr<CollisionCylinder>>::iterator remove = std::find(cylinderes.begin(), cylinderes.end(), cylinder);
        cylinderes.erase(remove);
    }
    std::vector<std::shared_ptr<CollisionCylinder>>::iterator iterate_remove = remove_cylinderes.begin();
    for (; iterate_remove != remove_cylinderes.end(); iterate_remove = remove_cylinderes.begin())
    {
        remove_cylinderes.erase(iterate_remove);
    }
    remove_cylinderes.clear();

    // �J�����O�R���W�����X�V����
    std::vector<Plane> frustum = SceneManager::Instance().GetCurrentScene()->GetCameraController()->GetFrustum();
    for (std::shared_ptr<CullingCollision> culling : cullings)
    {
        culling->Update();
    }
}

//-----------------------------------------
// �`�揈��
//-----------------------------------------
void CollisionManager::Draw()
{
    DebugRenderer* renderer = Graphics::Instance().GetDebugRenderer();

    // �J�����O�R���W�����`��
    for (std::shared_ptr<CullingCollision> culling : cullings)
    {
        culling->Render(renderer);
    }

    // ���R���W�����`��
    for (std::shared_ptr<CollisionSphere> sphere : spheres)
    {
        if (!sphere->GetAttackFlag()) continue;
        sphere->Render(renderer);
    }

    // �~���R���W�����`��
    for (std::shared_ptr<CollisionCylinder> cylinder : cylinderes)
    {
        cylinder->Render(renderer);
    }
}

//-----------------------------------------
// ���b�Z�[�W����M�����Ƃ��̏���
//-----------------------------------------
bool CollisionManager::OnMessage(const Telegram& message)
{
    return false;
}

//-----------------------------------------
// �R���W�������X�g��S�j��
//-----------------------------------------
void CollisionManager::Destroy()
{
    // �J�����O�R���W�������X�g�̔j��
    std::vector<std::shared_ptr<CullingCollision>>::iterator iterate_box = cullings.begin();
    for (; iterate_box != cullings.end(); iterate_box = cullings.begin())
    {
        cullings.erase(iterate_box);
    }

    // �J�����O�R���W�����j�����X�g�̔j��
    std::vector<std::shared_ptr<CullingCollision>>::iterator iterate_remove_box = remove_cullings.begin();
    for (; iterate_remove_box != remove_cullings.end(); iterate_remove_box = remove_cullings.begin())
    {
        remove_cullings.erase(iterate_remove_box);
    }

    // ���R���W�������X�g�̔j��
    std::vector<std::shared_ptr<CollisionSphere>>::iterator iterate_sphere = spheres.begin();
    for (; iterate_sphere != spheres.end(); iterate_sphere = spheres.begin())
    {
        spheres.erase(iterate_sphere);
    }

    // ���R���W�����j�����X�g�̔j��
    std::vector<std::shared_ptr<CollisionSphere>>::iterator iterate_remove_sphere = remove_spheres.begin();
    for (; iterate_remove_sphere != remove_spheres.end(); iterate_remove_sphere = remove_spheres.begin())
    {
        remove_spheres.erase(iterate_remove_sphere);
    }

    // �~���R���W�������X�g�j��
    std::vector<std::shared_ptr<CollisionCylinder>>::iterator iterate_cylinder = cylinderes.begin();
    for (; iterate_cylinder != cylinderes.end(); iterate_cylinder = cylinderes.begin())
    {
        cylinderes.erase(iterate_cylinder);
    }

    // �~���R���W�����j�����X�g�j��
    std::vector<std::shared_ptr<CollisionCylinder>>::iterator iterate_remove_cylinder = remove_cylinderes.begin();
    for (; iterate_remove_cylinder != remove_cylinderes.end(); iterate_remove_cylinder = remove_cylinderes.begin())
    {
        remove_cylinderes.erase(iterate_remove_cylinder);
    }
}

//-----------------------------------------
// �J�����O�R���W�����o�^
//-----------------------------------------
void CollisionManager::RegisterCulling(std::shared_ptr<CullingCollision> collision)
{
    // �J�����O�R���W�����z��ɃR���W������ǉ�
    cullings.emplace_back(collision);
}

//-----------------------------------------
// �J�����O�R���W��������
//-----------------------------------------
void CollisionManager::UnregisterCulling(std::shared_ptr<CullingCollision> collision)
{
    // �j���p�J�����O�R���W�����z��ɃR���W������ǉ�
    remove_cullings.emplace_back(collision);
}

//-----------------------------------------
//�@���R���W�����̓o�^
//-----------------------------------------
void CollisionManager::RegisterSphere(std::shared_ptr<CollisionSphere> collision)
{
    // ���z��ɃR���W������ǉ�
    spheres.emplace_back(collision);
}

//-----------------------------------------
// ���R���W�����̉���
//-----------------------------------------
void CollisionManager::UnregisterSphere(std::shared_ptr<CollisionSphere> collision)
{
    // �j���p���z��ɃR���W������ǉ�
    remove_spheres.emplace_back(collision);
}

//-----------------------------------------
//�@�~���R���W�����̓o�^
//-----------------------------------------
void CollisionManager::RegisterCylinder(std::shared_ptr<CollisionCylinder> collision)
{
    // �~���z��ɃR���W������ǉ�
    cylinderes.emplace_back(collision);
}

//-----------------------------------------
// �~���R���W�����̉���
//-----------------------------------------
void CollisionManager::UnregisterCylinder(std::shared_ptr<CollisionCylinder> collision)
{
    // �j���p�~���z��ɃR���W������ǉ�
    remove_cylinderes.emplace_back(collision);
}

//-----------------------------------------
// �����o������
//-----------------------------------------
void CollisionManager::PushOutCollision(CollisionObject* collisionA, std::shared_ptr<Actor> collisionA_actor, CollisionObject* collisionB, std::shared_ptr<Actor> collisionB_actor, ObjectCollisionResult& result)
{
    DirectX::XMVECTOR vec_positionA = DirectX::XMLoadFloat3(&result.positionA);
    DirectX::XMVECTOR vec_positionB = DirectX::XMLoadFloat3(&result.positionB);

    // 2�̋��̏d�����牟���o���ʂ����߂�
    float rateA = collisionA->GetWeight() / (collisionA->GetWeight() + collisionB->GetWeight());
    float rateB = 1.0f - rateA;

    DirectX::XMVECTOR velocityB = DirectX::XMVectorScale(result.vector, rateA);
    vec_positionB = DirectX::XMVectorSubtract(vec_positionB, velocityB);

    // �����o��
    DirectX::XMVECTOR velocityA = DirectX::XMVectorScale(result.vector, rateB);
    vec_positionA = DirectX::XMVectorAdd(vec_positionA, velocityA);

    // �Փˌ���
    DirectX::XMStoreFloat3(&result.positionA, vec_positionA);
    DirectX::XMStoreFloat3(&result.positionB, vec_positionB);

    // ���W�X�V
    collisionA_actor->SetPosition(result.positionA);
    collisionB_actor->SetPosition(result.positionB);
}

//-----------------------------------------
// �J�����̐����AABB�̓��O����
//-----------------------------------------
bool CollisionManager::IntersectFrustumVsAABB(std::shared_ptr<CollisionBox> aabb, std::vector<Plane> frustum_list)
{
    // ����̕��ʂ̐������[�v
    int index = 0;
    for (; index < Frustum_Plan_Max; ++index)
    {
        //�e���ʂ̖@���̐�����p����AABB�̂W���_�̒�����ŋߓ_�ƍŉ��_�����߂�
        DirectX::XMFLOAT3 NegaPos = aabb->GetPosition();	// �ŋߓ_
        DirectX::XMFLOAT3 PosiPos = aabb->GetPosition();	// �ŉ��_
        // ���a�擾
        DirectX::XMFLOAT3 radius = aabb->GetXMFloatRadius();

        // ����̕��ʎ擾
        Plane frustum = frustum_list[index];

        // �ŋߓ_�Z�o
        Mathf::NegaCalculate(NegaPos, frustum.normal, radius);

        // �ŉ��_�Z�o
        Mathf::PosiCalculate(PosiPos, frustum.normal, radius);

        //  �e���ʂƂ̓��ς��v�Z���A�����E���O����(�\������)���s�� 
        //  �O���ƕ�����Ώ�����break���m�肳����
        //  ������Ԃł���΁A�X�e�[�^�X��ύX���Ă��玟�̕��ʂƂ̃`�F�b�N�ɑ�����
        //  �����ł���΁A���̂܂܎��̕��ʂƂ̃`�F�b�N�ɑ�����
        float PosiLength, NegaLength;
        DirectX::XMStoreFloat(&PosiLength, DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&frustum.normal),
            DirectX::XMLoadFloat3(&PosiPos)));
        DirectX::XMStoreFloat(&NegaLength, DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&frustum.normal),
            DirectX::XMLoadFloat3(&NegaPos)));
        
        if (PosiLength < frustum.direction &&
            NegaLength < frustum.direction) 
        { 
            break;
        }
    }
    // �{�b�N�X��������̊O�ɂ���΃J�����O���s��
    return (index != Frustum_Plan_Max);
}

//-----------------------------------------
// ���Ƌ��̌�������
//-----------------------------------------
bool CollisionManager::IntersectSphereVsSphere(CollisionSphere* sphereA, CollisionSphere* sphereB)
{
    DirectX::XMFLOAT3 positionA = sphereA->GetPosition();
    DirectX::XMFLOAT3 positionB = sphereB->GetPosition();


    DirectX::XMVECTOR vec_positionA = DirectX::XMLoadFloat3(&positionA);
    DirectX::XMVECTOR vec_positionB = DirectX::XMLoadFloat3(&positionB);
    DirectX::XMVECTOR vector = DirectX::XMVectorSubtract(vec_positionA, vec_positionB);
    DirectX::XMVECTOR vec_lengthsq = DirectX::XMVector3Length(vector);
    float			  length;
    DirectX::XMStoreFloat(&length, vec_lengthsq);

    // ��������
    float renge = sphereA->GetRadius() + sphereB->GetRadius();
    if (renge < length)
    {
        return false;
    }

    // �P�ʃx�N�g����
    vector = DirectX::XMVector3Normalize(vector);

    // �߂肱�ݗʂ����߂�
    float diff = renge - length;
    vector = DirectX::XMVectorScale(vector, diff);

    return true;
}

//-----------------------------------------
// ���Ɖ~���̌�������
//-----------------------------------------
bool CollisionManager::IntersectSphereVsCylinder(CollisionSphere* sphere, CollisionCylinder* cylinder)
{
    // ���W�擾
    DirectX::XMFLOAT3 sphere_position = sphere->GetPosition();
    DirectX::XMFLOAT3 cylinder_position = cylinder->GetPosition();
    float             sphere_radius = sphere->GetRadius();
    float             cylinder_radius = cylinder->GetRadius();
    float             cylinder_height = cylinder->GetHeight();
    //A�̑�����B�̓�����Ȃ瓖�����Ă��Ȃ�
    if (sphere_position.y - sphere_radius > cylinder_position.y + cylinder_height)
    {
        return false;
    }
    //A�̓���B�̑�����艺�Ȃ瓖�����Ă��Ȃ�
    if (sphere_position.y + sphere_radius < cylinder_position.y)
    {
        return false;
    }

    // XZ���ʂł͈̔̓`�F�b�N
    DirectX::XMVECTOR vec_sphere_position = DirectX::XMLoadFloat3(&sphere_position);
    DirectX::XMVECTOR vec_cylinder_position = DirectX::XMLoadFloat3(&cylinder_position);
    //vec_sphere_position = DirectX::XMVectorSet(cylinder_position.x, sphere_position.y, cylinder_position.z, 0.0f);
    DirectX::XMVECTOR  vector = DirectX::XMVectorSubtract(vec_sphere_position, vec_cylinder_position);
    DirectX::XMVECTOR vec_length = DirectX::XMVector3Length(vector);
    float			  length;
    DirectX::XMStoreFloat(&length, vec_length);
    float renge = sphere_radius + cylinder_radius;
    if (renge < length)
    {
        return false;
    }

    // �P�ʃx�N�g����
    vector = DirectX::XMVector3Normalize(vector);

    // �߂肱�ݗʂ����߂�
    float diff = renge - length;
    vector = DirectX::XMVectorScale(vector, diff);

    return true;
}

//-----------------------------------------
// �~���Ɖ~���̌�������
//-----------------------------------------
bool CollisionManager::IntersectCylinderVsCylinder(CollisionCylinder* cylinderA, std::shared_ptr<Actor> collisionA_actor, CollisionCylinder* cylinderB, std::shared_ptr<Actor> collisionB_actor, ObjectCollisionResult& result)
{
    // ���W�擾
    result.positionA = collisionA_actor->GetPosition();
    result.positionB = collisionB_actor->GetPosition();
    //A�̑�����B�̓�����Ȃ瓖�����Ă��Ȃ�
    if (result.positionA.y > result.positionB.y + cylinderB->GetHeight())
    {
        return false;
    }
    //A�̓���B�̑�����艺�Ȃ瓖�����Ă��Ȃ�
    if (result.positionA.y + cylinderA->GetHeight() < result.positionB.y)
    {
        return false;
    }

    // XZ���ʂł͈̔̓`�F�b�N
    DirectX::XMVECTOR vec_positionA = DirectX::XMLoadFloat3(&result.positionA);
    DirectX::XMVECTOR vec_positionB = DirectX::XMLoadFloat3(&result.positionB);
    result.vector = DirectX::XMVectorSubtract(vec_positionA, vec_positionB);
    DirectX::XMVECTOR vec_length = DirectX::XMVector3Length(result.vector);
    float			  length;
    DirectX::XMStoreFloat(&length, vec_length);
    float renge = cylinderA->GetRadius() + cylinderB->GetRadius();
    if (renge < length)
    {
        return false;
    }

    // �P�ʃx�N�g����
    result.vector = DirectX::XMVector3Normalize(result.vector);

    // �߂荞�ݗʂ����߂�
    float diff = renge - length;
    result.vector = DirectX::XMVectorScale(result.vector, diff);

    return true;
}

//-----------------------------------------
// ���C�ƃ��f���̌�������
//-----------------------------------------
bool CollisionManager::IntersectRayVsModel(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, Model* model, HitResult& result)
{
    return raycast.IntersectRayVsModel(start, end, model, result);
}

//-----------------------------------------
// ���O����J�����O�R���W�����擾
//-----------------------------------------
std::shared_ptr<CullingCollision> CollisionManager::GetCollisionCullingFromName(std::string name)
{
    for (std::shared_ptr<CullingCollision> box : cullings)
    {
        if (box->GetName() == name)
        {
            return box;
        }
    }
    return nullptr;
}

//-----------------------------------------
// ���O���狅�R���W�����擾
//-----------------------------------------
std::shared_ptr<CollisionSphere> CollisionManager::GetCollisionSphereFromName(std::string name)
{
    for (std::shared_ptr<CollisionSphere> sphere : spheres)
    {
        if (sphere->GetName() == name)
        {
            return sphere;
        }
    }
    return nullptr;
}

//-----------------------------------------
// ID�Ɩ��O���狅�R���W�����擾
//-----------------------------------------
std::shared_ptr<CollisionSphere> CollisionManager::GetCollisionSphereFromNameAndID(int id, std::string name)
{
    for (std::shared_ptr<CollisionSphere> sphere : spheres)
    {
        if (sphere->GetActorID() == id && sphere->GetName() == name)
        {
            return sphere;
        }
    }
    return nullptr;
}

//-----------------------------------------
// ID���狅�R���W�������擾
//-----------------------------------------
std::vector<std::shared_ptr<CollisionSphere>> CollisionManager::GetCollisionSphereFromID(int id)
{
    std::vector<std::shared_ptr<CollisionSphere>> searched_sphere;
    for (std::shared_ptr<CollisionSphere> sphere : spheres)
    {
        if (sphere->GetActorID() == id)
        {
            searched_sphere.emplace_back(sphere);
        }
    }
    return searched_sphere;
}

//-----------------------------------------
// ���O����~���R���W�����擾
//-----------------------------------------
std::shared_ptr<CollisionCylinder> CollisionManager::GetCollisionCylinderFromName(std::string name)
{
    for (std::shared_ptr<CollisionCylinder> cylinder : cylinderes)
    {
        if (cylinder->GetName() == name)
        {
            return cylinder;
        }
    }
    return nullptr;
}

//-----------------------------------------
// ID�Ɩ��O����~���R���W�������擾
//-----------------------------------------
std::shared_ptr<CollisionCylinder> CollisionManager::GetCollisionCylinderFromNameAndID(int id, std::string name)
{
    for (std::shared_ptr<CollisionCylinder> cylinder : cylinderes)
    {
        if (cylinder->GetActorID() == id && cylinder->GetName() == name)
        {
            return cylinder;
        }
    }
    return nullptr;
}
