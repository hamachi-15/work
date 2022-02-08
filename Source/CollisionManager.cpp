#include "Graphics.h"

#include "Mathf.h"
#include "CollisionManager.h"
#include "SceneManager.h"

#include "Charactor.h"
#include "Enemy.h"

#include "CullingCollision.h"

#include "ActorManager.h"
//-----------------------------------------
// 更新処理
//-----------------------------------------
void CollisionManager::Update()
{
    // カリングコリジョン破棄処理
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

    // 球破棄処理
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

    // 円柱破棄処理
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

    // カリングコリジョン更新処理
    std::vector<Plane> frustum = SceneManager::Instance().GetCurrentScene()->GetCameraController()->GetFrustum();
    for (std::shared_ptr<CullingCollision> culling : cullings)
    {
        culling->Update();
    }
}

//-----------------------------------------
// 描画処理
//-----------------------------------------
void CollisionManager::Draw()
{
    DebugRenderer* renderer = Graphics::Instance().GetDebugRenderer();

    // カリングコリジョン描画
    for (std::shared_ptr<CullingCollision> culling : cullings)
    {
        culling->Render(renderer);
    }

    // 球コリジョン描画
    for (std::shared_ptr<CollisionSphere> sphere : spheres)
    {
        if (!sphere->GetAttackFlag()) continue;
        sphere->Render(renderer);
    }

    // 円柱コリジョン描画
    for (std::shared_ptr<CollisionCylinder> cylinder : cylinderes)
    {
        cylinder->Render(renderer);
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
    // カリングコリジョンリストの破棄
    std::vector<std::shared_ptr<CullingCollision>>::iterator iterate_box = cullings.begin();
    for (; iterate_box != cullings.end(); iterate_box = cullings.begin())
    {
        cullings.erase(iterate_box);
    }

    // カリングコリジョン破棄リストの破棄
    std::vector<std::shared_ptr<CullingCollision>>::iterator iterate_remove_box = remove_cullings.begin();
    for (; iterate_remove_box != remove_cullings.end(); iterate_remove_box = remove_cullings.begin())
    {
        remove_cullings.erase(iterate_remove_box);
    }

    // 球コリジョンリストの破棄
    std::vector<std::shared_ptr<CollisionSphere>>::iterator iterate_sphere = spheres.begin();
    for (; iterate_sphere != spheres.end(); iterate_sphere = spheres.begin())
    {
        spheres.erase(iterate_sphere);
    }

    // 球コリジョン破棄リストの破棄
    std::vector<std::shared_ptr<CollisionSphere>>::iterator iterate_remove_sphere = remove_spheres.begin();
    for (; iterate_remove_sphere != remove_spheres.end(); iterate_remove_sphere = remove_spheres.begin())
    {
        remove_spheres.erase(iterate_remove_sphere);
    }

    // 円柱コリジョンリスト破棄
    std::vector<std::shared_ptr<CollisionCylinder>>::iterator iterate_cylinder = cylinderes.begin();
    for (; iterate_cylinder != cylinderes.end(); iterate_cylinder = cylinderes.begin())
    {
        cylinderes.erase(iterate_cylinder);
    }

    // 円柱コリジョン破棄リスト破棄
    std::vector<std::shared_ptr<CollisionCylinder>>::iterator iterate_remove_cylinder = remove_cylinderes.begin();
    for (; iterate_remove_cylinder != remove_cylinderes.end(); iterate_remove_cylinder = remove_cylinderes.begin())
    {
        remove_cylinderes.erase(iterate_remove_cylinder);
    }
}

//-----------------------------------------
// カリングコリジョン登録
//-----------------------------------------
void CollisionManager::RegisterCulling(std::shared_ptr<CullingCollision> collision)
{
    // カリングコリジョン配列にコリジョンを追加
    cullings.emplace_back(collision);
}

//-----------------------------------------
// カリングコリジョン解除
//-----------------------------------------
void CollisionManager::UnregisterCulling(std::shared_ptr<CullingCollision> collision)
{
    // 破棄用カリングコリジョン配列にコリジョンを追加
    remove_cullings.emplace_back(collision);
}

//-----------------------------------------
//　球コリジョンの登録
//-----------------------------------------
void CollisionManager::RegisterSphere(std::shared_ptr<CollisionSphere> collision)
{
    // 球配列にコリジョンを追加
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
void CollisionManager::RegisterCylinder(std::shared_ptr<CollisionCylinder> collision)
{
    // 円柱配列にコリジョンを追加
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
void CollisionManager::PushOutCollision(CollisionObject* collisionA, std::shared_ptr<Actor> collisionA_actor, CollisionObject* collisionB, std::shared_ptr<Actor> collisionB_actor, ObjectCollisionResult& result)
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
    collisionA_actor->SetPosition(result.positionA);
    collisionB_actor->SetPosition(result.positionB);
}

//-----------------------------------------
// カメラの錐台とAABBの内外判定
//-----------------------------------------
bool CollisionManager::IntersectFrustumVsAABB(std::shared_ptr<CollisionBox> aabb, std::vector<Plane> frustum_list)
{
    // 錐台の平面の数分ループ
    int index = 0;
    for (; index < Frustum_Plan_Max; ++index)
    {
        //各平面の法線の成分を用いてAABBの８頂点の中から最近点と最遠点を求める
        DirectX::XMFLOAT3 NegaPos = aabb->GetPosition();	// 最近点
        DirectX::XMFLOAT3 PosiPos = aabb->GetPosition();	// 最遠点
        // 半径取得
        DirectX::XMFLOAT3 radius = aabb->GetXMFloatRadius();

        // 錐台の平面取得
        Plane frustum = frustum_list[index];

        // 最近点算出
        Mathf::NegaCalculate(NegaPos, frustum.normal, radius);

        // 最遠点算出
        Mathf::PosiCalculate(PosiPos, frustum.normal, radius);

        //  各平面との内積を計算し、交差・内外判定(表裏判定)を行う 
        //  外部と分かれば処理をbreakし確定させる
        //  交差状態であれば、ステータスを変更してから次の平面とのチェックに続ける
        //  内部であれば、そのまま次の平面とのチェックに続ける
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
    // ボックスが視錐台の外にあればカリングを行う
    return (index != Frustum_Plan_Max);
}

//-----------------------------------------
// 球と球の交差判定
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

    return true;
}

//-----------------------------------------
// 球と円柱の交差判定
//-----------------------------------------
bool CollisionManager::IntersectSphereVsCylinder(CollisionSphere* sphere, CollisionCylinder* cylinder)
{
    // 座標取得
    DirectX::XMFLOAT3 sphere_position = sphere->GetPosition();
    DirectX::XMFLOAT3 cylinder_position = cylinder->GetPosition();
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

    return true;
}

//-----------------------------------------
// 円柱と円柱の交差判定
//-----------------------------------------
bool CollisionManager::IntersectCylinderVsCylinder(CollisionCylinder* cylinderA, std::shared_ptr<Actor> collisionA_actor, CollisionCylinder* cylinderB, std::shared_ptr<Actor> collisionB_actor, ObjectCollisionResult& result)
{
    // 座標取得
    result.positionA = collisionA_actor->GetPosition();
    result.positionB = collisionB_actor->GetPosition();
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
// 名前からカリングコリジョン取得
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
// 名前から球コリジョン取得
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
// IDと名前から球コリジョン取得
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
// IDと名前から円柱コリジョンを取得
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
