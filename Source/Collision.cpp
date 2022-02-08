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
// 描画処理
//-----------------------------------
void CollisionBox::Render(DebugRenderer* renderer)
{
    renderer->DrawCube(GetPosition(), GetXMFloatRadius(), GetColor());
}

//-----------------------------------------
// 描画処理
//-----------------------------------------
void CollisionSphere::Render(DebugRenderer* renderer)
{
    renderer->DrawSphere(GetPosition(), GetRadius(), GetColor());
}

//-----------------------------------------
// 描画処理
//-----------------------------------------
void CollisionCylinder::Render(DebugRenderer* renderer)
{
    renderer->DrawCylinder(GetPosition(), GetRadius(), GetHeight(), GetColor());
}


//-----------------------------------------
// レイとモデルの交差判定
//-----------------------------------------
bool CollisionRayCast::IntersectRayVsModel(const DirectX::XMFLOAT3& start,
    const DirectX::XMFLOAT3& end, 
    const Model* model,
    HitResult& hit_result)
{
    // レイのベクトルとベクトルの長さの算出
    DirectX::XMVECTOR world_start = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR world_end = DirectX::XMLoadFloat3(&end);
    DirectX::XMVECTOR world_ray_vec = DirectX::XMVectorSubtract(world_end, world_start);
    DirectX::XMVECTOR world_ray_length = DirectX::XMVector3Length(world_ray_vec);

    // ワールド空間のレイの長さ
    DirectX::XMStoreFloat(&hit_result.distance, world_ray_length);

    bool hit = false;
    const ModelResource* resource = model->GetResource();
    for (const ModelResource::Mesh& mesh : resource->GetMeshes())
    {
        const Model::Node node = model->GetNodes().at(mesh.nodeIndex);

        // レイをワールド空間からローカル空間へ変換
        DirectX::XMMATRIX world_transform = DirectX::XMLoadFloat4x4(&node.world_transform);
        DirectX::XMMATRIX inverse_world_transform = DirectX::XMMatrixInverse(nullptr, world_transform);

        DirectX::XMVECTOR start = DirectX::XMVector3TransformCoord(world_start, inverse_world_transform);
        DirectX::XMVECTOR end = DirectX::XMVector3TransformCoord(world_end, inverse_world_transform);
        DirectX::XMVECTOR vec = DirectX::XMVectorSubtract(end, start);
        DirectX::XMVECTOR dir = DirectX::XMVector3Normalize(vec);
        DirectX::XMVECTOR length = DirectX::XMVector3Length(vec);

        // レイの長さ
        float neart;
        DirectX::XMStoreFloat(&neart, length);

        // 三角形(面)との交差判定
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
                // 三角形の頂点を抽出
                const ModelResource::Vertex& a = vertices.at(indices.at(index));
                const ModelResource::Vertex& b = vertices.at(indices.at(index + 1));
                const ModelResource::Vertex& c = vertices.at(indices.at(index + 2));

                DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&a.position);
                DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&b.position);
                DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&c.position);

                // 三角形の三辺ベクトルを算出
                DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
                DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
                DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(A, C);

                // 三角形の法線ベクトルを算出
                DirectX::XMVECTOR normal = DirectX::XMVector3Cross(AB, BC);

                // 内積の結果が正なら裏向き
                DirectX::XMVECTOR _dot = DirectX::XMVector3Dot(dir, normal);
                float dot;
                DirectX::XMStoreFloat(&dot, _dot);
                if (dot >= 0.0f) continue;

                // レイとの平面の交点を算出
                DirectX::XMVECTOR V = DirectX::XMVectorSubtract(A, start);
                DirectX::XMVECTOR T = DirectX::XMVectorDivide(DirectX::XMVector3Dot(normal, V), _dot);
                float t;
                DirectX::XMStoreFloat(&t, T);

                // 交点までの距離が今までに計算した最近距離より大きいときはスキップ
                if (t < 0.0f || t > neart) continue;

                DirectX::XMVECTOR position = position = DirectX::XMVectorAdd(DirectX::XMVectorMultiply(dir, T), start);

                // 交点が三角形の内側にあるか判定
                // 一つ目
                DirectX::XMVECTOR V1 = DirectX::XMVectorSubtract(A, position);
                DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(V1, AB);
                DirectX::XMVECTOR Dot1 = DirectX::XMVector3Dot(Cross1, normal);
                float dot1;
                DirectX::XMStoreFloat(&dot1, Dot1);
                if (dot1 < 0.0f)continue;

                // 二つ目
                DirectX::XMVECTOR V2 = DirectX::XMVectorSubtract(B, position);
                DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(V2, BC);
                DirectX::XMVECTOR Dot2 = DirectX::XMVector3Dot(Cross2, normal);
                float dot2;
                DirectX::XMStoreFloat(&dot2, Dot2);
                if (dot2 < 0.0f)continue;

                // 三つ目
                DirectX::XMVECTOR V3 = DirectX::XMVectorSubtract(C, position);
                DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(V3, CA);
                DirectX::XMVECTOR Dot3 = DirectX::XMVector3Dot(Cross3, normal);
                float dot3;
                DirectX::XMStoreFloat(&dot3, Dot3);
                if (dot3 < 0.0f)continue;

                // 最近距離を更新
                neart = t;

                // 交点と法線を更新
                hit_position = position;
                hit_normal = normal;
                material_index = subset.materialIndex;
            }
        }
        if (material_index >= 0)
        {
            // ローカル空間からワールド空間へ変換
            DirectX::XMVECTOR world_position = DirectX::XMVector3TransformCoord(hit_position, world_transform);
            DirectX::XMVECTOR world_cross_vec = DirectX::XMVectorSubtract(world_position, world_start);
            DirectX::XMVECTOR world_cross_length = DirectX::XMVector3Length(world_cross_vec);
            float distance;
            DirectX::XMStoreFloat(&distance, world_cross_length);
            // ヒット情報保存
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
// 通常用
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

            // 法線をレイ方向に射影
            DirectX::XMVECTOR normal = DirectX::XMVector3Cross(_ab, _bc);
            DirectX::XMVECTOR vdot = DirectX::XMVector3Dot(normal, dir);
            float fdot;
            DirectX::XMStoreFloat(&fdot, vdot);
            if (fdot >= 0)   continue;

            // 発射位置から三角形の一点を法線に射影
            DirectX::XMVECTOR v = DirectX::XMVectorSubtract(_a, start);
            // レイの長さ
            DirectX::XMVECTOR vt = DirectX::XMVectorDivide(DirectX::XMVector3Dot(normal, v), vdot);
            float ft;
            DirectX::XMStoreFloat(&ft, vt);
            if (ft < 0 || ft > neart) continue;
            // 交点算出
            DirectX::XMVECTOR vec_position = DirectX::XMVectorAdd(DirectX::XMVectorMultiply(dir, vt), start);

            // 一つ目
            DirectX::XMVECTOR v1 = DirectX::XMVectorSubtract(_a, vec_position);
            DirectX::XMVECTOR cross1 = DirectX::XMVector3Cross(v1, _ab);
            DirectX::XMVECTOR vec_dot1 = DirectX::XMVector3Dot(cross1, normal);
            float dot1;
            DirectX::XMStoreFloat(&dot1, vec_dot1);
            if (dot1 < 0.0f)continue;

            // 二つ目
            DirectX::XMVECTOR v2 = DirectX::XMVectorSubtract(_b, vec_position);
            DirectX::XMVECTOR cross2 = DirectX::XMVector3Cross(v2, _bc);
            DirectX::XMVECTOR vec_dot2 = DirectX::XMVector3Dot(cross2, normal);
            float dot2;
            DirectX::XMStoreFloat(&dot2, vec_dot2);
            if (dot2 < 0.0f)continue;

            // 三つ目
            DirectX::XMVECTOR v3 = DirectX::XMVectorSubtract(_c, vec_position);
            DirectX::XMVECTOR cross3 = DirectX::XMVector3Cross(v3, _ca);
            DirectX::XMVECTOR vec_dot3 = DirectX::XMVector3Dot(cross3, normal);
            float dot3;
            DirectX::XMStoreFloat(&dot3, vec_dot3);
            if (dot3 < 0.0f)continue;

            // 最近距離を更新
            neart = ft;

            // 交点と法線を更新
            hit_position = vec_position;
            hit_normal = normal;
            material_index = subset.materialIndex;
        }
    }

    return material_index;
}

//-----------------------------------------
// x軸専用
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

            // 法線をレイ方向に射影
            DirectX::XMVECTOR normal = DirectX::XMVector3Cross(_ab, _bc);
            DirectX::XMVECTOR vdot = DirectX::XMVector3Dot(normal, dir);
            float fdot;
            DirectX::XMStoreFloat(&fdot, vdot);
            if (fdot >= 0)   continue;

            // 発射位置から三角形の一点を法線に射影
            DirectX::XMVECTOR v = DirectX::XMVectorSubtract(_a, start);
            // レイの長さ
            DirectX::XMVECTOR vt = DirectX::XMVectorDivide(DirectX::XMVector3Dot(normal, v), vdot);
            float ft;
            DirectX::XMStoreFloat(&ft, vt);
            if (ft < 0 || ft > neart) continue;
            // 交点算出
            DirectX::XMVECTOR vposition = DirectX::XMVectorAdd(DirectX::XMVectorMultiply(dir, vt), start);
            // 2Dの外積を利用した内点判定
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
// y軸専用
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

            // 法線をレイ方向に射影
            DirectX::XMVECTOR normal = DirectX::XMVector3Cross(_ab, _bc);
            DirectX::XMVECTOR vdot = DirectX::XMVector3Dot(normal, dir);
            float fdot;
            DirectX::XMStoreFloat(&fdot, vdot);
            if (fdot >= 0)   continue;

            // 発射位置から三角形の一点を法線に射影
            DirectX::XMVECTOR v = DirectX::XMVectorSubtract(_a, start);
            // レイの長さ
            DirectX::XMVECTOR vt = DirectX::XMVectorDivide(DirectX::XMVector3Dot(normal, v), vdot);
            float ft;
            DirectX::XMStoreFloat(&ft, vt);
            if (ft < 0 || ft > neart) continue;
            // 交点算出
            DirectX::XMVECTOR vposition = DirectX::XMVectorAdd(DirectX::XMVectorMultiply(dir, vt), start);
            // 2Dの外積を利用した内点判定
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
// z軸専用
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

            // 法線をレイ方向に射影
            DirectX::XMVECTOR normal = DirectX::XMVector3Cross(_ab, _bc);
            DirectX::XMVECTOR vdot = DirectX::XMVector3Dot(normal, dir);
            float fdot;
            DirectX::XMStoreFloat(&fdot, vdot);
            if (fdot >= 0)   continue;

            // 発射位置から三角形の一点を法線に射影
            DirectX::XMVECTOR v = DirectX::XMVectorSubtract(_a, start);
            // レイの長さ
            DirectX::XMVECTOR vt = DirectX::XMVectorDivide(DirectX::XMVector3Dot(normal, v), vdot);
            float ft;
            DirectX::XMStoreFloat(&ft, vt);
            if (ft < 0 || ft > neart) continue;
            // 交点算出
            DirectX::XMVECTOR vposition = DirectX::XMVectorAdd(DirectX::XMVectorMultiply(dir, vt), start);
            // 2Dの外積を利用した内点判定
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