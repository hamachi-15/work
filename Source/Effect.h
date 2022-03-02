#pragma once
#include <DirectXMath.h>
#include <Effekseer.h>


//-------------------------------
// �G�t�F�N�g
//--------------------------------
class Effect
{
public:
	// �R���X�g���N�^
	Effect(const char* filename, Effekseer::Manager* effect_manager);

	// �f�X�g���N�^
	~Effect();

	// �Đ�����
	Effekseer::Handle Play(Effekseer::Manager* effect_manager, const DirectX::XMFLOAT3& position, float scalse = 1.0f);

	// ��~����
	void Stop(Effekseer::Manager* effect_manager, Effekseer::Handle handle);

	// ���W�ݒ�
	void SetPosition(Effekseer::Manager* effect_manager, Effekseer::Handle handle, const DirectX::XMFLOAT3& position);

	// �X�P�[���ݒ�
	void SetScale(Effekseer::Manager* effect_manager, Effekseer::Handle handle, const DirectX::XMFLOAT3& scale);

	// ���W���Z
	void AddPosition(Effekseer::Manager* effect_manager, Effekseer::Handle handle, const DirectX::XMFLOAT3& add);

	// ���W�擾
	DirectX::XMFLOAT3 GetPosition(Effekseer::Manager* effect_manager, Effekseer::Handle handle) const;
private:
	Effekseer::Effect* effekseer_effect = nullptr;
};