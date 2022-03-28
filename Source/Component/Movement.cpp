#include <algorithm>
#include <imgui.h>
#include "Mathf.h"
#include "Movement.h"
#include "Player.h"
#include "ActorManager.h"
#include "CollisionManager.h"
//----------------------------
// コンストラクタ
//----------------------------
Movement::Movement()
{
}

//----------------------------
// デストラクタ
//----------------------------
Movement::~Movement()
{
}

//----------------------------
// GUI描画
//----------------------------
void Movement::OnGUI()
{
	ImGui::InputFloat("MoveMaxSpeed", &max_move_speed);
	ImGui::InputFloat("Turn Speed", &turn_speed);
	ImGui::InputFloat3("Velocity", &velocity.x);
	int ground = isground;
	ImGui::InputInt("IsGround", &ground);
}

//----------------------------
// 開始処理
//----------------------------
void Movement::Start()
{
}

//----------------------------
// 更新処理
//----------------------------
void Movement::Update(float elapsed_time)
{
	// 速力更新処理
	UpdateVelocity(elapsed_time);
}

//----------------------------
// 速力更新処理
//----------------------------
void Movement::UpdateVelocity(float elapsed_time)
{
	// 経過フレーム
	float elapsed_frame = 60.0f * elapsed_time;

	// 垂直速力更新処理
	UpdateVerticalVelocity(elapsed_frame);

	// 水平速力更新処理
	UpdateHorizontalVelocity(elapsed_frame);

	// 垂直移動更新処理
	UpdateVerticalMove(elapsed_time);

	// 水平移動更新処理
	UpdateHorizontalMove(elapsed_time);
}

//----------------------------
// 移動
//----------------------------
void Movement::Move(const DirectX::XMFLOAT3& direction , float speed)
{
	// 移動方向ベクトルを設定
	move_vecX = direction.x;
	move_vecZ = direction.z;

	// 最大速度設定
	max_move_speed = speed;
}

//----------------------------
// 移動
//----------------------------
void Movement::MoveLocal(const DirectX::XMFLOAT3& direction, float elapsed_time)
{
}

//----------------------------
// 旋回
//----------------------------
float Movement::Turn(DirectX::XMFLOAT3& direction)
{
	std::shared_ptr<Actor> actor = GetActor();

	// 進行ベクトルがゼロベクトルの場合は処理する必要なし
	float length = sqrtf(direction.x * direction.x + direction.z * direction.z);
	if (length <= 0.001f)
	{
		return 0;
	}
	// 進行ベクトルを単位ベクトル化
	direction.x /= length;
	direction.z /= length;

	// 自身の回転値から前方向を求める
	DirectX::XMFLOAT3 angle = actor->GetAngle();
	float frontX = sinf(angle.y);
	float frontZ = cosf(angle.y);

	// 回転角を求めるため、2つの単位ベクトルの内積を計算する
	float dot = frontX * direction.x + frontZ * direction.z;

	// 内積値は-1.0～1.0で表現されており、2つの単位ベクトルの角度が
	// 小さいほど1.0に近づくという性質を利用して回転速度を調整する
	float rot = 1.0f - dot;

	// 左右判定を行うために２つの単位ベクトルの外積を計算する
	float cross = (frontX * direction.z) - (frontZ * direction.x);

	// 2Dの外積値が正の場合か負の場合かによって左右判定が行える
	// 左右判定を行うことによって左右回転を選択する
	if (cross < 0.0f)
	{
		actor->AddAngle({ 0, rot, 0 });
	}
	else
	{
		actor->SubtractAngle({ 0, rot, 0 });
	}
	return dot;
}

//----------------------------
// ジャンプ処理
//----------------------------
void Movement::Jump(float speed)
{
	// 上方向の力を設定
	velocity.y = speed;
}
//----------------------------
// 空中ダッシュ処理
//----------------------------
void Movement::AirDush(float gravity_cut_time)
{
	velocity.x *= 2;
	velocity.z *= 2;
	velocity.y = 0;
	this->gravity_cut_time = gravity_cut_time;
}

void Movement::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
	velocity.x = impulse.x;
	velocity.y = impulse.y;
	velocity.z = impulse.z;
}

//----------------------------
// 垂直速力更新処理
//----------------------------
void Movement::UpdateVerticalVelocity(float elapsed_frame)
{
	// 重力処理
	gravity_cut_time -= elapsed_frame;
	if (gravity_cut_time <= 0.0f)
	{
		velocity.y += gravity * elapsed_frame;
	}
}

//----------------------------
// 垂直移動更新処理
//----------------------------
void Movement::UpdateVerticalMove(float elapsed_time)
{
	std::shared_ptr<Actor> actor = GetActor();
	std::shared_ptr<Actor> actor_stage = ActorManager::Instance().GetActor("Filde");
	std::shared_ptr<Actor> actor_player = ActorManager::Instance().GetActor("Player");
	std::shared_ptr<Player> player = actor_player->GetComponent<Player>();

	float my = velocity.y * elapsed_time;
	
	// キャラクターのY軸方向となる法線ベクトル
	DirectX::XMFLOAT3 normal = { 0, 1, 0 };

	slope_rate = 0.0f;

	// 落下中
	if (my < 0.0f)
	{
		DirectX::XMFLOAT3 position = actor->GetPosition();
		// レイの開始位置。足元より少し上
		DirectX::XMFLOAT3 start = { position.x, position.y + step_offset, position.z };
		// レイの終了位置。移動後の位置に
		DirectX::XMFLOAT3 end = { position.x, position.y + my, position.z };

		// レイキャストによる地面判定
		HitResult hit;
		if(CollisionManager::Instance().IntersectRayVsModel(start, end, actor_stage->GetModel(), hit))
		{
			// 地面に接地している
			position.y = hit.position.y;
			actor->SetPosition(position);

			// 傾斜率の計算
			float normal_lengthXZ = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
			slope_rate = 1.0f - (hit.normal.y / (normal_lengthXZ + hit.normal.y));

			normal = hit.normal;

			isground = true;
			velocity.y = 0.0f;
		}
		else
		{
			// 空中に浮いている
			position.y += my;
			actor->SetPosition(position);
			isground = false;
		}
	}
	// 上昇中
	else if(my > 0.0f)
	{
		DirectX::XMFLOAT3 position = actor->GetPosition();
		isground = false;
		position.y += my;
		actor->SetPosition(position);
	}
}

//----------------------------
// 水平速力更新処理
//----------------------------
void Movement::UpdateHorizontalVelocity(float elapsed_frame)
{
	// XZ平面の速力を減速する
	float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
	if (length > 0.0f)
	{
		// 摩擦力
		float friction = this->friction * elapsed_frame;

		// 空中にいるときは摩擦力を減らす
		if (!isground) friction *= air_control;
		
		// 摩擦による横方向の減速処理
		if (length > friction)
		{
			// 単位ベクトル化
			float vx = velocity.x / length;
			float vz = velocity.z / length;

			velocity.x -= vx * friction;
			velocity.z -= vz * friction;
		}
		else 
		{
			// 横方向の速度が摩擦力以下になったので速力を無効化
			velocity.x = 0.0f;
			velocity.z = 0.0f;
		}
	}

	// XZ平面の速力を加速する
	if (length <= max_move_speed)
	{
		// 移動ベクトルがゼロベクトルではないなら加速する
		float move_vec_length = sqrtf(move_vecX * move_vecX + move_vecZ * move_vecZ);
		if (move_vec_length > 0.0f)
		{
			// 加速力
			float acceleration = this->acceleration * elapsed_frame;
			if (!isground)acceleration *= air_control;

			// 移動ベクトルによる加速処理
			velocity.x += move_vecX * acceleration;
			velocity.z += move_vecZ * acceleration;

			// 最大速度制限
			float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
			if (length > max_move_speed)
			{
				float vx = velocity.x / length;
				float vz = velocity.z / length;

				velocity.x = vx * max_move_speed;
				velocity.z = vz * max_move_speed;
			}
			if (isground && slope_rate > 0.0f)
			{
				//velocity.y -= length * slope_rate * elapsed_frame;
			}
		}
	}
	// 移動ベクトルをリセット
	move_vecX = 0.0f;
	move_vecZ = 0.0f;
}

//----------------------------
// 水平移動更新処理
//----------------------------
void Movement::UpdateHorizontalMove(float elapsed_time)
{
	std::shared_ptr<Actor> actor = GetActor();
	std::shared_ptr<Actor> actor_stage = ActorManager::Instance().GetActor("Filde");;

	DirectX::XMFLOAT3 position = actor->GetPosition();

	float velocity_lengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
	if (velocity_lengthXZ > 0.0f)
	{
		// 水平移動値
		float mx = velocity.x * elapsed_time;
		float mz = velocity.z * elapsed_time;

		// レイとの開始位置と終点位置
		DirectX::XMFLOAT3 start = { position.x , position.y + step_offset , position.z };
		DirectX::XMFLOAT3 end = { position.x + mx , position.y + step_offset , position.z + mz };

		// TODO レイキャストで壁ずり等
		HitResult hit;
		if (CollisionManager::Instance().IntersectRayVsModel(start, end, actor_stage->GetModel(), hit))
		{
			// 壁までのベクトル
			DirectX::XMVECTOR _start = DirectX::XMLoadFloat3(&start);
			DirectX::XMVECTOR _end = DirectX::XMLoadFloat3(&end);
			DirectX::XMVECTOR vec = DirectX::XMVectorSubtract(_end, _start);

			// 壁の法線
			DirectX::XMVECTOR normal = DirectX::XMLoadFloat3(&hit.normal);

			//入射ベクトルを法線に射影
			DirectX::XMVECTOR dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(vec), normal);

			// 補正位置の計算
			DirectX::XMVECTOR CollectPosition = DirectX::XMVectorMultiplyAdd(normal, dot, _end);
			DirectX::XMFLOAT3 collect_position;
			DirectX::XMStoreFloat3(&collect_position, CollectPosition);

			// 壁ずり方向へレイキャスト
			HitResult hit2;
			if (!CollisionManager::Instance().IntersectRayVsModel(hit.position, collect_position, actor_stage->GetModel(), hit2))
			{
				// 壁ずり方向で壁に当たらなかったら補正位置に移動
				position.x = collect_position.x;
				position.z = collect_position.z;
				actor->SetPosition(position);
			}
			else
			{
				position.x = hit2.position.x;
				position.z = hit2.position.z;
				actor->SetPosition(position);
			}
		}
		else
		{
			position.x += mx;
			position.z += mz;
			actor->SetPosition(position);
		}
	}
}
