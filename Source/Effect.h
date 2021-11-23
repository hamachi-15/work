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
	Effect(const char* filename);
	// �f�X�g���N�^
	~Effect();

	// �Đ�����
	Effekseer::Handle Play(const DirectX::XMFLOAT3& position, float scalse = 1.0f);

	// ��~����
	void Stop(Effekseer::Handle handle);

	// ���W�ݒ�
	void SetPosition(Effekseer::Handle handle, const DirectX::XMFLOAT3& position);

	// �X�P�[���ݒ�
	void SetScale(Effekseer::Handle handle, const DirectX::XMFLOAT3& scale);

private:
	Effekseer::Effect* effekseer_effect = nullptr;
};