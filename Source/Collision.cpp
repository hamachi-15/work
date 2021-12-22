#include "Collision.h"
#include "Graphics.h"
#include "ActorManager.h"
#include "Charactor.h"
#include "SceneManager.h"

//-----------------------------------------
// 更新処理
//-----------------------------------------
void CollisionSphere::Update(float elapsed_time)
{
    std::shared_ptr<Actor> actor = GetActor();

    Model* model = actor->GetModel();
    // コリジョンフラグが立っていなけれは更新しない
    if (GetCollisionFlag() == false) return;

    switch (GetPositionMask())
    {
        // クラスメンバの座標の更新
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
        // ローカル座標からの座標の更新
    case CollisionPositionMask::Collision_Mask_Local_Member_Position:
        {
        // TODO リファクタリング
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
// 描画処理
//-----------------------------------------
void CollisionSphere::Draw()
{
    DebugRenderer* renderer = Graphics::Instance().GetDebugRenderer();
    // 座標更新マスクがアクター座標を参照になっているならアクター座標で更新
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
// 更新処理
//-----------------------------------------
void CollisionCylinder::Update(float elapsed_time)
{
    Model* model = GetActor()->GetModel();

    // コリジョンフラグが立っていなけれは更新しない
    if (GetCollisionFlag() == false) return;

    switch (GetPositionMask())
    {
        // クラスメンバの座標の更新
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
    // ローカル座標からの座標の更新
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
// 描画処理
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
// 更新処理
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
    // 球vs球の交差判定
    {   
        size_t sphere_count = spheres.size();
        for (size_t i = 0; i < sphere_count; i++)
        {
            CollisionSphere* sphereA = spheres.at(i).get();
            // 当たり判定フラグが立っていなかったら飛ばす
            if (!sphereA->GetCollisionFlag())
            {
                continue;
            }
            for (size_t j = i + 1; j < sphere_count; j++)
            {
                CollisionSphere* sphereB = spheres.at(j).get();
                // 当たり判定フラグが立っていなかったら飛ばす
                if (!sphereB->GetCollisionFlag())
                {
                    continue;
                }

                IntersectSphereVsSphere(sphereA, sphereB);

            }
        }
    }
    // 球vs円柱の交差判定
    // TODO リザルトを考慮した当たり判定に変更
    {
        size_t sphere_count = spheres.size();
        size_t cylinder_count = cylinderes.size();
        for (size_t i = 0; i < sphere_count; i++)
        {
            CollisionSphere* sphere = spheres.at(i).get();
            // 当たり判定フラグが立っていなかったら飛ばす
            if (!sphere->GetCollisionFlag())
            {
                continue;
            }
            // 球コリジョンのキャラクタコンポーネント取得
            std::shared_ptr<Charactor> sphere_charactor_component = sphere->GetActor()->GetComponent<Charactor>();

            for (size_t j = 0; j < cylinder_count; j++)
            {
                CollisionCylinder* cylinder = cylinderes.at(j).get();
                // 当たり判定フラグが立っていなかったら飛ばす
                if (!cylinder->GetCollisionFlag() ||
                    sphere_charactor_component->GetID() == cylinder->GetActor()->GetComponent<Charactor>()->GetID())
                {
                    continue;
                }                
                if (IntersectSphereVsCylinder(sphere, cylinder))
                {
                    // コリジョンの属性が武器ならノックバック
                    if (sphere->GetCollisionElement() == CollisionElement::Weppon)
                    {
                        // 武器と衝突したコリジョンの持ち主に衝突したことを送信する
                        Message message;
                        message.message = MessageType::Message_GetHit_Attack;
                        message.hit_position = sphere->GetPosition();
                        MetaAI::Instance().SendMessaging(
                            static_cast<int>(MetaAI::Identity::Collision), // 送信元
                            static_cast<int>(cylinder->GetActorID()),      // 受信先
                            message);                                      // メッセージ
                        // 攻撃か当たったことを武器のコリジョンの持ち主に送信する
                        message.message = MessageType::Message_Hit_Attack;
                        message.hit_position = {0.0f, 0.0f, 0.0f};
                        MetaAI::Instance().SendMessaging(
                            static_cast<int>(MetaAI::Identity::Collision),   // 送信元
                            static_cast<int>(sphere->GetActorID()),          // 受信先
                            message);                                        // メッセージ
                    }
                }
            }
        }
    }
    // 円柱vs円柱の交差判定
   // ObjectCollisionResult result;
    {
        size_t cylinder_count = cylinderes.size();
        for (size_t i = 0; i < cylinder_count; i++)
        {
            CollisionCylinder* cylinderA = cylinderes.at(i).get();
            // 当たり判定フラグが立っていなかったら飛ばす
            if (!cylinderA->GetCollisionFlag())
            {
                continue;
            }

            for (size_t j = i + 1; j < cylinder_count; j++)
            {
                CollisionCylinder* cylinderB = cylinderes.at(j).get();
                // 当たり判定フラグが立っていなかったら飛ばす
                if (!cylinderB->GetCollisionFlag())
                {
                    continue;
                }

                if (IntersectCylinderVsCylinder(cylinderA, cylinderB, result))
                {
                    PushOutCollision(cylinderA, cylinderB, result);
                    // 現在のシーンがワールドマップならシーンへ敵とエンカウントをしたメッセージを送る
                    const char* scene_name = SceneManager::Instance().GetCurrentScene()->GetName();
                    int isplayercolA = strcmp(cylinderA->GetName(), "Player");
                    int isplayercolB = strcmp(cylinderB->GetName(), "Player");
                    int isworldmap = strcmp(scene_name, "SceneWorldMap");
                    if (isplayercolA == 0 && isworldmap == 0 ||
                        isplayercolB == 0 && isworldmap == 0)
                    {
                        Message message;
                        message.message = MessageType::Message_Hit_Boddy;
                        // 衝突した敵の座標を設定
                        if (isplayercolA == 0)
                        {
                            message.hit_position = cylinderA->GetActor()->GetPosition();
                        }
                        else
                        {
                            message.hit_position = cylinderB->GetActor()->GetPosition();
                        }
                        MetaAI::Instance().SendMessaging(
                            static_cast<int>(MetaAI::Identity::Collision),   // 送信元
                            static_cast<int>(MetaAI::Identity::WorldMap),    // 受信先
                            message);                                        // メッセージ
                        return;
                    }
                }
            }
        }
    }
}

//-----------------------------------------
// 描画処理
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
// メッセージを受信したときの処理
//-----------------------------------------
bool CollisionManager::OnMessage(const Telegram& message)
{
    return false;
}

//-----------------------------------------
// コリジョンリストを全破棄
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
//　球コリジョンの登録
//-----------------------------------------
void CollisionManager::ReregisterSphere(std::shared_ptr<CollisionSphere> collision)
{
    spheres.emplace_back(collision); 
}

//-----------------------------------------
// 球コリジョンの解除
//-----------------------------------------
void CollisionManager::UnregisterSphere(std::shared_ptr<CollisionSphere> collision)
{
    // 破棄用球配列にコリジョンを追加
    remove_spheres.emplace_back(collision);
}

//-----------------------------------------
//　円柱コリジョンの登録
//-----------------------------------------
void CollisionManager::ReregisterCylinder(std::shared_ptr<CollisionCylinder> collision)
{
    cylinderes.emplace_back(collision);
}

//-----------------------------------------
// 円柱コリジョンの解除
//-----------------------------------------
void CollisionManager::UnregisterCylinder(std::shared_ptr<CollisionCylinder> collision)
{
    // 破棄用円柱配列にコリジョンを追加
    remove_cylinderes.emplace_back(collision);
}

//-----------------------------------------
// 押し出し処理
//-----------------------------------------
void CollisionManager::PushOutCollision(CollisionObject* collisionA, CollisionObject* collisionB, ObjectCollisionResult& result)
{
    DirectX::XMVECTOR vec_positionA = DirectX::XMLoadFloat3(&result.positionA);
    DirectX::XMVECTOR vec_positionB = DirectX::XMLoadFloat3(&result.positionB);

    // 2つの球の重さから押し出し量を求める
    float rateA = collisionA->GetWeight() / (collisionA->GetWeight() + collisionB->GetWeight());
    float rateB = 1.0f - rateA;

    DirectX::XMVECTOR velocityB = DirectX::XMVectorScale(result.vector, rateA);
    vec_positionB = DirectX::XMVectorSubtract(vec_positionB, velocityB);

    // 押し出す
    DirectX::XMVECTOR velocityA = DirectX::XMVectorScale(result.vector, rateB);
    vec_positionA = DirectX::XMVectorAdd(vec_positionA, velocityA);

    // 衝突結果
    DirectX::XMStoreFloat3(&result.positionA, vec_positionA);
    DirectX::XMStoreFloat3(&result.positionB, vec_positionB);

    // 座標更新
    collisionA->GetActor()->SetPosition(result.positionA);
    collisionB->GetActor()->SetPosition(result.positionB);
}

//-----------------------------------------
// 球と球の交差判定
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

    // 距離判定
    float renge = sphereA->GetRadius() + sphereB->GetRadius();
    if (renge < length)
    {
        return false;
    }

    // 単位ベクトル化
    vector = DirectX::XMVector3Normalize(vector);

    // めりこみ量を求める
    float diff = renge - length;
    vector = DirectX::XMVectorScale(vector, diff);

  //  PushOutCollision(sphereA, positionA, vec_positionA, sphereB, positionB, vec_positionB, vector, diff);

    return true;
}

//-----------------------------------------
// 球と円柱の交差判定
//-----------------------------------------
bool CollisionManager::IntersectSphereVsCylinder(CollisionSphere* sphere, CollisionCylinder* cylinder)
{
    // 座標取得
    DirectX::XMFLOAT3 sphere_position = sphere->GetPosition();
    DirectX::XMFLOAT3 cylinder_position = cylinder->GetActor()->GetPosition();
    float             sphere_radius = sphere->GetRadius();
    float             cylinder_radius = cylinder->GetRadius();
    float             cylinder_height = cylinder->GetHeight();
    //Aの足元がBの頭より上なら当たっていない
    if (sphere_position.y - sphere_radius > cylinder_position.y + cylinder_height)
    {
        return false;
    }
    //Aの頭がBの足元より下なら当たっていない
    if (sphere_position.y + sphere_radius < cylinder_position.y)
    {
        return false;
    }

    // XZ平面での範囲チェック
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

    // 単位ベクトル化
    vector = DirectX::XMVector3Normalize(vector);

    // めりこみ量を求める
    float diff = renge - length;
    vector = DirectX::XMVectorScale(vector, diff);

    //PushOutCollision(sphere, cylinder, result);

    return true;
}

//-----------------------------------------
// 円柱と円柱の交差判定
//-----------------------------------------
bool CollisionManager::IntersectCylinderVsCylinder(CollisionCylinder* cylinderA, CollisionCylinder* cylinderB, ObjectCollisionResult& result)
{
    // 座標取得
    result.positionA = cylinderA->GetActor()->GetPosition();
    result.positionB = cylinderB->GetActor()->GetPosition();
    //Aの足元がBの頭より上なら当たっていない
    if (result.positionA.y > result.positionB.y + cylinderB->GetHeight())
    {
        return false;
    }
    //Aの頭がBの足元より下なら当たっていない
    if (result.positionA.y + cylinderA->GetHeight() < result.positionB.y)
    {
        return false;
    }

    // XZ平面での範囲チェック
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

    // 単位ベクトル化
    result.vector = DirectX::XMVector3Normalize(result.vector);

    // めり込み量を求める
    float diff = renge - length;
    result.vector = DirectX::XMVectorScale(result.vector, diff);

    //PushOutCollision(cylinderA, cylinderB, result);

    return true;
}

//-----------------------------------------
// レイとモデルの交差判定
//-----------------------------------------
bool CollisionManager::IntersectRayVsModel(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, Model* model, HitResult& result)
{
    return raycast.IntersectRayVsModel(start, end, model, result);
}

//-----------------------------------------
// 名前から球コリジョン取得
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
// IDから球コリジョンを取得
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
// 名前から円柱コリジョン取得
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

//-----------------------------------
// GUI描画
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
