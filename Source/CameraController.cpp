#include "CameraController.h"
#include "Input.h"
#include "Camera.h"
#include "Messenger.h"

//-----------------------------
// �R���X�g���N�^
//-----------------------------
CameraController::CameraController()
{
	position = Camera::Instance().GetEye();
	new_position = Camera::Instance().GetEye();

	Camera_Change_FreeMode_key = Messenger::Instance().AddReceiver(MessageData::CameraChangeFreeMode, [&](void* data) { OnFreeMode(data); });
	Camera_Change_LockonMode_key = Messenger::Instance().AddReceiver(MessageData::CameraChangeLockonMode, [&](void* data) { OnLockonMode(data); });
	Camera_Change_MotionMode_key = Messenger::Instance().AddReceiver(MessageData::CameraChangeMotionMode, [&](void* data) { OnMotionMode(data); });
}

//-----------------------------
// �f�X�g���N�^
//-----------------------------
CameraController::~CameraController()
{
	//Messenger::Instance().RemoveReceiver(Camera_Change_FreeMode_key);
	//Messenger::Instance().RemoveReceiver(Camera_Change_LockonMode_key);
	//Messenger::Instance().RemoveReceiver(Camera_Change_MotionMode_key);
}

//-----------------------------
// �X�V����
//-----------------------------
void CameraController::Update(float elapsed_time)
{
	// �e���[�h�ł̏���
	switch (mode)
	{
	case	Mode::FreeCamera:	UpdateFreeCamera(elapsed_time);	break;
	case	Mode::LockonCamera:	UpdateLockonCamera(elapsed_time);	break;
	case	Mode::MotionCamera:	UpdateMotionCamera(elapsed_time);	break;
	}

	// ���X�ɖڕW�ɋ߂Â���
	static	constexpr	float	speed = 1.0f / 8.0f;
	position.x += (new_position.x - position.x) * speed;
	position.y += (new_position.y - position.y) * speed;
	position.z += (new_position.z - position.z) * speed;
	target.x   += (new_target.x - target.x) * speed;
	target.y   += (new_target.y - target.y) * speed;
	target.z   += (new_target.z - target.z) * speed;

	// �J�����̎��_�ƒ����_�ɐݒ�
	Camera::Instance().SetLookAt(position, this->target, DirectX::XMFLOAT3(0, 1, 0));
}

//-----------------------------
// �t���[�J����
//-----------------------------
void CameraController::OnFreeMode(void* data)
{
	MessageData::CameraChangeFreeModeData* camera_data = static_cast<MessageData::CameraChangeFreeModeData*>(data);

	if (mode != Mode::FreeCamera)
	{
		// �p�x�Z�o
		DirectX::XMFLOAT3 vector;
		vector.x = new_position.x - new_target.x;
		vector.y = new_position.y - new_target.y;
		vector.z = new_position.z - new_target.z;
		angle.y = atan2f(sinf(angle.y), cosf(angle.y));
		angle.x = atan2f(sinf(angle.x), cosf(angle.x));
	}
	mode = Mode::FreeCamera;
	new_target = camera_data->target;
	new_target.y += 0.01f;
}

//-----------------------------
// ���b�N�I���J����
//-----------------------------
void CameraController::OnLockonMode(void* data)
{
	MessageData::CameraChangeLockonModeData* camera_data = static_cast<MessageData::CameraChangeLockonModeData*>(data);
	if (mode != Mode::LockonCamera)
		side_value = CalcSide(camera_data->start, camera_data->target);

	mode = Mode::LockonCamera;
	target_work[0] = camera_data->start;
	target_work[1] = camera_data->target;
	target_work[0].y += 50.0f;
	target_work[1].y += 0.01f;
}

//-----------------------------
// ���[�V�����J����
//-----------------------------
void CameraController::OnMotionMode(void* data)
{
	MessageData::CameraChangeMotionModeData* camera_data = static_cast<MessageData::CameraChangeMotionModeData*>(data);
	if (mode != Mode::MotionCamera)
	{
		motion_timer = 0.0f;
	}		
	mode = Mode::MotionCamera;
	motion_data.clear();
	motion_data = camera_data->data;
}

//-----------------------------
// �t���[�J�����X�V����
//-----------------------------
void CameraController::UpdateFreeCamera(float elapsed_time)
{
	GamePad& gamepad = Input::Instance().GetGamePad();
	float ax = gamepad.GetAxisRX();
	float ay = gamepad.GetAxisRY();

	// �J�����̉�]���x
	float speed = roll_speed * elapsed_time;

	// �X�e�B�b�N�̓��͒l�ɉ�����X����Y��
	angle.x += ay * speed;
	angle.y += ax * speed;

	// X���̃J�����̉�]�𐧌�
	if (angle.x > max_angleX)
	{
		angle.x = max_angleX;
	}
	if (angle.x < min_angleX)
	{
		angle.x = min_angleX;
	}
	// Y���̉�]�l��-3.14�`3.14�Ɏ��܂�悤�ɂ���
	if (angle.y > -DirectX::XM_PI)
	{
		angle.y -= DirectX::XM_2PI;
	}
	if (angle.y < DirectX::XM_PI)
	{
		angle.y += DirectX::XM_2PI;
	}

	// �J������]�l����]�s��ɕϊ�
	DirectX::XMMATRIX transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

	// ��]�s�񂩂�O�����x�N�g������肾��
	DirectX::XMVECTOR vec_front = transform.r[2];
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, vec_front);

	// �����_������x�N�g�������Ɉ�苗���𗣂ꂽ�J�������_
	DirectX::XMFLOAT3 eye;
	new_position.x = target.x - front.x * range;
	new_position.y = target.y - front.y * range;
	new_position.z = target.z - front.z * range;
}

//-----------------------------
// ���b�N�I���J�����X�V����
//-----------------------------
void CameraController::UpdateLockonCamera(float elapsed_time)
{
	// ������Ɉړ�������
	DirectX::XMVECTOR target1 = DirectX::XMVectorSet(target_work[0].x, 0.5f, target_work[0].z, 0.0f);
	DirectX::XMVECTOR target2 = DirectX::XMVectorSet(target_work[1].x, 0.5f, target_work[1].z, 0.0f);
	DirectX::XMVECTOR camera_right_view = DirectX::XMLoadFloat3(&Camera::Instance().GetRight());
	DirectX::XMVECTOR camera_up_view = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR vector = DirectX::XMVectorSubtract(target2, target1);
	DirectX::XMVECTOR length = DirectX::XMVector3Length(vector);
	
	target1 = DirectX::XMLoadFloat3(&target_work[0]);
	target2 = DirectX::XMLoadFloat3(&target_work[1]);

	// �V���������_���Z�o
	DirectX::XMStoreFloat3(&new_target,
		DirectX::XMVectorMultiplyAdd(vector, DirectX::XMVectorReplicate(0.5f), target1));

	// �V�������W���Z�o
	length = DirectX::XMVectorClamp(length,
		DirectX::XMVectorReplicate(length_limit[0]),
		DirectX::XMVectorReplicate(length_limit[1]));
	target1 = DirectX::XMVectorMultiplyAdd(length, DirectX::XMVector3Normalize(DirectX::XMVectorNegate(vector)), target1);
	target1 = DirectX::XMVectorMultiplyAdd(camera_right_view, DirectX::XMVectorReplicate(side_value * 3.0f), target1);
	target1 = DirectX::XMVectorMultiplyAdd(camera_up_view, DirectX::XMVectorReplicate(3.0f), target1);
	DirectX::XMStoreFloat3(&new_position, target1);
}

//-----------------------------
// ���[�V�����J�����X�V����
//-----------------------------
void CameraController::UpdateMotionCamera(float elapsed_time)
{
	if (motion_data.empty())
	{
		return;
	}

	motion_timer += elapsed_time * 60.0f;
	if (motion_data.size() == 1)
	{
		if (motion_timer >= motion_data[0].time)
		{
			new_position = motion_data[0].position;
			new_target = motion_data[0].target;
			position = new_position;
			target = new_target;
		}
	}
	else
	{
		bool set = false;
		for (int i = 0; i < motion_data.size() - 1; ++i)
		{
			if (motion_data[i].time <= motion_timer && motion_timer < motion_data[i + 1].time)
			{
				set = true;
				float	value = motion_data[i + 1].time - motion_data[i].time;
				value = (motion_timer - motion_data[i].time) / value;
				new_position = motion_data[i].position;
				new_position.x += (motion_data[i + 1].position.x - motion_data[i].position.x) * value;
				new_position.y += (motion_data[i + 1].position.y - motion_data[i].position.y) * value;
				new_position.z += (motion_data[i + 1].position.z - motion_data[i].position.z) * value;
				new_target = motion_data[i].target;
				new_target.x += (motion_data[i + 1].target.x - motion_data[i].target.x) * value;
				new_target.y += (motion_data[i + 1].target.y - motion_data[i].target.y) * value;
				new_target.z += (motion_data[i + 1].target.z - motion_data[i].target.z) * value;
				position = new_position;
				target = new_target;
				break;
			}
		}
		if (!set)
		{
			if (motion_timer >= motion_data[motion_data.size() - 1].time)
			{
				new_position = motion_data[motion_data.size() - 1].position;
				new_target = motion_data[motion_data.size() - 1].target;
				position = new_position;
				target = new_target;
			}
		}

	}
}

//-----------------------------
// �����̃Y�������Z�o
//-----------------------------
float CameraController::CalcSide(DirectX::XMFLOAT3 position1, DirectX::XMFLOAT3 position2)
{
	// �O�ς�p���ĉ����̃Y�������Z�o
	DirectX::XMFLOAT2 vector;
	vector.x = position.x - target.x;
	vector.y = position.z - target.z;
	float length = sqrtf(vector.x * vector.x + vector.y * vector.y);
	vector.x /= length;
	vector.y /= length;
	// TODO
	DirectX::XMFLOAT2	n;
	n.x = position1.x - position2.x;
	n.y = position1.z - position2.z;
	length = sqrtf(n.x * n.x + n.y * n.y);
	n.x /= length;
	n.y /= length;
	return	((vector.x * n.y) - (vector.y * n.x) < 0) ? +1.0f : -1.0f;
}
