#include "Graphics.h"
#include "Effect.h"
#include "EffectManager.h"

//---------------------------------
// �R���X�g���N�^
//---------------------------------
Effect::Effect(const char* filename, Effekseer::Manager* effekseer_manager)
{
	// �~���[�e�N�X
	std::lock_guard<std::mutex> lock(Graphics::Instance().GetMutex());

	// Effekseer�̃��\�[�X��ǂݍ���
	// Effekseer��UTF-16�̃t�@�C���p�X�ȊO�͑Ή����Ă��Ȃ��ו����R�[�h�ϊ����K�v
	char16_t utf16_filemane[256];
	Effekseer::ConvertUtf8ToUtf16(utf16_filemane, 256, filename);

	// Effekseer�G�t�F�N�g��ǂݍ���
	effekseer_effect = Effekseer::Effect::Create(effekseer_manager, (EFK_CHAR*)utf16_filemane);
}

//---------------------------------
// �f�X�g���N�^
//---------------------------------
Effect::~Effect()
{
	// �G�t�F�N�V�A�[�̔j������
	if (effekseer_effect != nullptr)
	{
		effekseer_effect->Release();
		effekseer_effect = nullptr;
	}
}

//------------------------------------
// �Đ�����
//------------------------------------
Effekseer::Handle Effect::Play(Effekseer::Manager* effekseer_manager, const DirectX::XMFLOAT3& position, float scale)
{
	// �G�t�F�N�g�Đ�
	Effekseer::Vector3D vector = {position.x, position.y, position.z};
	Effekseer::Handle handle = effekseer_manager->Play(effekseer_effect, vector);
	effekseer_manager->SetScale(handle, scale, scale, scale);

	return handle;
}

//---------------------------------------
// ��~����
//---------------------------------------
void Effect::Stop(Effekseer::Manager* effekseer_manager, Effekseer::Handle handle)
{
	// �G�t�F�N�g�Đ���~
	effekseer_manager->StopEffect(handle);
}

//-----------------------------------------
// ���W�ݒ�
//-----------------------------------------
void Effect::SetPosition(Effekseer::Manager* effekseer_manager, Effekseer::Handle handle, const DirectX::XMFLOAT3& position)
{
	effekseer_manager->SetLocation(handle, position.x, position.y, position.z);
}

//----------------------------------------
// �X�P�[���ݒ�
//-----------------------------------------
void Effect::SetScale(Effekseer::Manager* effekseer_manager, Effekseer::Handle handle, const DirectX::XMFLOAT3& scale)
{
	effekseer_manager->SetScale(handle, scale.x, scale.y, scale.z);
}

//-----------------------------------------
// ���W���Z
//-----------------------------------------
void Effect::AddPosition(Effekseer::Manager* effekseer_manager, Effekseer::Handle handle, const DirectX::XMFLOAT3& add)
{
	Effekseer::Vector3D vec(add.x, add.y, add.z);
	effekseer_manager->AddLocation(handle, vec);
}

//-----------------------------------------
// ���W�擾
//-----------------------------------------
DirectX::XMFLOAT3 Effect::GetPosition(Effekseer::Manager* effekseer_manager, Effekseer::Handle handle) const
{
	Effekseer::Vector3D vector = effekseer_manager->GetLocation(handle);
	
	return DirectX::XMFLOAT3(vector.X, vector.Y, vector.Z);
}
