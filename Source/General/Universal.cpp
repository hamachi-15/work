#include "Universal.h"
#include "Enemy.h"
#include "Charactor.h"
#include "Mathf.h"

#include "Model.h"
#include "Collision.h"

// ゲームデータ
#include "AttackCollitionTime.h"
//**********************************
// 
// 汎用関数クラス
// 
//**********************************
//-------------------------------------------
// ターゲットが前方にいるか判定
//-------------------------------------------
bool Universal::JudgementTargetInFront(const DirectX::XMFLOAT3& origine_position, const DirectX::XMFLOAT3& origine_angle, const DirectX::XMFLOAT3& target_position)
{
	// 敵とプレイヤーのベクトル算出
	DirectX::XMVECTOR vector = Mathf::ReturnVectorSubtract(target_position, origine_position);

	// 敵の角度から前方向を算出
	DirectX::XMVECTOR front = Mathf::ReturnVectorFront(origine_angle);

	// 敵の前方向とベクトルから内積算出
	float dot = Mathf::ReturnFloatDot(front, vector);

	// 内積の正負で前方にいるか判定
	return (dot < 0);
}

//-------------------------------------------
// 当たり判定を行う時間かの判定
//-------------------------------------------
bool Universal::JudgementCollisionTime(std::shared_ptr<Actor> actor, std::shared_ptr<AttackCollitionTime> collision_time_data)
{
	std::shared_ptr<Charactor> charactor = actor->GetComponent<Charactor>();

	// 現在のコリジョンフラグと1フレーム前のコリジョンフラグを取得
	bool old_collision_time_flag = charactor->GetOldCollisionTimeFlag();

	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	bool collision_time_flag = charactor->SearchAnimationTime(actor, collision_time_data->start_time, collision_time_data->end_time);

	// コリジョンフラグが変化したら
	if (old_collision_time_flag != collision_time_flag)
	{
		// 前フレームのコリジョンフラグを代入。コリジョンフラグが変化したタイミングを調べる用
		charactor->SetOldCollisionTimeFlag(collision_time_flag);
		return true;
	}

	return false;
}

//-------------------------------------------
// コリジョン座標を指定のノード座標へ更新
//-------------------------------------------
void Universal::NodePositionUpdate(CollisionObject* collision_object, std::string node_name, Model* model)
{
	// ノード座標取得
	DirectX::XMFLOAT3 position;
	Mathf::GetNodePosition(node_name.c_str(), position, model);

	// 座標設定
	collision_object->SetPosition(position);
}

//-------------------------------------------
// コリジョン座標を指定のローカル座標へ更新
//-------------------------------------------
void Universal::LocalPositionUpdate(CollisionObject* collision_object, Model::Node* node)
{

	DirectX::XMMATRIX world_transform_matrix = DirectX::XMLoadFloat4x4(&node->world_transform);
	DirectX::XMFLOAT3 local_position = collision_object->GetLocalPosition();
	
	DirectX::XMVECTOR position = DirectX::XMVector3TransformCoord(
		DirectX::XMLoadFloat3(&local_position), world_transform_matrix);

	DirectX::XMFLOAT3 collision_position;
	DirectX::XMStoreFloat3(&collision_position, position);
	
	// 座標設定
	collision_object->SetPosition(collision_position);
}

//-------------------------------------------
// コリジョン座標を指定のアクター座標へ更新
//-------------------------------------------
void Universal::ActorPositionUpdate(CollisionObject* collision_object, std::shared_ptr<Actor> actor)
{
	// 座標設定
	collision_object->SetPosition(actor->GetPosition());
}

//-------------------------------------------
// コリジョン座標を指定のアクター、
// 指定のローカル座標ではない座標へ更新
//-------------------------------------------
void Universal::CastamPositionUpdate(CollisionObject* collision_object, std::shared_ptr<Actor> actor, std::string node_name, Model* model)
{
	DirectX::XMFLOAT3 position;
	Mathf::GetNodePosition(node_name.c_str(), position, model);
	DirectX::XMFLOAT3 actor_position = actor->GetPosition();
	DirectX::XMFLOAT3 castam_poition = { actor_position.x, position.y - collision_object->GetLocalPosition().y, actor_position.z };
	collision_object->SetPosition(castam_poition);
}
