#include "Collision.h"
#include "CullingCollision.h"
#include "CollisionManager.h"
#include "Universal.h"
#include "SceneManager.h"

//************************************
// 
// プレイヤーのカリングコリジョン
// 
//************************************
//-----------------------------------
// コンストラクタ
//-----------------------------------
CullingCollision::CullingCollision(EnemyCategory enemy_category, std::shared_ptr<Actor> actor)
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
    Universal::NodePositionUpdate(collision_culling.get(), collision_culling->GetNodeName(), model);

    // カリングを行うかの判定
    std::vector<Plane> frustum = SceneManager::Instance().GetCurrentScene()->GetCameraController()->GetFrustum();
    Reaction(CollisionManager::Instance().IntersectFrustumVsAABB(collision_culling, frustum));
}

//-----------------------------------
// 描画処理
//-----------------------------------
void CullingCollision::Render(DebugRenderer* renderer)
{
    if (!collision_culling->GetAttackFlag()) return;
    collision_culling->Render(renderer);
}

//-----------------------------------
// 衝突時のリアクション処理
//-----------------------------------
void CullingCollision::Reaction(bool flag)
{
    actor->SetCullingFlag(flag);
}
