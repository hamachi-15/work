#include <vector>
#include "Mathf.h"

#include "MetaAI.h"

#include "FireBall.h"
#include "Effect.h"

// マネージャーインクルード
#include "ActorManager.h"
#include "CollisionManager.h"
#include "FireBallManager.h"

#include "Charactor.h"
#include "Enemy.h"

#include "Model.h"

#include "GameDatabase.h"
//******************************
// 
// 火球クラス
// 
//******************************
//-------------------------------
// 開始処理
//-------------------------------
void FireBall::Start()
{
    // キャラクターの取得
    std::shared_ptr<Charactor> charactor = actor->GetComponent<Charactor>();

    // 当たり判定設定
    std::vector<std::shared_ptr<CollisionParameterData>> collision_parameter = GameDataBase::Instance().GetAttackCollitionParamterDataList(EnemyCategory::FireBall);
    CollisionParameter parameter;
    for (std::shared_ptr<CollisionParameterData> data : collision_parameter)
    {
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

        collision_sphere = std::make_shared<CollisionSphere>(parameter);

        // 球のコリジョンをマネージャーに設定
        CollisionManager::Instance().RegisterSphere(collision_sphere);
    }

    // モデル取得
    Model* model = actor->GetModel();

    // ノード取得
    Mathf::GetNodePosition(parameter.node_name.c_str(), fireball_position, model);

    // エフェクトマネージャー初期化
    effekseer_manager = std::make_unique<EffectManager>();
    effekseer_manager->Initialize();

    // エフェクト読み込み
    fireball_effect = std::make_unique<Effect>("Data/Effect/test.efk", effekseer_manager->GetEffekseerManager());

    fireball_position.y = 9;
    collision_sphere->SetPosition(fireball_position);
}

//-------------------------------
// 更新処理
//-------------------------------
void FireBall::Update(float elapsed_time)
{
    // モデル取得
    Model* model = actor->GetModel();

    // 球VS円柱
    int cylinder_count = CollisionManager::Instance().GetCollisionCylinderCount();
    for (int j = 0; j < cylinder_count; j++)
    {
        std::shared_ptr<CollisionCylinder> cylinder = CollisionManager::Instance().GetCollisionCylinder(j);
        // アクターがプレイヤーなら飛ばす
        if (collision_sphere->GetActorType() == cylinder->GetActorType()) continue;

        // 当たり判定フラグが立っていなかったら飛ばす
        if (!cylinder->GetCollisionFlag()) continue;
        ObjectCollisionResult result;
        if (CollisionManager::Instance().IntersectSphereVsCylinder(collision_sphere.get(), cylinder.get(), result))
        {
            // 円柱のアクター取得
            std::shared_ptr<Actor> cylinder_actor = ActorManager::Instance().
            GetActor(cylinder->GetActorName());

            Message message;
            // 攻撃が当たったことのメッセージ
            cylinder_actor->GetComponent<Charactor>()->ApplyDamage(attack, 0.0f);

            // 攻撃を当てたことのメッセージ
            // 敵コンポーネント取得
            std::shared_ptr<Enemy> enemy = actor->GetComponent<Enemy>();
            message.message = MessageType::Message_Hit_Attack;
            message.hit_position = { 0.0f, 0.0f, 0.0f };
            Reaction(collision_sphere->GetActorID() + enemy->GetIdentity(), message);
        }
    }

    // 火球エフェクトの座標処理
    float speed = this->speed * elapsed_time;
    fireball_position.x += direction.x * speed;
    fireball_position.z += direction.z * speed;
    // エフェクト再生
    fireball_effect->Play(effekseer_manager->GetEffekseerManager(), fireball_position, fireball_scale);

    // エフェクトマネージャー更新処理
    effekseer_manager->Update(elapsed_time);
    effekseer_manager->GetEffekseerManager()->StopAllEffects();
    // 当たり判定の座標設定
    collision_sphere->SetPosition(fireball_position);

    life_timer -= elapsed_time;

    if (life_timer <= 0.0f)
    {
        FireBallManager::Instance().Unregister(shared_from_this());
        CollisionManager::Instance().UnregisterSphere(collision_sphere);
    }
}

//-------------------------------
// 描画処理
//-------------------------------
void FireBall::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
    // 描画処理
    effekseer_manager->Render(view, projection);
}

//-------------------------------
// 衝突時のリアクション処理
//-------------------------------
void FireBall::Reaction(int receiver, const Message& message)
{
    // 武器と衝突したコリジョンの持ち主に衝突したことを送信する
    MetaAI::Instance().SendMessaging(
        static_cast<int>(MetaAI::Identity::Collision), // 送信元
        receiver,                                // 受信先
        message);                                      // メッセージ
}
