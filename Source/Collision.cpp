#include "Collision.h"
#include "Graphics.h"
#include "ActorManager.h"
#include "Charactor.h"
#include "Enemy.h"

#include "SceneManager.h"
#include "Model.h"
//#include "Scene.h"

#include "DebugRenderer.h"
#include "Mathf.h"

//-----------------------------------
// �`�揈��
//-----------------------------------
void CollisionBox::Render(DebugRenderer* renderer)
{
    renderer->DrawCube(GetPosition(), GetXMFloatRadius(), GetColor());
}

//-----------------------------------------
// �`�揈��
//-----------------------------------------
void CollisionSphere::Render(DebugRenderer* renderer)
{
    renderer->DrawSphere(GetPosition(), GetRadius(), GetColor());
}

//-----------------------------------------
// �`�揈��
//-----------------------------------------
void CollisionCylinder::Render(DebugRenderer* renderer)
{
    renderer->DrawCylinder(GetPosition(), GetRadius(), GetHeight(), GetColor());
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