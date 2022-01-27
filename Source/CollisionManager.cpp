#include "CollisionManager.h"

#include "Mathf.h"
#include "SceneManager.h"

#include "Charactor.h"
#include "Enemy.h"

//-----------------------------------------
// �X�V����
//-----------------------------------------
void CollisionManager::Update()
{
    // �{�b�N�X�j������
    for (std::shared_ptr<CollisionBox> box : remove_boxes)
    {
        std::vector<std::shared_ptr<CollisionBox>>::iterator remove = std::find(boxes.begin(), boxes.end(), box);
        boxes.erase(remove);
    }
    std::vector<std::shared_ptr<CollisionBox>>::iterator iterate_box_remove = remove_boxes.begin();
    for (; iterate_box_remove != remove_boxes.end(); iterate_box_remove = remove_boxes.begin())
    {
        remove_boxes.erase(iterate_box_remove);
    }
    remove_boxes.clear();

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

    ObjectCollisionResult result;

    // �R���W�������J�����̐�����ɂ��邩�̔���
    {
        Scene* scene = SceneManager::Instance().GetCurrentScene();
        size_t box_count = boxes.size();
        for (std::shared_ptr<CollisionBox> aabb : boxes)
        {
            // �J�����O���s�����̃t���O
            bool culling_flag = true;
            int index = 0;
            for (; index < 6; ++index)
            {
                //�e���ʂ̖@���̐�����p����AABB�̂W���_�̒�����ŋߓ_�ƍŉ��_�����߂�
                DirectX::XMFLOAT3 NegaPos = aabb->GetActor()->GetPosition();	// �ŋߓ_
                DirectX::XMFLOAT3 PosiPos = aabb->GetActor()->GetPosition();	// �ŉ��_
                // ���a�擾
                DirectX::XMFLOAT3 radius = aabb->GetRadius();

                // ����̕��ʎ擾
                Plane frustum = scene->camera_controller->frustum[index];

                // �ŋߓ_�Z�o
                Mathf::NegaCalculate(NegaPos, frustum.normal, radius);

                // �ŉ��_�Z�o
                Mathf::PosiCalculate(PosiPos, frustum.normal, radius);

                //  �e���ʂƂ̓��ς��v�Z���A�����E���O����(�\������)���s�� 
                //  �O���ƕ�����Ώ�����break���m�肳����
                //  ������Ԃł���΁A�X�e�[�^�X��ύX���Ă��玟�̕��ʂƂ̃`�F�b�N�ɑ�����
                //  �����ł���΁A���̂܂܎��̕��ʂƂ̃`�F�b�N�ɑ�����
                float PosiLength, NegaLength;
                DirectX::XMStoreFloat(&PosiLength, DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&frustum.normal), DirectX::XMLoadFloat3(&PosiPos)));
                DirectX::XMStoreFloat(&NegaLength, DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&frustum.normal), DirectX::XMLoadFloat3(&NegaPos)));
                if (PosiLength < frustum.direction && NegaLength < frustum.direction)
                {
                    break;
                }
            }
            // �{�b�N�X������O�ɂ���΃J�����O���s��
            if (index != 6)
            {
                aabb->GetActor()->SetCullingFlag(true);
            }
            else
            {
                aabb->GetActor()->SetCullingFlag(false);
            }
        }
    }

    // ��vs���̌�������
    {
        size_t sphere_count = spheres.size();
        for (size_t i = 0; i < sphere_count; i++)
        {
            CollisionSphere* sphereA = spheres.at(i).get();
            // �����蔻��t���O�������Ă��Ȃ��������΂�
            if (!sphereA->GetCollisionFlag())
            {
                continue;
            }
            for (size_t j = i + 1; j < sphere_count; j++)
            {
                CollisionSphere* sphereB = spheres.at(j).get();
                // �����蔻��t���O�������Ă��Ȃ��������΂�
                if (!sphereB->GetCollisionFlag())
                {
                    continue;
                }

                IntersectSphereVsSphere(sphereA, sphereB);

            }
        }
    }
    // ��vs�~���̌�������
    // TODO ���U���g���l�����������蔻��ɕύX
    {
        size_t sphere_count = spheres.size();
        size_t cylinder_count = cylinderes.size();
        for (size_t i = 0; i < sphere_count; i++)
        {
            CollisionSphere* sphere = spheres.at(i).get();
            // �����蔻��t���O�������Ă��Ȃ��������΂�
            if (!sphere->GetCollisionFlag())
            {
                continue;
            }
            // ���R���W�����̃L�����N�^�R���|�[�l���g�擾
            std::shared_ptr<Charactor> sphere_charactor_component = sphere->GetActor()->GetComponent<Charactor>();

            for (size_t j = 0; j < cylinder_count; j++)
            {
                CollisionCylinder* cylinder = cylinderes.at(j).get();
                // �����蔻��t���O�������Ă��Ȃ��������΂�
                if (!cylinder->GetCollisionFlag() ||
                    sphere_charactor_component->GetID() == cylinder->GetActor()->GetComponent<Charactor>()->GetID())
                {
                    continue;
                }
                if (IntersectSphereVsCylinder(sphere, cylinder))
                {
                    // �R���W�����̑���������Ȃ�m�b�N�o�b�N
                    if (sphere->GetCollisionElement() == CollisionElement::Weppon)
                    {
                        // ����ƏՓ˂����R���W�����̎�����ɏՓ˂������Ƃ𑗐M����
                        Message message;
                        message.message = MessageType::Message_GetHit_Attack;
                        message.hit_position = sphere->GetPosition();
                        MetaAI::Instance().SendMessaging(
                            static_cast<int>(MetaAI::Identity::Collision), // ���M��
                            static_cast<int>(cylinder->GetActorID()),      // ��M��
                            message);                                      // ���b�Z�[�W
                        // �U���������������Ƃ𕐊�̃R���W�����̎�����ɑ��M����
                        message.message = MessageType::Message_Hit_Attack;
                        message.hit_position = { 0.0f, 0.0f, 0.0f };
                        MetaAI::Instance().SendMessaging(
                            static_cast<int>(MetaAI::Identity::Collision),   // ���M��
                            static_cast<int>(sphere->GetActorID()),          // ��M��
                            message);                                        // ���b�Z�[�W
                    }
                }
            }
        }
    }
    // �~��vs�~���̌�������
    {
        size_t cylinder_count = cylinderes.size();
        for (size_t i = 0; i < cylinder_count; i++)
        {
            CollisionCylinder* cylinderA = cylinderes.at(i).get();
            // �����蔻��t���O�������Ă��Ȃ��������΂�
            if (!cylinderA->GetCollisionFlag())
            {
                continue;
            }

            for (size_t j = i + 1; j < cylinder_count; j++)
            {
                CollisionCylinder* cylinderB = cylinderes.at(j).get();
                // �����蔻��t���O�������Ă��Ȃ��������΂�
                if (!cylinderB->GetCollisionFlag())
                {
                    continue;
                }

                if (IntersectCylinderVsCylinder(cylinderA, cylinderB, result))
                {
                    PushOutCollision(cylinderA, cylinderB, result);
                    // ���݂̃V�[�������[���h�}�b�v�Ȃ�V�[���֓G�ƃG���J�E���g���������b�Z�[�W�𑗂�
                    const char* scene_name = SceneManager::Instance().GetCurrentScene()->GetName();
                    int isplayercolA = strcmp(cylinderA->GetName().c_str(), "Player");
                    int isplayercolB = strcmp(cylinderB->GetName().c_str(), "Player");
                    int isworldmap = strcmp(scene_name, "SceneWorldMap");
                    if (isplayercolA == 0 && isworldmap == 0 ||
                        isplayercolB == 0 && isworldmap == 0)
                    {
                        Message message;
                        message.message = MessageType::Message_Hit_Boddy;
                        // �Փ˂����G�̍��W��ݒ�
                        if (isplayercolA == 0)
                        {
                            message.hit_position = cylinderA->GetActor()->GetPosition();
                            message.territory_tag = cylinderB->GetActor()->GetComponent<Enemy>()->GetBelongingToTerritory();
                        }
                        else
                        {
                            message.hit_position = cylinderB->GetActor()->GetPosition();
                            message.territory_tag = cylinderA->GetActor()->GetComponent<Enemy>()->GetBelongingToTerritory();
                        }
                        MetaAI::Instance().SendMessaging(
                            static_cast<int>(MetaAI::Identity::Collision),   // ���M��
                            static_cast<int>(MetaAI::Identity::WorldMap),    // ��M��
                            message);                                        // ���b�Z�[�W
                        return;
                    }
                }
            }
        }
    }
}

//-----------------------------------------
// �`�揈��
//-----------------------------------------
void CollisionManager::Draw()
{
    for (std::shared_ptr<CollisionBox> box : boxes)
    {
        box->Draw();
    }
    for (std::shared_ptr<CollisionSphere> sphere : spheres)
    {
        sphere->Draw();
    }
    for (std::shared_ptr<CollisionCylinder> cylinder : cylinderes)
    {
        cylinder->Draw();
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
    // �����̃R���W�������X�g�̔j��
    std::vector<std::shared_ptr<CollisionBox>>::iterator iterate_box = boxes.begin();
    for (; iterate_box != boxes.end(); iterate_box = boxes.begin())
    {
        boxes.erase(iterate_box);
    }

    // ���R���W�������X�g�̔j��
    std::vector<std::shared_ptr<CollisionSphere>>::iterator iterate_sphere = spheres.begin();
    for (; iterate_sphere != spheres.end(); iterate_sphere = spheres.begin())
    {
        spheres.erase(iterate_sphere);
    }

    // �~���R���W�������X�g�j��
    std::vector<std::shared_ptr<CollisionCylinder>>::iterator iterate_cylinder = cylinderes.begin();
    for (; iterate_cylinder != cylinderes.end(); iterate_cylinder = cylinderes.begin())
    {
        cylinderes.erase(iterate_cylinder);
    }
}

//-----------------------------------------
// AABB�R���W�����o�^
//-----------------------------------------
void CollisionManager::RegisterBox(std::shared_ptr<CollisionBox> collision)
{
    // AABB�z��ɃR���W������ǉ�
    boxes.emplace_back(collision);
}

//-----------------------------------------
// AABB�R���W��������
//-----------------------------------------
void CollisionManager::UnregisterBox(std::shared_ptr<CollisionBox> collision)
{
    // �j���pAABB�z��ɃR���W������ǉ�
    remove_boxes.emplace_back(collision);
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
void CollisionManager::PushOutCollision(CollisionObject* collisionA, CollisionObject* collisionB, ObjectCollisionResult& result)
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
    collisionA->GetActor()->SetPosition(result.positionA);
    collisionB->GetActor()->SetPosition(result.positionB);
}

//-----------------------------------------
// ���Ƌ��̌�������
//-----------------------------------------
bool CollisionManager::IntersectSphereVsSphere(CollisionSphere* sphereA, CollisionSphere* sphereB)
{
    DirectX::XMFLOAT3 positionA;
    DirectX::XMFLOAT3 positionB;
    if (sphereA->GetPositionMask() == CollisionPositionMask::Collision_Mask_Actor_Position)
    {
        positionA = sphereA->GetActor()->GetPosition();
    }
    else
    {
        positionA = sphereA->GetPosition();
    }
    if (sphereB->GetPositionMask() == CollisionPositionMask::Collision_Mask_Actor_Position)
    {
        positionB = sphereB->GetActor()->GetPosition();
    }
    else
    {
        positionB = sphereB->GetPosition();
    }

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

    //  PushOutCollision(sphereA, positionA, vec_positionA, sphereB, positionB, vec_positionB, vector, diff);

    return true;
}

//-----------------------------------------
// ���Ɖ~���̌�������
//-----------------------------------------
bool CollisionManager::IntersectSphereVsCylinder(CollisionSphere* sphere, CollisionCylinder* cylinder)
{
    // ���W�擾
    DirectX::XMFLOAT3 sphere_position = sphere->GetPosition();
    DirectX::XMFLOAT3 cylinder_position = cylinder->GetActor()->GetPosition();
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

    //PushOutCollision(sphere, cylinder, result);

    return true;
}

//-----------------------------------------
// �~���Ɖ~���̌�������
//-----------------------------------------
bool CollisionManager::IntersectCylinderVsCylinder(CollisionCylinder* cylinderA, CollisionCylinder* cylinderB, ObjectCollisionResult& result)
{
    // ���W�擾
    result.positionA = cylinderA->GetActor()->GetPosition();
    result.positionB = cylinderB->GetActor()->GetPosition();
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

    //PushOutCollision(cylinderA, cylinderB, result);

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
// ���O���痧���̃R���W�����擾
//-----------------------------------------
std::shared_ptr<CollisionBox> CollisionManager::GetCollisionBoxFromName(std::string name)
{
    for (std::shared_ptr<CollisionBox> box : boxes)
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
