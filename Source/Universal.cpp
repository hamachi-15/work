#include "Universal.h"
#include "Enemy.h"
#include "Charactor.h"
#include "Mathf.h"
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
	bool collision_time_flag = charactor->SearchAnimationTime(actor, 0.2f, 0.6f);

	// コリジョンフラグが変化したら
	if (old_collision_time_flag != collision_time_flag)
	{
		// 前フレームのコリジョンフラグを代入。コリジョンフラグが変化したタイミングを調べる用
		charactor->SetOldCollisionTimeFlag(collision_time_flag);
		return collision_time_flag;
	}

	return NULL;
}
