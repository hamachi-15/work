#include "EnemyCollision.h"
// メッセージ
#include "Telegram.h"

// AI
#include "MetaAI.h"

// モデル
#include "Model.h"

// 描画
#include "DebugRenderer.h"
#include "Graphics.h"

// マネージャー
#include "ActorManager.h"
#include "CollisionManager.h"
#include "SceneManager.h"

// コンポーネント
#include "Charactor.h"
#include "Enemy.h"

// エフェクト
#include "Effect.h"
#include "EffectManager.h"

// 汎用関数
#include "Universal.h"

// ゲームデータ
#include "GameDatabase.h"
#include "CollisionParameterData.h"
//************************************
// 
// プレイヤーのコリジョン
// 
//************************************
//-----------------------------------
// デストラクタ
//-----------------------------------
EnemyCollision::~EnemyCollision()
{
    // 球コリジョンの破棄処理
    std::vector<std::shared_ptr<CollisionSphere>>::iterator iterate_sphere = collision_spheres.begin();
    for (; iterate_sphere != collision_spheres.end(); iterate_sphere = collision_spheres.begin())
    {
        collision_spheres.erase(iterate_sphere);
    }
    collision_spheres.clear();

    // マネージャーの球コリジョン削除
    for (std::shared_ptr<CollisionSphere> sphere : collision_spheres)
    {  
        // 球コリジョン削除
    	CollisionManager::Instance().UnregisterSphere(sphere);
    }

    // マネージャーの円柱コリジョン削除
    // 円柱コリジョン削除
    CollisionManager::Instance().UnregisterCylinder(collision_cylinder);
}

//-----------------------------------
// GUI描画
//-----------------------------------
void EnemyCollision::OnGUI()
{

}

//-----------------------------------
// 開始処理
//-----------------------------------
void EnemyCollision::Start()
{
    // アクターの取得
    std::shared_ptr<Actor> actor = GetActor();

    // キャラクターの取得
    std::shared_ptr<Charactor> charactor = actor->GetComponent<Charactor>();

    // エフェクトマネージャー取得
    std::shared_ptr<EffectManager> effect_manager = Graphics::Instance().GetEffectManager();

    // エフェクト読み込み
    hit_effect = std::make_shared<Effect>("Data/Effect/Hit.efk", effect_manager->GetEffekseerManager());

    // モデル取得
    Model* model = actor->GetModel();
    std::vector<std::shared_ptr<CollisionParameterData>> collision_parameter = GameDataBase::Instance().GetAttackCollitionParamterDataList(enemy_category);
    for (std::shared_ptr<CollisionParameterData> data : collision_parameter)
    {
        CollisionParameter parameter;
        std::string collision_name = actor->GetName();
        parameter.name = collision_name + data->collision_name;
        parameter.actor_name = actor->GetName();
        parameter.node_name = data->node_name;
        parameter.actor_id = charactor->GetID() + identity;
        parameter.radius = data->radius;
        parameter.weight = data->weight;
        parameter.height = data->height;
        parameter.local_position.x = data->local_x;
        parameter.local_position.y = data->local_y;
        parameter.local_position.z = data->local_z;
        parameter.collision_flg = data->collision_flag;
        parameter.actor_type = data->actor_type;
        parameter.update_type = data->collision_update_type;
        // データのコリジョンタイプが円柱なら
        if (data->collision_type == CollisionMeshType::Cylinder)
        {
            collision_cylinder = std::make_shared<CollisionCylinder>(parameter);
            // 円柱のコリジョンをマネージャーに設定
            CollisionManager::Instance().RegisterCylinder(collision_cylinder);
        }
        // データのコリジョンタイプが球なら
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
void EnemyCollision::Update(float elapsed_time)
{
    // アクター取得
    std::shared_ptr<Actor> actor = GetActor();

    // モデル取得
    Model* model = GetActor()->GetModel();

    // 指定されたノードのローカル座標に更新
    for (std::shared_ptr<CollisionSphere> sphere : collision_spheres)
    {
        // コリジョンの座標更新のタイプに合わせて更新
        UpdateCollision(sphere, actor, model);
    }

    // コリジョンの座標更新のタイプに合わせて更新
    UpdateCollision(collision_cylinder, actor, model);
    std::shared_ptr<Charactor> charactor = GetActor()->GetComponent<Charactor>();

    // 球コリジョン取得
    int sphere_count = static_cast<int>(collision_spheres.size());

    // マネージャーの円柱の数取得
    int cylinder_count = static_cast<int>(CollisionManager::Instance().GetCollisionCylinderCount());

    // 敵の球コリジョンとマネージャーに設定されている円柱コリジョンの当たり判定
    for (int i = 0; i < sphere_count; i++)
    {
        // 球コリジョン取得
        CollisionSphere* sphere = collision_spheres.at(i).get();

        // 攻撃フラグが立っていなかったら飛ばす
        if (!sphere->GetAttackFlag()) continue;

        // 攻撃が当たっていた場合飛ばす
        if (charactor->GetHitAttackFlag()) break;

        for (int j = 0; j < cylinder_count; j++)
        {
            std::shared_ptr<CollisionCylinder> cylinder = CollisionManager::Instance().GetCollisionCylinder(j);
            // 同じアクター同士の当たり判定なら飛ばす
            if (sphere->GetActorType() == cylinder->GetActorType()) continue;

            // 当たり判定フラグが立っていなかったら飛ばす
            if (!cylinder->GetCollisionFlag()) continue;
            // 球と円柱が当たっていたら
            ObjectCollisionResult result;
            if (CollisionManager::Instance().IntersectSphereVsCylinder(sphere, cylinder.get(), result))
            {
                // 円柱コリジョンのアクター取得
                std::shared_ptr<Actor> cylinder_actor = ActorManager::Instance().GetActor(cylinder->GetActorName());

                // 円柱コリジョンを持っているキャラクターにダメージを与える
                cylinder_actor->GetComponent<Charactor>()->ApplyDamage(charactor->GetAttack(), 0.8f);

                // 円柱コリジョンを持つアクター位置取得
                DirectX::XMFLOAT3 cylinder_actor_position = cylinder_actor->GetPosition();

                // エフェクトの再生位置を設定
                DirectX::XMFLOAT3 diff;
                DirectX::XMStoreFloat3(&diff, result.vector);
                DirectX::XMFLOAT3 sphere_position = sphere->GetPosition();
                DirectX::XMFLOAT3 play_position = {
                    sphere_position.x - diff.x,
                    sphere_position.y - diff.y,
                    sphere_position.z - diff.z };
                // ヒットエフェクト再生
                hit_effect->Play(Graphics::Instance().GetEffectManager()->GetEffekseerManager(),
                    play_position, hit_effect_scale);

                // リアクションをメッセージに送る
                Message message;
                message.message = MessageType::Message_Hit_Attack;
                message.hit_position = { 0.0f, 0.0f, 0.0f };
                Reaction(sphere->GetActorID(), message);
            }
        }
    }


    // 当たり判定フラグが立っていなかったら飛ばす
    if (!collision_cylinder->GetCollisionFlag()) return;

    // 円柱Vs円柱
    ObjectCollisionResult result;
    for (int j = 0; j < cylinder_count; j++)
    {
        std::shared_ptr<CollisionCylinder> cylinderB = CollisionManager::Instance().GetCollisionCylinder(j);
        // 当たり判定フラグが立っていなかったら飛ばす
        if (!cylinderB->GetCollisionFlag()) continue;

        // 同じアクター同士の当たり判定なら飛ばす
        if (collision_cylinder->GetActorType() == cylinderB->GetActorType()) continue;

        std::shared_ptr<Actor> cylinderB_actor = ActorManager::Instance().GetActor(cylinderB->GetActorName());
        // 円柱同士が当たっていたら
        if (CollisionManager::Instance().IntersectCylinderVsCylinder(collision_cylinder.get(), GetActor(), cylinderB.get(), cylinderB_actor, result))
        {
            CollisionManager::Instance().PushOutCollision(collision_cylinder.get(), GetActor(), cylinderB.get(), cylinderB_actor, result);
        }
    }

}

//--------------------------------
// コリジョン更新処理
//--------------------------------
void EnemyCollision::UpdateCollision(std::shared_ptr<CollisionObject> collision, std::shared_ptr<Actor> actor, Model* model)
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
void EnemyCollision::Reaction(int receiver, const Message& message)
{
    // 武器と衝突したコリジョンの持ち主に衝突したことを送信する
    MetaAI::Instance().SendMessaging(
        static_cast<int>(MetaAI::Identity::Collision),  // 送信元
        receiver,                                       // 受信先
        message);                                       // メッセージ
}
