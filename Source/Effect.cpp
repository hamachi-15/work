#include "Graphics.h"
#include "Effect.h"
#include "EffectManager.h"

//---------------------------------
// �R���X�g���N�^
//---------------------------------
Effect::Effect(const char* filename)
{
	// �~���[�e�N�X
	std::lock_guard<std::mutex> lock(Graphics::Instance().GetMutex());

	// Effekseer�̃��\�[�X��ǂݍ���
	// Effekseer��UTF-16�̃t�@�C���p�X�ȊO�͑Ή����Ă��Ȃ��ו����R�[�h�ϊ����K�v
	char16_t utf16_filemane[256];
	Effekseer::ConvertUtf8ToUtf16(utf16_filemane, 256, filename);

	// Effekseer::Manager���擾
	Effekseer::Manager* effekseer_manager = EffectManager::Instance().GetEffekseerManager();

	// Effekseer�G�t�F�N�g��ǂݍ���
	effekseer_effect = Effekseer::Effect::Create(effekseer_manager, (EFK_CHAR*)utf16_filemane);
}

//---------------------------------
// �f�X�g���N�^
//---------------------------------
Effect::~Effect()
{
	if (effekseer_effect != nullptr)
	{
		effekseer_effect->Release();
		effekseer_effect = nullptr;
	}
}

//------------------------------------
// �Đ�����
//------------------------------------
Effekseer::Handle Effect::Play(const DirectX::XMFLOAT3& position, float scale)
{
	// Effekseer::Manager���擾
	Effekseer::Manager* effekseer_manager = EffectManager::Instance().GetEffekseerManager();

	// �G�t�F�N�g�Đ�
	Effekseer::Handle handle = effekseer_manager->Play(effekseer_effect, position.x, position.y, position.z);
	effekseer_manager->SetScale(handle, scale, scale, scale);

	return handle;
}

//---------------------------------------
// ��~����
//---------------------------------------
void Effect::Stop(Effekseer::Handle handle)
{
	// Effekseer::Manager���擾
	Effekseer::Manager* effekseer_manager = EffectManager::Instance().GetEffekseerManager();

	// �G�t�F�N�g�Đ���~
	effekseer_manager->StopEffect(handle);
}

//-----------------------------------------
// ���W�ݒ�
//-----------------------------------------
void Effect::SetPosition(Effekseer::Handle handle, const DirectX::XMFLOAT3& position)
{
	// Effekseer::Manager���擾
	Effekseer::Manager* effekseer_manager = EffectManager::Instance().GetEffekseerManager();

	effekseer_manager->SetLocation(handle, position.x, position.y, position.z);
}

//----------------------------------------
// �X�P�[���ݒ�
//-----------------------------------------
void Effect::SetScale(Effekseer::Handle handle, const DirectX::XMFLOAT3& scale)
{
	// Effekseer::Manager���擾
	Effekseer::Manager* effekseer_manager = EffectManager::Instance().GetEffekseerManager();

	effekseer_manager->SetScale(handle, scale.x, scale.y, scale.z);
}