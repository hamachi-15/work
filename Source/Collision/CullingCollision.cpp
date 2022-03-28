#include <string>
#include "Collision.h"
#include "CullingCollision.h"
#include "CollisionManager.h"
#include "Universal.h"
#include "SceneManager.h"

// ゲームデータ
#include "GameDatabase.h"
#include "ActorType.h"
#include "CullingCollisionParameterData.h"
//************************************
// 
// カリングコリジョン
// 
//************************************
//-----------------------------------
// コンストラクタ
//-----------------------------------
CullingCollision::CullingCollision(ActorType enemy_category, std::shared_ptr<Actor> actor)
{
    // アクター設定
    this->actor = actor;

    // モデル取得
    Model* model = this->actor->GetModel();

    std::vector<std::shared_ptr<CullingCollisionParameterData>> collision_parameter = GameDataBase::Instance().GetAttackCullingCollisionParameterDataList(enemy_category);
    for (std::shared_ptr<CullingCollisionParameterData> data : collision_parameter)
    {
        CollisionParameter parameter;
        parameter.name = this->actor->GetName();
        parameter.actor_name = this->actor->GetName();
        parameter.node_name = data->node_name;
        parameter.xmfloat_radius = { data->radius_x, data->radius_y,data->radius_z };
        parameter.local_position = { data->local_x, data->local_y, data->local_z };
        DirectX::XMFLOAT3 node_position = {};
        parameter.update_type = data->update_type;
        //Mathf::GetNodePosition(parameter.node_name.c_str(), node_position, model);
        //parameter.position = { node_position.x, node_position.y + 0.5f, node_position.z };
        collision_culling = std::make_shared<CollisionBox>(parameter);
    }
}

//-----------------------------------
// デストラクタ
//-----------------------------------
CullingCollision::~CullingCollision()
{
}

//-----------------------------------
// GUI描画
//-----------------------------------
void CullingCollision::OnGUI()
{
}

//-----------------------------------
// 開始処理
//-----------------------------------
void CullingCollision::Start()
{
}

//-----------------------------------
// 更新処理
//-----------------------------------
void CullingCollision::Update()
{
    // モデル取得
    Model* model = actor->GetModel();

    // 指定されたノードのローカル座標に更新
    // 更新方法がその他に設定されていなければ
    if (collision_culling->GetUpdateType() == CollisionUpdateType::Update_Node)
    {// 特定のノードの位置に更新
        Universal::NodePositionUpdate(collision_culling.get(), collision_culling->GetNodeName(), model);
    }
    else if (collision_culling->GetUpdateType() == CollisionUpdateType::Update_Local)
    {
        Model::Node* node = model->FindNode(collision_culling->GetNodeName());
        Universal::LocalPositionUpdate(collision_culling.get(), node);
    }

    // カリングを行うかの判定
    std::vector<Plane> frustum = SceneManager::Instance().GetCurrentScene()->GetCameraController()->GetFrustum();
    Reaction(CollisionManager::Instance().IntersectFrustumVsAABB(collision_culling, frustum));
}

//-----------------------------------
// 描画処理
//-----------------------------------
void CullingCollision::Render(DebugRenderer* renderer)
{
    // デバッグプリミティブ描画
    collision_culling->Render(renderer);
}

//-----------------------------------
// 衝突時のリアクション処理
//-----------------------------------
void CullingCollision::Reaction(bool flag)
{
    // カリングフラグの設定
    actor->SetCullingFlag(flag);
}
