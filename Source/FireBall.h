#pragma once
#include <Effekseer.h>
#include "Collision.h"
#include "Effect.h"
#include "EffectManager.h"

class Actor;

//******************************
// 
// 火球クラス
// 
//******************************
class FireBall : public std::enable_shared_from_this<FireBall>
{
public:
	// コンストラクタ
	FireBall(std::shared_ptr<Actor> actor, DirectX::XMFLOAT3 direction) : actor(actor), direction(direction){}
	
	// デストラクタ
	~FireBall() {}

	// 開始処理
	void Start();

	// 更新処理
	void Update(float elapsed_time);

	// 描画処理
	void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

	// 衝突時のリアクション処理
	void Reaction(int receiver, const Message& message);
private:
	std::shared_ptr<Actor>				actor;
	std::unique_ptr<Effect>				fireball_effect;	// 火球のエフェクト
	std::unique_ptr<EffectManager>		effekseer_manager;
	std::shared_ptr<CollisionSphere>	collision_sphere;	// 当たり判定
	Effekseer::Handle					handle;				// エフェクトのハンドル
	
	DirectX::XMFLOAT3					fireball_position;	// 初期位置
	float								fireball_scale = 5.0f;		// エフェクトのスケール
	DirectX::XMFLOAT3					direction;			// 進行ベクトル
	float								speed = 100.0f;		// 火球の移動速度

	float								attack = 5.0f;		// 攻撃力
	float								life_timer = 4.0f;	// 生存時間
};