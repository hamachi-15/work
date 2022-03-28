#include "StageObjectCollision.h"
// データベース
#include "GameDatabase.h"
#include "ActorType.h"
#include "CollisionParameterData.h"

// マネージャー
#include "CollisionManager.h"

// コンポーネント
#include "Actor.h"
#include "Charactor.h"

// 汎用関数
#include "Universal.h"

//*************************************
// 
// オブジェクトのコリジョン
// 
//*************************************
//-----------------------
// GUI描画
//-----------------------
void StageObjectCollision::OnGUI()
{
}

//-----------------------
// 開始処理
//-----------------------
void StageObjectCollision::Start()
{
    // アクターの取得
    std::shared_ptr<Actor> actor = GetActor();

    // キャラクターの取得
    std::shared_ptr<Charactor> charactor = actor->GetComponent<Charactor>();

    // コリジョンのデータからコリジョンを設定
    std::vector<std::shared_ptr<CollisionParameterData>> collision_parameter = GameDataBase::Instance().GetAttackCollitionParamterDataList(category);
    for (std::shared_ptr<CollisionParameterData> data : collision_parameter)
    {
        CollisionParameter parameter;
        std::string collision_name = actor->GetName();
        parameter.name = collision_name + data->collision_name;
        parameter.actor_name = actor->GetName();
        parameter.node_name = data->node_name;
        //parameter.actor_id = charactor->GetID();
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
    }
}

//-----------------------
// 更新処理
//-----------------------
void StageObjectCollision::Update(float elapsed_time)
{
    // アクター取得
    std::shared_ptr<Actor> actor = GetActor();

    // アクター座標にコリジョンを更新
    Universal::ActorPositionUpdate(collision_cylinder.get(), actor);
}
