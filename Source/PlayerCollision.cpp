#include "PlayerCollision.h"
#include "Telegram.h"
#include "MetaAI.h"
#include "Model.h"
#include "Universal.h"
#include "DebugRenderer.h"
#include "Graphics.h"

#include "ActorManager.h"
#include "CollisionManager.h"

#include "SceneManager.h"

#include "Charactor.h"
#include "Enemy.h"

#include "GameDatabase.h"

//************************************
// 
// プレイヤーのコリジョン
// 
//************************************
//-----------------------------------
// デストラクタ
//-----------------------------------
PlayerCollision::~PlayerCollision()
{

    // マネージャーのコリジョン削除
    // 球コリジョン削除
    for (std::shared_ptr<CollisionSphere> sphere : collision_spheres)
    {  
    	CollisionManager::Instance().UnregisterSphere(sphere);
    }

    // 円柱コリジョン削除
    CollisionManager::Instance().UnregisterCylinder(collision_cylinder);

    // 球コリジョン配列の解放
    std::vector<std::shared_ptr<CollisionSphere>>::iterator iterate_sphere = collision_spheres.begin();
    for (; iterate_sphere != collision_spheres.end(); iterate_sphere = collision_spheres.begin())
    {
        collision_spheres.erase(iterate_sphere);
    }
    collision_spheres.clear();
}

//-----------------------------------
// GUI描画
//-----------------------------------
void PlayerCollision::OnGUI()
{

}

//-----------------------------------
// 開始処理
//-----------------------------------
void PlayerCollision::Start()
{
    // アクターの取得
    std::shared_ptr<Actor> actor = GetActor();

    // キャラクターの取得
    std::shared_ptr<Charactor> charactor = actor->GetComponent<Charactor>();

    // モデル取得
    Model* model = actor->GetModel();

    std::vector<std::shared_ptr<CollisionParameterData>> collision_parameter = GameDataBase::Instance().GetAttackCollitionParamterDataList(EnemyCategory::None);
    for (std::shared_ptr<CollisionParameterData> data : collision_parameter)
    {
        CollisionParameter parameter;
        std::string collision_name = actor->GetName();
        parameter.name = collision_name + data->collision_name;
        parameter.actor_name = actor->GetName();
        parameter.node_name = data->node_name;
        parameter.actor_id = charactor->GetID();
        parameter.radius = data->radius;
        parameter.weight = data->weight;
        parameter.height = data->height;
        parameter.local_position.x = data->local_x;
        parameter.local_position.y = data->local_y;
        parameter.local_position.z = data->local_z;
        parameter.collision_flg = data->collision_flag;
        parameter.actor_type = data->actor_type;
        parameter.update_type = data->collision_update_type;
        if (data->collision_type == CollisionMeshType::Cylinder)
        {
            collision_cylinder = std::make_shared<CollisionCylinder>(parameter);
            // 円柱のコリジョンをマネージャーに設定
            CollisionManager::Instance().RegisterCylinder(collision_cylinder);
        }
        else if (data->collision_type == CollisionMeshType::Sphere)
        {
           collision_spheres.emplace_back(std::make_shared<CollisionSphere>(parameter));
            // 球のコリジョンをマネージャーに設定
           CollisionManager::Instance().RegisterSphere(collision_spheres.back());
        }
    }
}

//-----------------------------------
// 更新処理
//-----------------------------------
void PlayerCollision::Update(float elapsed_time)
{
    // アクター取得
    std::shared_ptr<Actor> actor = GetActor();

    // モデル取得
    Model* model = actor->GetModel();
    // 指定されたノードのローカル座標に更新
    for (std::shared_ptr<CollisionSphere> sphere : collision_spheres)
    {
        // コリジョンの座標更新のタイプに合わせて更新
        UpdateCollision(sphere, actor, model);
    }

    // コリジョンの座標更新のタイプに合わせて更新
    UpdateCollision(collision_cylinder, actor, model);

    std::shared_ptr<Charactor> charactor = GetActor()->GetComponent<Charactor>();
    // 球VS円柱
    size_t sphere_count = collision_spheres.size();
    size_t cylinder_count = CollisionManager::Instance().GetCollisionCylinderCount();
    for (size_t i = 0; i < sphere_count; i++)
    {
        CollisionSphere* sphere = collision_spheres.at(i).get();
        // 攻撃フラグが立っていなかったら飛ばす
        if (!sphere->GetAttackFlag()) continue;

        // 攻撃が当たっていた場合飛ばす
        if (charactor->GetHitAttackFlag()) break;

        for (size_t j = 0; j < cylinder_count; j++)
        {
            std::shared_ptr<CollisionCylinder> cylinder = CollisionManager::Instance().GetCollisionCylinder(j);
            // アクターがプレイヤーなら飛ばす
            if (sphere->GetActorType() == cylinder->GetActorType()) continue;

            // 当たり判定フラグが立っていなかったら飛ばす
            if (!cylinder->GetCollisionFlag()) continue;

            if (CollisionManager::Instance().IntersectSphereVsCylinder(sphere, cylinder.get()))
            {
                std::shared_ptr<Actor> cylinder_actor = ActorManager::Instance().GetActor(cylinder->GetActorName());
                Message message;
                // 攻撃が当たったことのメッセージ
                //message.message = MessageType::Message_GetHit_Attack;
                //message.hit_position = sphere->GetPosition();
                //Reaction(cylinder->GetActorID(), message);
                cylinder_actor->GetComponent<Charactor>()->ApplyDamage(charactor->GetAttack(), 0.0f);
                // 攻撃を当てたことのメッセージ
                message.message = MessageType::Message_Hit_Attack;
                message.hit_position = { 0.0f, 0.0f, 0.0f };
                Reaction(sphere->GetActorID(), message);
            }
        }
    }

    // 当たり判定フラグが立っていなかったら飛ばす
    if (!collision_cylinder->GetCollisionFlag()) return;

    // 円柱VS円柱
    ObjectCollisionResult result;
    for (size_t j = 0; j < cylinder_count; j++)
    {
        std::shared_ptr<CollisionCylinder> cylinderB = CollisionManager::Instance().GetCollisionCylinder(j);
        // 当たり判定フラグが立っていなかったら飛ばす
        if (!cylinderB->GetCollisionFlag()) continue;

        // アクターがプレイヤーなら飛ばす
        if (collision_cylinder->GetActorType() == cylinderB->GetActorType()) continue;

        std::shared_ptr<Actor> cylinderB_actor = ActorManager::Instance().GetActor(cylinderB->GetActorName());
        if (CollisionManager::Instance().IntersectCylinderVsCylinder(collision_cylinder.get(), GetActor(), cylinderB.get(), cylinderB_actor, result))
        {
            CollisionManager::Instance().PushOutCollision(collision_cylinder.get(), GetActor(), cylinderB.get(), cylinderB_actor, result);
            Message message;
            // 現在のシーンがワールドマップならシーンへ敵とエンカウントをしたメッセージを送る
            std::string scene_name = SceneManager::Instance().GetCurrentScene()->GetName();

            // 衝突した敵の座標を設定
            message.hit_position = collision_cylinder->GetPosition();
            message.territory_tag = ActorManager::Instance().GetActor(cylinderB->GetActorName())->GetComponent<Enemy>()->GetBelongingToTerritory();
            if (scene_name == "SceneWorldMap")
            {
                message.message = MessageType::Message_Hit_Boddy;
                Reaction(static_cast<int>(MetaAI::Identity::WorldMap), message);
                return;
            }
            else if (cylinderB->GetAttackFlag())
            {
                message.message = MessageType::Message_GetHit_Attack;
                Reaction(static_cast<int>(MetaAI::Identity::Player), message);
            }
        }
    }
}

//-------------------------------------------
// コリジョンの座標更新のタイプに合わせて更新
//-------------------------------------------
void PlayerCollision::UpdateCollision(std::shared_ptr<CollisionObject> collision,
    std::shared_ptr<Actor> actor, Model* model)
{
    // コリジョンの座標更新のタイプに合わせて更新
    switch (collision->GetUpdateType())
    {
    case CollisionUpdateType::Update_Actor:
        // アクター座標に更新
        Universal::ActorPositionUpdate(collision.get(), actor);
        break;
    case CollisionUpdateType::Update_Node:
        // 特定ノード座標に更新
        Universal::NodePositionUpdate(collision.get(), collision->GetNodeName(), model);
        break;
    case CollisionUpdateType::Update_Local:
        // 特定ノード座標のローカル位置を算出し更新
        Universal::LocalPositionUpdate(collision.get(), model->FindNode(collision->GetNodeName()));
        break;
    case CollisionUpdateType::Update_Castam:
        // その他の更新方法
        Universal::CastamPositionUpdate(collision.get(), actor, collision->GetNodeName(), model);
        break;
    }
}

//-----------------------------------
// 衝突時のリアクション処理
//-----------------------------------
void PlayerCollision::Reaction(int receiver, const Message& message)
{
    // 武器と衝突したコリジョンの持ち主に衝突したことを送信する
    MetaAI::Instance().SendMessaging(
        static_cast<int>(MetaAI::Identity::Collision), // 送信元
        receiver,                                // 受信先
        message);                                      // メッセージ
}
