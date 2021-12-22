#include "Collision.h"
#include "Graphics.h"
#include "ActorManager.h"
#include "Charactor.h"
#include "SceneManager.h"

//-----------------------------------------
// �X�V����
//-----------------------------------------
void CollisionSphere::Update(float elapsed_time)
{
    std::shared_ptr<Actor> actor = GetActor();

    Model* model = actor->GetModel();
    // �R���W�����t���O�������Ă��Ȃ���͍X�V���Ȃ�
    if (GetCollisionFlag() == false) return;

    switch (GetPositionMask())
    {
        // �N���X�����o�̍��W�̍X�V
    case CollisionPositionMask::Collision_Mask_Member_Position:
        {
        const char* n = GetNodeName();
            Model::Node* node = model->FindNode(n);
            DirectX::XMFLOAT3 position = {
                node->world_transform._41,
                node->world_transform._42,
                node->world_transform._43 };
            SetPosition(position);
        }
        break;
        // ���[�J�����W����̍��W�̍X�V
    case CollisionPositionMask::Collision_Mask_Local_Member_Position:
        {
        // TODO ���t�@�N�^�����O
            DirectX::XMFLOAT3 collision_position;
            Model::Node* node = model->FindNode(GetNodeName());
            DirectX::XMMATRIX world_transform_matrix = DirectX::XMLoadFloat4x4(&node->world_transform);
            DirectX::XMFLOAT3 local_position = GetLocalPosition();
            DirectX::XMVECTOR position = DirectX::XMVector3TransformCoord(
                DirectX::XMLoadFloat3(&local_position), world_transform_matrix);
            DirectX::XMStoreFloat3(&collision_position, position);
            SetPosition(collision_position);
        }
        break;
    }
}

//-----------------------------------------
// �`�揈��
//-----------------------------------------
void CollisionSphere::Draw()
{
    DebugRenderer* renderer = Graphics::Instance().GetDebugRenderer();
    // ���W�X�V�}�X�N���A�N�^�[���W���Q�ƂɂȂ��Ă���Ȃ�A�N�^�[���W�ōX�V
    if (GetPositionMask() == CollisionPositionMask::Collision_Mask_Actor_Position && GetCollisionFlag())
    {
        renderer->DrawSphere(GetActor()->GetPosition(), GetRadius(), { 0.0f, 1.0f, 0.0f, 1.0f });
    }else if (GetPositionMask() == CollisionPositionMask::Collision_Mask_Local_Member_Position && GetCollisionFlag()||
        GetPositionMask() == CollisionPositionMask::Collision_Mask_Member_Position && GetCollisionFlag())
    {
        renderer->DrawSphere(GetPosition(), GetRadius(), { 0.0f, 1.0f, 0.0f, 1.0f });
    }
}

//-----------------------------------------
// �X�V����
//-----------------------------------------
void CollisionCylinder::Update(float elapsed_time)
{
    Model* model = GetActor()->GetModel();

    // �R���W�����t���O�������Ă��Ȃ���͍X�V���Ȃ�
    if (GetCollisionFlag() == false) return;

    switch (GetPositionMask())
    {
        // �N���X�����o�̍��W�̍X�V
    case CollisionPositionMask::Collision_Mask_Member_Position:
    {
        Model::Node* node = model->FindNode(GetNodeName());
        DirectX::XMFLOAT3 position = {
            node->world_transform._41,
            node->world_transform._42,
            node->world_transform._43 };
        SetPosition(position);
    }
    break;
    // ���[�J�����W����̍��W�̍X�V
    case CollisionPositionMask::Collision_Mask_Local_Member_Position:
    {
        DirectX::XMFLOAT3 collision_position;
        Model::Node* node = model->FindNode(GetNodeName());
        DirectX::XMMATRIX world_transform_matrix = DirectX::XMLoadFloat4x4(&node->world_transform);
        DirectX::XMFLOAT3 local_position = GetLocalPosition();
        DirectX::XMVECTOR position = DirectX::XMVector3TransformCoord(
            DirectX::XMLoadFloat3(&local_position), world_transform_matrix);
        DirectX::XMStoreFloat3(&collision_position, position);
        SetPosition(collision_position);
    }
    break;
    }
}

//-----------------------------------------
// �`�揈��
//-----------------------------------------
void CollisionCylinder::Draw()
{
    DebugRenderer* renderer = Graphics::Instance().GetDebugRenderer();
    if (GetPositionMask() == CollisionPositionMask::Collision_Mask_Actor_Position)
    {
        renderer->DrawCylinder(GetActor()->GetPosition(), GetRadius(), GetHeight(), { 1.0f, 0.0f, 0.0f, 1.0f });
    }
    else
    {
        renderer->DrawCylinder(GetPosition(), GetRadius(), GetHeight(), { 1.0f, 0.0f, 0.0f, 1.0f });
    }
}

//-----------------------------------------
// �X�V����
//-----------------------------------------
void CollisionManager::Update()
{
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
                        message.hit_position = {0.0f, 0.0f, 0.0f};
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
   // ObjectCollisionResult result;
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
                    int isplayercolA = strcmp(cylinderA->GetName(), "Player");
                    int isplayercolB = strcmp(cylinderB->GetName(), "Player");
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
                        }
                        else
                        {
                            message.hit_position = cylinderB->GetActor()->GetPosition();
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
    std::vector<std::shared_ptr<CollisionSphere>>::iterator iterate_sphere = spheres.begin();
    for (; iterate_sphere != spheres.end(); iterate_sphere = spheres.begin())
    {
        spheres.erase(iterate_sphere);
    }
    std::vector<std::shared_ptr<CollisionCylinder>>::iterator iterate_cylindere = cylinderes.begin();
    for (; iterate_cylindere != cylinderes.end(); iterate_cylindere = cylinderes.begin())
    {
        cylinderes.erase(iterate_cylindere);
    }
}

//-----------------------------------------
//�@���R���W�����̓o�^
//-----------------------------------------
void CollisionManager::ReregisterSphere(std::shared_ptr<CollisionSphere> collision)
{
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
void CollisionManager::ReregisterCylinder(std::shared_ptr<CollisionCylinder> collision)
{
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
    if(sphereA->GetPositionMask() == CollisionPositionMask::Collision_Mask_Actor_Position)
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
// ���O���狅�R���W�����擾
//-----------------------------------------
std::shared_ptr<CollisionSphere> CollisionManager::GetCollisionSphereFromName(const char* name)
{
    for (std::shared_ptr<CollisionSphere> sphere : spheres)
    {
        if (strcmp(sphere->GetName(), name) == 0)
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
std::shared_ptr<CollisionCylinder> CollisionManager::GetCollisionCylinderFromName(const char* name)
{
    for (std::shared_ptr<CollisionCylinder> cylinder : cylinderes)
    {
        if (strcmp(cylinder->GetName(), name) == 0)
        {
            return cylinder;
        }
    }
    return nullptr;
}

//-----------------------------------------
// ���C�ƃ��f���̌�������
//-----------------------------------------
bool CollisionRayCast::IntersectRayVsModel(const DirectX::XMFLOAT3& start,
    const DirectX::XMFLOAT3& end, 
    const Model* model,
    HitResult& hit_result)
{
    // ���C�̃x�N�g���ƃx�N�g���̒����̎Z�o
    DirectX::XMVECTOR world_start = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR world_end = DirectX::XMLoadFloat3(&end);
    DirectX::XMVECTOR world_ray_vec = DirectX::XMVectorSubtract(world_end, world_start);
    DirectX::XMVECTOR world_ray_length = DirectX::XMVector3Length(world_ray_vec);

    // ���[���h��Ԃ̃��C�̒���
    DirectX::XMStoreFloat(&hit_result.distance, world_ray_length);

    bool hit = false;
    const ModelResource* resource = model->GetResource();
    for (const ModelResource::Mesh& mesh : resource->GetMeshes())
    {
        const Model::Node node = model->GetNodes().at(mesh.nodeIndex);

        // ���C�����[���h��Ԃ��烍�[�J����Ԃ֕ϊ�
        DirectX::XMMATRIX world_transform = DirectX::XMLoadFloat4x4(&node.world_transform);
        DirectX::XMMATRIX inverse_world_transform = DirectX::XMMatrixInverse(nullptr, world_transform);

        DirectX::XMVECTOR start = DirectX::XMVector3TransformCoord(world_start, inverse_world_transform);
        DirectX::XMVECTOR end = DirectX::XMVector3TransformCoord(world_end, inverse_world_transform);
        DirectX::XMVECTOR vec = DirectX::XMVectorSubtract(end, start);
        DirectX::XMVECTOR dir = DirectX::XMVector3Normalize(vec);
        DirectX::XMVECTOR length = DirectX::XMVector3Length(vec);

        // ���C�̒���
        float neart;
        DirectX::XMStoreFloat(&neart, length);

        // �O�p�`(��)�Ƃ̌�������
        const std::vector<ModelResource::Vertex>& vertices = mesh.vertices;
        const std::vector<UINT>& indices = mesh.indices;

        int material_index = -1;
        DirectX::XMVECTOR hit_position;
        DirectX::XMVECTOR hit_normal;
        for (const ModelResource::Subset& subset : mesh.subsets)
        {
            for (UINT i = 0; i < subset.indexCount; i += 3)
            {
                UINT index = subset.startIndex + i;
                // �O�p�`�̒��_�𒊏o
                const ModelResource::Vertex& a = vertices.at(indices.at(index));
                const ModelResource::Vertex& b = vertices.at(indices.at(index + 1));
                const ModelResource::Vertex& c = vertices.at(indices.at(index + 2));

                DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&a.position);
                DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&b.position);
                DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&c.position);

                // �O�p�`�̎O�Ӄx�N�g�����Z�o
                DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
                DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
                DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(A, C);

                // �O�p�`�̖@���x�N�g�����Z�o
                DirectX::XMVECTOR normal = DirectX::XMVector3Cross(AB, BC);

                // ���ς̌��ʂ����Ȃ痠����
                DirectX::XMVECTOR _dot = DirectX::XMVector3Dot(dir, normal);
                float dot;
                DirectX::XMStoreFloat(&dot, _dot);
                if (dot >= 0.0f) continue;

                // ���C�Ƃ̕��ʂ̌�_���Z�o
                DirectX::XMVECTOR V = DirectX::XMVectorSubtract(A, start);
                DirectX::XMVECTOR T = DirectX::XMVectorDivide(DirectX::XMVector3Dot(normal, V), _dot);
                float t;
                DirectX::XMStoreFloat(&t, T);

                // ��_�܂ł̋��������܂łɌv�Z�����ŋߋ������傫���Ƃ��̓X�L�b�v
                if (t < 0.0f || t > neart) continue;

                DirectX::XMVECTOR position = position = DirectX::XMVectorAdd(DirectX::XMVectorMultiply(dir, T), start);

                // ��_���O�p�`�̓����ɂ��邩����
                // ���
                DirectX::XMVECTOR V1 = DirectX::XMVectorSubtract(A, position);
                DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(V1, AB);
                DirectX::XMVECTOR Dot1 = DirectX::XMVector3Dot(Cross1, normal);
                float dot1;
                DirectX::XMStoreFloat(&dot1, Dot1);
                if (dot1 < 0.0f)continue;

                // ���
                DirectX::XMVECTOR V2 = DirectX::XMVectorSubtract(B, position);
                DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(V2, BC);
                DirectX::XMVECTOR Dot2 = DirectX::XMVector3Dot(Cross2, normal);
                float dot2;
                DirectX::XMStoreFloat(&dot2, Dot2);
                if (dot2 < 0.0f)continue;

                // �O��
                DirectX::XMVECTOR V3 = DirectX::XMVectorSubtract(C, position);
                DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(V3, CA);
                DirectX::XMVECTOR Dot3 = DirectX::XMVector3Dot(Cross3, normal);
                float dot3;
                DirectX::XMStoreFloat(&dot3, Dot3);
                if (dot3 < 0.0f)continue;

                // �ŋߋ������X�V
                neart = t;

                // ��_�Ɩ@�����X�V
                hit_position = position;
                hit_normal = normal;
                material_index = subset.materialIndex;
            }
        }
        if (material_index >= 0)
        {
            // ���[�J����Ԃ��烏�[���h��Ԃ֕ϊ�
            DirectX::XMVECTOR world_position = DirectX::XMVector3TransformCoord(hit_position, world_transform);
            DirectX::XMVECTOR world_cross_vec = DirectX::XMVectorSubtract(world_position, world_start);
            DirectX::XMVECTOR world_cross_length = DirectX::XMVector3Length(world_cross_vec);
            float distance;
            DirectX::XMStoreFloat(&distance, world_cross_length);
            // �q�b�g���ۑ�
            if (hit_result.distance > distance)
            {
                DirectX::XMVECTOR WorldNormal = DirectX::XMVector3Transform(hit_normal, world_transform);
                hit_result.distance = distance;
                hit_result.material_index = material_index;
                DirectX::XMStoreFloat3(&hit_result.position, world_position);
                DirectX::XMStoreFloat3(&hit_result.normal, DirectX::XMVector3Normalize(WorldNormal));
                hit = true;
            }
        }
    }
    return hit;
}

//-----------------------------------------
// �ʏ�p
//-----------------------------------------
int CollisionRayCast::IntersectRayVsModel(const DirectX::XMVECTOR& start, 
    const DirectX::XMVECTOR& dir, 
    float neart, 
    const ModelResource::Mesh& mesh,
    DirectX::XMVECTOR& hit_position,
    DirectX::XMVECTOR& hit_normal)
{
    const std::vector<ModelResource::Vertex>& vertices = mesh.vertices;
    const std::vector<UINT>& indices = mesh.indices;
    const ModelResource::Vertex* vertices_data = vertices.data();
    const UINT* indices_data = indices.data();

    int material_index = -1;
    for (const ModelResource::Subset& subset : mesh.subsets)
    {
        for (UINT i = 0; i < subset.indexCount; i += 3)
        {
            UINT index = subset.startIndex + i;
            const ModelResource::Vertex& a = vertices_data[indices_data[index + 0]];
            const ModelResource::Vertex& b = vertices_data[indices_data[index + 1]];
            const ModelResource::Vertex& c = vertices_data[indices_data[index + 2]];


            DirectX::XMVECTOR _a = DirectX::XMLoadFloat3(&a.position);
            DirectX::XMVECTOR _b = DirectX::XMLoadFloat3(&b.position);
            DirectX::XMVECTOR _c = DirectX::XMLoadFloat3(&c.position);

            DirectX::XMVECTOR _ab = DirectX::XMVectorSubtract(_b, _a);
            DirectX::XMVECTOR _bc = DirectX::XMVectorSubtract(_c, _b);
            DirectX::XMVECTOR _ca = DirectX::XMVectorSubtract(_a, _c);

            // �@�������C�����Ɏˉe
            DirectX::XMVECTOR normal = DirectX::XMVector3Cross(_ab, _bc);
            DirectX::XMVECTOR vdot = DirectX::XMVector3Dot(normal, dir);
            float fdot;
            DirectX::XMStoreFloat(&fdot, vdot);
            if (fdot >= 0)   continue;

            // ���ˈʒu����O�p�`�̈�_��@���Ɏˉe
            DirectX::XMVECTOR v = DirectX::XMVectorSubtract(_a, start);
            // ���C�̒���
            DirectX::XMVECTOR vt = DirectX::XMVectorDivide(DirectX::XMVector3Dot(normal, v), vdot);
            float ft;
            DirectX::XMStoreFloat(&ft, vt);
            if (ft < 0 || ft > neart) continue;
            // ��_�Z�o
            DirectX::XMVECTOR vec_position = DirectX::XMVectorAdd(DirectX::XMVectorMultiply(dir, vt), start);

            // ���
            DirectX::XMVECTOR v1 = DirectX::XMVectorSubtract(_a, vec_position);
            DirectX::XMVECTOR cross1 = DirectX::XMVector3Cross(v1, _ab);
            DirectX::XMVECTOR vec_dot1 = DirectX::XMVector3Dot(cross1, normal);
            float dot1;
            DirectX::XMStoreFloat(&dot1, vec_dot1);
            if (dot1 < 0.0f)continue;

            // ���
            DirectX::XMVECTOR v2 = DirectX::XMVectorSubtract(_b, vec_position);
            DirectX::XMVECTOR cross2 = DirectX::XMVector3Cross(v2, _bc);
            DirectX::XMVECTOR vec_dot2 = DirectX::XMVector3Dot(cross2, normal);
            float dot2;
            DirectX::XMStoreFloat(&dot2, vec_dot2);
            if (dot2 < 0.0f)continue;

            // �O��
            DirectX::XMVECTOR v3 = DirectX::XMVectorSubtract(_c, vec_position);
            DirectX::XMVECTOR cross3 = DirectX::XMVector3Cross(v3, _ca);
            DirectX::XMVECTOR vec_dot3 = DirectX::XMVector3Dot(cross3, normal);
            float dot3;
            DirectX::XMStoreFloat(&dot3, vec_dot3);
            if (dot3 < 0.0f)continue;

            // �ŋߋ������X�V
            neart = ft;

            // ��_�Ɩ@�����X�V
            hit_position = vec_position;
            hit_normal = normal;
            material_index = subset.materialIndex;
        }
    }

    return material_index;
}

//-----------------------------------------
// x����p
//-----------------------------------------
int CollisionRayCast::IntersectRayVsModelX(const DirectX::XMVECTOR& start,
    const DirectX::XMVECTOR& dir, 
    float neart, 
    const ModelResource::Mesh& mesh, 
    DirectX::XMVECTOR& hit_position, 
    DirectX::XMVECTOR& hit_normal)
{
    const std::vector<ModelResource::Vertex>& vertices = mesh.vertices;
    const std::vector<UINT>& indices = mesh.indices;
    const ModelResource::Vertex* vertices_data = vertices.data();
    const UINT* indices_data = indices.data();

    int material_index = -1;
    for (const ModelResource::Subset& subset : mesh.subsets)
    {
        for (UINT i = 0; i < subset.indexCount; i += 3)
        {
            UINT index = subset.startIndex + i;
            const ModelResource::Vertex& a = vertices_data[indices_data[index + 0]];
            const ModelResource::Vertex& b = vertices_data[indices_data[index + 1]];
            const ModelResource::Vertex& c = vertices_data[indices_data[index + 2]];


            DirectX::XMVECTOR _a = DirectX::XMLoadFloat3(&a.position);
            DirectX::XMVECTOR _b = DirectX::XMLoadFloat3(&b.position);
            DirectX::XMVECTOR _c = DirectX::XMLoadFloat3(&c.position);

            DirectX::XMVECTOR _ab = DirectX::XMVectorSubtract(_b, _a);
            DirectX::XMVECTOR _bc = DirectX::XMVectorSubtract(_c, _b);
            DirectX::XMVECTOR _ca = DirectX::XMVectorSubtract(_a, _c);

            // �@�������C�����Ɏˉe
            DirectX::XMVECTOR normal = DirectX::XMVector3Cross(_ab, _bc);
            DirectX::XMVECTOR vdot = DirectX::XMVector3Dot(normal, dir);
            float fdot;
            DirectX::XMStoreFloat(&fdot, vdot);
            if (fdot >= 0)   continue;

            // ���ˈʒu����O�p�`�̈�_��@���Ɏˉe
            DirectX::XMVECTOR v = DirectX::XMVectorSubtract(_a, start);
            // ���C�̒���
            DirectX::XMVECTOR vt = DirectX::XMVectorDivide(DirectX::XMVector3Dot(normal, v), vdot);
            float ft;
            DirectX::XMStoreFloat(&ft, vt);
            if (ft < 0 || ft > neart) continue;
            // ��_�Z�o
            DirectX::XMVECTOR vposition = DirectX::XMVectorAdd(DirectX::XMVectorMultiply(dir, vt), start);
            // 2D�̊O�ς𗘗p�������_����
            DirectX::XMFLOAT3 fposition, ab, bc, ca;
            DirectX::XMStoreFloat3(&fposition, vposition);
            DirectX::XMStoreFloat3(&ab, _ab);
            if ((a.position.y - fposition.y) * ab.z - (a.position.z - fposition.z) * ab.y > 0)
                continue;

            DirectX::XMStoreFloat3(&bc, _bc);
            if ((b.position.y - fposition.y) * bc.z - (b.position.z - fposition.z) * bc.y > 0)
                continue;

            DirectX::XMStoreFloat3(&ca, _ca);
            if ((c.position.y - fposition.y) * ca.z - (c.position.z - fposition.z) * ca.y > 0)
                continue;

            neart = ft;
            material_index = subset.materialIndex;
            hit_position = vposition;
            hit_normal = normal;
        }
    }
    return material_index;
}

//-----------------------------------------
// y����p
//-----------------------------------------
int CollisionRayCast::IntersectRayVsModelY(const DirectX::XMVECTOR& start,
    const DirectX::XMVECTOR& dir, 
    float neart, 
    const ModelResource::Mesh& mesh,
    DirectX::XMVECTOR& hit_position, 
    DirectX::XMVECTOR& hit_normal)
{
    const std::vector<ModelResource::Vertex>& vertices = mesh.vertices;
    const std::vector<UINT>& indices = mesh.indices;
    const ModelResource::Vertex* vertices_data = vertices.data();
    const UINT* indices_data = indices.data();

    int material_index = -1;
    for (const ModelResource::Subset& subset : mesh.subsets)
    {
        for (UINT i = 0; i < subset.indexCount; i += 3)
        {

            UINT index = subset.startIndex + i;
            const ModelResource::Vertex& a = vertices_data[indices_data[index + 0]];
            const ModelResource::Vertex& b = vertices_data[indices_data[index + 1]];
            const ModelResource::Vertex& c = vertices_data[indices_data[index + 2]];


            DirectX::XMVECTOR _a = DirectX::XMLoadFloat3(&a.position);
            DirectX::XMVECTOR _b = DirectX::XMLoadFloat3(&b.position);
            DirectX::XMVECTOR _c = DirectX::XMLoadFloat3(&c.position);

            DirectX::XMVECTOR _ab = DirectX::XMVectorSubtract(_b, _a);
            DirectX::XMVECTOR _bc = DirectX::XMVectorSubtract(_c, _b);
            DirectX::XMVECTOR _ca = DirectX::XMVectorSubtract(_a, _c);

            // �@�������C�����Ɏˉe
            DirectX::XMVECTOR normal = DirectX::XMVector3Cross(_ab, _bc);
            DirectX::XMVECTOR vdot = DirectX::XMVector3Dot(normal, dir);
            float fdot;
            DirectX::XMStoreFloat(&fdot, vdot);
            if (fdot >= 0)   continue;

            // ���ˈʒu����O�p�`�̈�_��@���Ɏˉe
            DirectX::XMVECTOR v = DirectX::XMVectorSubtract(_a, start);
            // ���C�̒���
            DirectX::XMVECTOR vt = DirectX::XMVectorDivide(DirectX::XMVector3Dot(normal, v), vdot);
            float ft;
            DirectX::XMStoreFloat(&ft, vt);
            if (ft < 0 || ft > neart) continue;
            // ��_�Z�o
            DirectX::XMVECTOR vposition = DirectX::XMVectorAdd(DirectX::XMVectorMultiply(dir, vt), start);
            // 2D�̊O�ς𗘗p�������_����
            DirectX::XMFLOAT3 fposition, ab, bc, ca;
            DirectX::XMStoreFloat3(&fposition, vposition);
            DirectX::XMStoreFloat3(&ab, _ab);
            if ((a.position.x - fposition.x) * ab.z - (a.position.z - fposition.z) * ab.x > 0)
                continue;

            DirectX::XMStoreFloat3(&bc, _bc);
            if ((b.position.x - fposition.x) * bc.z - (b.position.z - fposition.z) * bc.x > 0)
                continue;

            DirectX::XMStoreFloat3(&ca, _ca);
            if ((c.position.x - fposition.x) * ca.z - (c.position.z - fposition.z) * ca.x > 0)
                continue;

            neart = ft;
            material_index = subset.materialIndex;
            hit_position = vposition;
            hit_normal = normal;
        }
    }
    return material_index;
}

//-----------------------------------------
// z����p
//-----------------------------------------
int CollisionRayCast::IntersectRayVsModelZ(const DirectX::XMVECTOR& start,
    const DirectX::XMVECTOR& dir, 
    float neart, 
    const ModelResource::Mesh& mesh,
    DirectX::XMVECTOR& hit_position,
    DirectX::XMVECTOR& hit_normal)
{
    const std::vector<ModelResource::Vertex>& vertices = mesh.vertices;
    const std::vector<UINT>& indices = mesh.indices;
    const ModelResource::Vertex* vertices_data = vertices.data();
    const UINT* indices_data = indices.data();

    int material_index = -1;
    for (const ModelResource::Subset& subset : mesh.subsets)
    {
        for (UINT i = 0; i < subset.indexCount; i += 3)
        {
            UINT index = subset.startIndex + i;
            const ModelResource::Vertex& a = vertices_data[indices_data[index + 0]];
            const ModelResource::Vertex& b = vertices_data[indices_data[index + 1]];
            const ModelResource::Vertex& c = vertices_data[indices_data[index + 2]];


            DirectX::XMVECTOR _a = DirectX::XMLoadFloat3(&a.position);
            DirectX::XMVECTOR _b = DirectX::XMLoadFloat3(&b.position);
            DirectX::XMVECTOR _c = DirectX::XMLoadFloat3(&c.position);

            DirectX::XMVECTOR _ab = DirectX::XMVectorSubtract(_b, _a);
            DirectX::XMVECTOR _bc = DirectX::XMVectorSubtract(_c, _b);
            DirectX::XMVECTOR _ca = DirectX::XMVectorSubtract(_a, _c);

            // �@�������C�����Ɏˉe
            DirectX::XMVECTOR normal = DirectX::XMVector3Cross(_ab, _bc);
            DirectX::XMVECTOR vdot = DirectX::XMVector3Dot(normal, dir);
            float fdot;
            DirectX::XMStoreFloat(&fdot, vdot);
            if (fdot >= 0)   continue;

            // ���ˈʒu����O�p�`�̈�_��@���Ɏˉe
            DirectX::XMVECTOR v = DirectX::XMVectorSubtract(_a, start);
            // ���C�̒���
            DirectX::XMVECTOR vt = DirectX::XMVectorDivide(DirectX::XMVector3Dot(normal, v), vdot);
            float ft;
            DirectX::XMStoreFloat(&ft, vt);
            if (ft < 0 || ft > neart) continue;
            // ��_�Z�o
            DirectX::XMVECTOR vposition = DirectX::XMVectorAdd(DirectX::XMVectorMultiply(dir, vt), start);
            // 2D�̊O�ς𗘗p�������_����
            DirectX::XMFLOAT3 fposition, ab, bc, ca;
            DirectX::XMStoreFloat3(&fposition, vposition);
            DirectX::XMStoreFloat3(&ab, _ab);
            if ((a.position.x - fposition.x) * ab.y - (a.position.y - fposition.y) * ab.x > 0)
                continue;

            DirectX::XMStoreFloat3(&bc, _bc);
            if ((b.position.x - fposition.x) * bc.y - (b.position.y - fposition.y) * bc.x > 0)
                continue;

            DirectX::XMStoreFloat3(&ca, _ca);
            if ((c.position.x - fposition.x) * ca.y - (c.position.y - fposition.y) * ca.x > 0)
                continue;

            neart = ft;
            material_index = subset.materialIndex;
            hit_position = vposition;
            hit_normal = normal;
        }
    }
    return material_index;
}

//-----------------------------------
// GUI�`��
//-----------------------------------
void CollisionObject::OnGUI()
{
    bool flag = GetCollisionFlag();
    float radius = GetRadius();
    ImGui::Checkbox("CollisionFlag", &flag);
    //ImGui::InputFloat3("Rotation", &rotation.x);
    ImGui::InputFloat("Angle", &radius);
    ImGui::InputFloat3("position", &position.x);
    //ImGui::InputFloat3("Scale", &scale.x);
}
