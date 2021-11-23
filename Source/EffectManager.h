#pragma once
#include <DirectXMath.h>
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>

//------------------------------------
// �G�t�F�N�g�}�l�[�W���[
//------------------------------------
class EffectManager
{
private:
	// �R���X�g���N�^
	EffectManager() {}
	// �f�X�g���N�^
	~EffectManager() {}
public:
	// �C���X�^���X�擾
	static EffectManager& Instance()
	{
		static EffectManager instance;
		return instance;
	}

	// ����������
	void Initialize();

	// �I��������
	void Finalize();

	// �X�V����
	void Update(float elapsed_time);

	// �`�揈��
	void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

	// Effekseer�}�l�[�W���[�̎擾
	Effekseer::Manager* GetEffekseerManager() { return effekseer_manager; }

private:
	Effekseer::Manager*				effekseer_manager = nullptr;
	EffekseerRenderer::Renderer*	effekseer_renderer = nullptr;
};