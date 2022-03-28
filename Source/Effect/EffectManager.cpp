#include "Graphics.h"
#include "EffectManager.h"

//------------------------------------
// ����������
//------------------------------------
void EffectManager::Initialize()
{
	Graphics& graphics = Graphics::Instance();

	// Effekseer�����_���[�쐬
	effekseer_renderer = EffekseerRendererDX11::Renderer::Create(
		graphics.GetDevice(), graphics.GetDeviceContext(), 2048);

	// Effekseer�}�l�[�W���[����
	effekseer_manager = Effekseer::Manager::Create(2048);

	// Effekseer�����_���̊e��ݒ�(���ʂȃJ�X�^�}�C�Y�����Ȃ��ꍇ�͂��̐ݒ��OK)
	effekseer_manager->SetSpriteRenderer(effekseer_renderer->CreateSpriteRenderer());
	effekseer_manager->SetRibbonRenderer(effekseer_renderer->CreateRibbonRenderer());
	effekseer_manager->SetRingRenderer(effekseer_renderer->CreateRingRenderer());
	effekseer_manager->SetTrackRenderer(effekseer_renderer->CreateTrackRenderer());
	effekseer_manager->SetModelRenderer(effekseer_renderer->CreateModelRenderer());

	// Effekseer���ł̃��[�_�[�ݒ�(���ʂȃJ�X�^�}�C�Y�����Ȃ��ꍇ�͂��̐ݒ��OK)
	effekseer_manager->SetTextureLoader(effekseer_renderer->CreateTextureLoader());
	effekseer_manager->SetModelLoader(effekseer_renderer->CreateModelLoader());
	effekseer_manager->SetMaterialLoader(effekseer_renderer->CreateMaterialLoader());

	// Effekseer��������W�n�Ōv�Z����
	effekseer_manager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
}

//-----------------------------------
// �I��������
//-----------------------------------
void EffectManager::Finalize()
{
	if (effekseer_manager != nullptr)
	{
		effekseer_manager->Destroy();
		effekseer_manager = nullptr;
	}
	if (effekseer_renderer != nullptr)
	{
		effekseer_renderer->Destroy();
		effekseer_renderer = nullptr;
	}
}

//-----------------------------------
// �X�V����
//------------------------------------
void EffectManager::Update(float elapsed_time)
{
	// �G�t�F�N�g�̍X�V����(�����̓t���[���̌o�ߎ��Ԃ�n��)
	effekseer_manager->Update(elapsed_time * 60.0f);
}

//-------------------------------------
// �`�揈��
//-------------------------------------
void EffectManager::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
	// �r���[���v���W�F�N�V�����s���Effekseer�����_���ɐݒ�
	effekseer_renderer->SetCameraMatrix(*reinterpret_cast<const Effekseer::Matrix44*>(&view));
	effekseer_renderer->SetProjectionMatrix(*reinterpret_cast<const Effekseer::Matrix44*>(&projection));

	// Effekseer�`��J�n
	effekseer_renderer->BeginRendering();

	// Effekseer�`����s
	// �}�l�[�W���[�P�ʂŕ`�悷��̂ŕ`�揇�𐧌䂷��ꍇ�̓}�l�[�W���[�𕡐��쐬��
	// Draw()�֐������s���鏇���Ő���
	effekseer_manager->Draw();

	// Effekseer�`��I��
	effekseer_renderer->EndRendering();
}

