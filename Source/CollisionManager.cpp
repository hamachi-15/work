#include "CollisionManager.h"

#include "Mathf.h"
#include "SceneManager.h"

#include "Charactor.h"
#include "Enemy.h"

//-----------------------------------------
// 更新処理
//-----------------------------------------
void CollisionManager::Update()
{
    // ボックス破棄処理
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

    ObjectCollisionResult result;

    // コリジョンがカメラの錐台内にいるかの判定
    {
        Scene* scene = SceneManager::Instance().GetCurrentScene();
        size_t box_count = boxes.size();
        for (std::shared_ptr<CollisionBox> aabb : boxes)
        {
            // カリングを行うかのフラグ
            bool culling_flag = true;
            int index = 0;
            for (; index < 6; ++index)
            {
                //各平面の法線の成分を用いてAABBの８頂点の中から最近点と最遠点を求める
                DirectX::XMFLOAT3 NegaPos = aabb->GetActor()->GetPosition();	// 最近点
                DirectX::XMFLOAT3 PosiPos = aabb->GetActor()->GetPosition();	// 最遠点
                // 半径取得
                DirectX::XMFLOAT3 radius = aabb->GetRadius();

                // 錐台の平面取得
                Plane frustum = scene->camera_controller->frustum[index];

                // 最近点算出
                Mathf::NegaCalculate(NegaPos, frustum.normal, radius);

                // 最遠点算出
                Mathf::PosiCalculate(PosiPos, frustum.normal, radius);

                //  各平面との内積を計算し、交差・内外判定(表裏判定)を行う 
                //  外部と分かれば処理をbreakし確定させる
                //  交差状態であれば、ステータスを変更してから次の平面とのチェックに続ける
                //  内部であれば、そのまま次の平面とのチェックに続ける
                float PosiLength, NegaLength;
                DirectX::XMStoreFloat(&PosiLength, DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&frustum.normal), DirectX::XMLoadFloat3(&PosiPos)));
                DirectX::XMStoreFloat(&NegaLength, DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&frustum.normal), DirectX::XMLoadFloat3(&NegaPos)));
                if (PosiLength < frustum.direction && NegaLength < frustum.direction)
                {
                    break;
                }
            }
            // ボックスが錐台外にあればカリングを行う
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
                        message.hit_position = { 0.0f, 0.0f, 0.0f };
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
                    int isplayercolA = strcmp(cylinderA->GetName().c_str(), "Player");
                    int isplayercolB = strcmp(cylinderB->GetName().c_str(), "Player");
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
                            message.territory_tag = cylinderB->GetActor()->GetComponent<Enemy>()->GetBelongingToTerritory();
                        }
                        else
                        {
                            message.hit_position = cylinderB->GetActor()->GetPosition();
                            message.territory_tag = cylinderA->GetActor()->GetComponent<Enemy>()->GetBelongingToTerritory();
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
    // 立方体コリジョンリストの破棄
    std::vector<std::shared_ptr<CollisionBox>>::iterator iterate_box = boxes.begin();
    for (; iterate_box != boxes.end(); iterate_box = boxes.begin())
    {
        boxes.erase(iterate_box);
    }

    // 球コリジョンリストの破棄
    std::vector<std::shared_ptr<CollisionSphere>>::iterator iterate_sphere = spheres.begin();
    for (; iterate_sphere != spheres.end(); iterate_sphere = spheres.begin())
    {
        spheres.erase(iterate_sphere);
    }

    // 円柱コリジョンリスト破棄
    std::vector<std::shared_ptr<CollisionCylinder>>::iterator iterate_cylinder = cylinderes.begin();
    for (; iterate_cylinder != cylinderes.end(); iterate_cylinder = cylinderes.begin())
    {
        cylinderes.erase(iterate_cylinder);
    }
}

//-----------------------------------------
// AABBコリジョン登録
//-----------------------------------------
void CollisionManager::RegisterBox(std::shared_ptr<CollisionBox> collision)
{
    // AABB配列にコリジョンを追加
    boxes.emplace_back(collision);
}

//-----------------------------------------
// AABBコリジョン解除
//-----------------------------------------
void CollisionManager::UnregisterBox(std::shared_ptr<CollisionBox> collision)
{
    // 破棄用AABB配列にコリジョンを追加
    remove_boxes.emplace_back(collision);
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
// 名前から立方体コリジョン取得
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
