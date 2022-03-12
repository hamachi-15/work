#include <algorithm>
#include <imgui.h>
#include "Mathf.h"
#include "Movement.h"
#include "Player.h"
#include "ActorManager.h"
#include "CollisionManager.h"
//----------------------------
// �R���X�g���N�^
//----------------------------
Movement::Movement()
{
}

//----------------------------
// �f�X�g���N�^
//----------------------------
Movement::~Movement()
{
}

//----------------------------
// GUI�`��
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
// �J�n����
//----------------------------
void Movement::Start()
{
}

//----------------------------
// �X�V����
//----------------------------
void Movement::Update(float elapsed_time)
{
	// ���͍X�V����
	UpdateVelocity(elapsed_time);
}

//----------------------------
// ���͍X�V����
//----------------------------
void Movement::UpdateVelocity(float elapsed_time)
{
	// �o�߃t���[��
	float elapsed_frame = 60.0f * elapsed_time;

	// �������͍X�V����
	UpdateVerticalVelocity(elapsed_frame);

	// �������͍X�V����
	UpdateHorizontalVelocity(elapsed_frame);

	// �����ړ��X�V����
	UpdateVerticalMove(elapsed_time);

	// �����ړ��X�V����
	UpdateHorizontalMove(elapsed_time);
}

//----------------------------
// �ړ�
//----------------------------
void Movement::Move(const DirectX::XMFLOAT3& direction , float speed)
{
	// �ړ������x�N�g����ݒ�
	move_vecX = direction.x;
	move_vecZ = direction.z;

	// �ő呬�x�ݒ�
	max_move_speed = speed;
}

//----------------------------
// �ړ�
//----------------------------
void Movement::MoveLocal(const DirectX::XMFLOAT3& direction, float elapsed_time)
{
}

//----------------------------
// ����
//----------------------------
float Movement::Turn(DirectX::XMFLOAT3& direction)
{
	std::shared_ptr<Actor> actor = GetActor();

	// �i�s�x�N�g�����[���x�N�g���̏ꍇ�͏�������K�v�Ȃ�
	float length = sqrtf(direction.x * direction.x + direction.z * direction.z);
	if (length <= 0.001f)
	{
		return 0;
	}
	// �i�s�x�N�g����P�ʃx�N�g����
	direction.x /= length;
	direction.z /= length;

	// ���g�̉�]�l����O���������߂�
	DirectX::XMFLOAT3 angle = actor->GetAngle();
	float frontX = sinf(angle.y);
	float frontZ = cosf(angle.y);

	// ��]�p�����߂邽�߁A2�̒P�ʃx�N�g���̓��ς��v�Z����
	float dot = frontX * direction.x + frontZ * direction.z;

	// ���ϒl��-1.0�`1.0�ŕ\������Ă���A2�̒P�ʃx�N�g���̊p�x��
	// �������ق�1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����
	float rot = 1.0f - dot;

	// ���E������s�����߂ɂQ�̒P�ʃx�N�g���̊O�ς��v�Z����
	float cross = (frontX * direction.z) - (frontZ * direction.x);

	// 2D�̊O�ϒl�����̏ꍇ�����̏ꍇ���ɂ���č��E���肪�s����
	// ���E������s�����Ƃɂ���č��E��]��I������
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
// �W�����v����
//----------------------------
void Movement::Jump(float speed)
{
	// ������̗͂�ݒ�
	velocity.y = speed;
}
//----------------------------
// �󒆃_�b�V������
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
// �������͍X�V����
//----------------------------
void Movement::UpdateVerticalVelocity(float elapsed_frame)
{
	// �d�͏���
	gravity_cut_time -= elapsed_frame;
	if (gravity_cut_time <= 0.0f)
	{
		velocity.y += gravity * elapsed_frame;
	}
}

//----------------------------
// �����ړ��X�V����
//----------------------------
void Movement::UpdateVerticalMove(float elapsed_time)
{
	std::shared_ptr<Actor> actor = GetActor();
	std::shared_ptr<Actor> actor_stage = ActorManager::Instance().GetActor("Filde");
	std::shared_ptr<Actor> actor_player = ActorManager::Instance().GetActor("Player");
	std::shared_ptr<Player> player = actor_player->GetComponent<Player>();

	float my = velocity.y * elapsed_time;
	
	// �L�����N�^�[��Y�������ƂȂ�@���x�N�g��
	DirectX::XMFLOAT3 normal = { 0, 1, 0 };

	slope_rate = 0.0f;

	// ������
	if (my < 0.0f)
	{
		DirectX::XMFLOAT3 position = actor->GetPosition();
		// ���C�̊J�n�ʒu�B������菭����
		DirectX::XMFLOAT3 start = { position.x, position.y + step_offset, position.z };
		// ���C�̏I���ʒu�B�ړ���̈ʒu��
		DirectX::XMFLOAT3 end = { position.x, position.y + my, position.z };

		// ���C�L���X�g�ɂ��n�ʔ���
		HitResult hit;
		if(CollisionManager::Instance().IntersectRayVsModel(start, end, actor_stage->GetModel(), hit))
		{
			// �n�ʂɐڒn���Ă���
			position.y = hit.position.y;
			actor->SetPosition(position);

			// �X�Η��̌v�Z
			float normal_lengthXZ = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
			slope_rate = 1.0f - (hit.normal.y / (normal_lengthXZ + hit.normal.y));

			normal = hit.normal;

			isground = true;
			velocity.y = 0.0f;
		}
		else
		{
			// �󒆂ɕ����Ă���
			position.y += my;
			actor->SetPosition(position);
			isground = false;
		}
	}
	// �㏸��
	else if(my > 0.0f)
	{
		DirectX::XMFLOAT3 position = actor->GetPosition();
		isground = false;
		position.y += my;
		actor->SetPosition(position);
	}
}

//----------------------------
// �������͍X�V����
//----------------------------
void Movement::UpdateHorizontalVelocity(float elapsed_frame)
{
	// XZ���ʂ̑��͂���������
	float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
	if (length > 0.0f)
	{
		// ���C��
		float friction = this->friction * elapsed_frame;

		// �󒆂ɂ���Ƃ��͖��C�͂����炷
		if (!isground) friction *= air_control;
		
		// ���C�ɂ�鉡�����̌�������
		if (length > friction)
		{
			// �P�ʃx�N�g����
			float vx = velocity.x / length;
			float vz = velocity.z / length;

			velocity.x -= vx * friction;
			velocity.z -= vz * friction;
		}
		else 
		{
			// �������̑��x�����C�͈ȉ��ɂȂ����̂ő��͂𖳌���
			velocity.x = 0.0f;
			velocity.z = 0.0f;
		}
	}

	// XZ���ʂ̑��͂���������
	if (length <= max_move_speed)
	{
		// �ړ��x�N�g�����[���x�N�g���ł͂Ȃ��Ȃ��������
		float move_vec_length = sqrtf(move_vecX * move_vecX + move_vecZ * move_vecZ);
		if (move_vec_length > 0.0f)
		{
			// ������
			float acceleration = this->acceleration * elapsed_frame;
			if (!isground)acceleration *= air_control;

			// �ړ��x�N�g���ɂ���������
			velocity.x += move_vecX * acceleration;
			velocity.z += move_vecZ * acceleration;

			// �ő呬�x����
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
	// �ړ��x�N�g�������Z�b�g
	move_vecX = 0.0f;
	move_vecZ = 0.0f;
}

//----------------------------
// �����ړ��X�V����
//----------------------------
void Movement::UpdateHorizontalMove(float elapsed_time)
{
	std::shared_ptr<Actor> actor = GetActor();
	std::shared_ptr<Actor> actor_stage = ActorManager::Instance().GetActor("Filde");;

	DirectX::XMFLOAT3 position = actor->GetPosition();

	float velocity_lengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
	if (velocity_lengthXZ > 0.0f)
	{
		// �����ړ��l
		float mx = velocity.x * elapsed_time;
		float mz = velocity.z * elapsed_time;

		// ���C�Ƃ̊J�n�ʒu�ƏI�_�ʒu
		DirectX::XMFLOAT3 start = { position.x , position.y + step_offset , position.z };
		DirectX::XMFLOAT3 end = { position.x + mx , position.y + step_offset , position.z + mz };

		// TODO ���C�L���X�g�ŕǂ��蓙
		HitResult hit;
		if (CollisionManager::Instance().IntersectRayVsModel(start, end, actor_stage->GetModel(), hit))
		{
			// �ǂ܂ł̃x�N�g��
			DirectX::XMVECTOR _start = DirectX::XMLoadFloat3(&start);
			DirectX::XMVECTOR _end = DirectX::XMLoadFloat3(&end);
			DirectX::XMVECTOR vec = DirectX::XMVectorSubtract(_end, _start);

			// �ǂ̖@��
			DirectX::XMVECTOR normal = DirectX::XMLoadFloat3(&hit.normal);

			//���˃x�N�g����@���Ɏˉe
			DirectX::XMVECTOR dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(vec), normal);

			// �␳�ʒu�̌v�Z
			DirectX::XMVECTOR CollectPosition = DirectX::XMVectorMultiplyAdd(normal, dot, _end);
			DirectX::XMFLOAT3 collect_position;
			DirectX::XMStoreFloat3(&collect_position, CollectPosition);

			// �ǂ�������փ��C�L���X�g
			HitResult hit2;
			if (!CollisionManager::Instance().IntersectRayVsModel(hit.position, collect_position, actor_stage->GetModel(), hit2))
			{
				// �ǂ�������ŕǂɓ�����Ȃ�������␳�ʒu�Ɉړ�
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
