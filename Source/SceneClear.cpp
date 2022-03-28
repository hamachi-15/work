#include "SceneClear.h"
#include "EnemyManager.h"
#include "ResourceManager.h"
#include "ShaderManager.h"
#include "SceneManager.h"
#include "AudioManager.h"

#include "SceneTitle.h"
#include "Graphics.h"
#include "Sprite.h"

#include "GameDatabase.h"

#include "Input.h"
//-------------------------------------
// ����������
//-------------------------------------
void SceneClear::Initialize()
{
	// �e�N�X�`���ǂݍ���
	sprite = std::make_unique<Sprite>();
	anybutton_texture = ResourceManager::Instance().LoadTexture("Data/Sprite/PushAnyButton.png");
	clear_texture = std::make_unique<Texture>();
	clear_texture->Load("Data/Sprite/GameClear.png");
	// BGM�Đ�
	AudioManager::Instance().PlayBGM(BGMType::Clear);
	AudioManager::Instance().SetBGMVolume(BGMType::Clear, 0.4f);
}

//-------------------------------------
// �I��������
//-------------------------------------
void SceneClear::Finalize()
{
	// ���j�t���O���N���A
	EnemyManager::Instance().CrearDefeat();

	// BGM�Đ���~
	AudioManager::Instance().StopBGM(BGMType::Clear);
}

//-------------------------------------
// �X�V����
//-------------------------------------
void SceneClear::Update(float elapsed_time)
{
	GamePad& gamepad = Input::Instance().GetGamePad();
	const GamePadButton any_button =
		GamePad::BTN_A
		| GamePad::BTN_B
		| GamePad::BTN_X
		| GamePad::BTN_Y;
	// A�AB�AX�AY�{�^���������ꂽ�Ƃ��Ƀ^�C�g����ʂɑJ��
	if (gamepad.GetButtonDown() & any_button)
	{
		// �^�C�g���ɑJ��
		SceneManager::Instance().ChangeScene(new SceneTitle());
	}
}

//-------------------------------------
// �`�揈��
//-------------------------------------
void SceneClear::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* context = graphics.GetDeviceContext();
	// �X�N���[���T�C�Y�擾
	DirectX::XMFLOAT2 screen_size = { graphics.GetScreenWidth(), graphics.GetScreenHeight() };

	// �o�b�N�o�b�t�@�̃N���A����
	{
		ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
		ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();
		// ��ʃN���A
		graphics.ScreenClear(&render_target_view, depth_stencil_view);
	}

	// �X�N���[���e�N�X�`���ɕ`��
	ScreenRender(context, nullptr, screen_size);

	// �o�b�N�o�b�t�@�ɕ`��
	BuckBufferRender(context, nullptr, screen_size);
}

//-------------------------------------
// �X�N���[���e�N�X�`���`��
//-------------------------------------
void SceneClear::ScreenRender(ID3D11DeviceContext* context, RenderContext* render_context, const DirectX::XMFLOAT2& screen_size)
{
	Graphics& graphics = Graphics::Instance();

	ShaderManager& shader_manager = ShaderManager::Instance();
	std::shared_ptr<Shader> sprite_shader = shader_manager.GetShader(ShaderManager::ShaderType::Sprite);

	// �X�N���[���e�N�X�`���������_�[�^�[�Q�b�g�ɐݒ肵�ĉ�ʃN���A
	ID3D11RenderTargetView* screen_texture = graphics.GetTexture()->GetRenderTargetView();
	ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();
	{
		graphics.SetRenderTargetView(&screen_texture, depth_stencil_view);
		graphics.ScreenClear(&screen_texture, depth_stencil_view);
	}
	// �r���[�|�[�g�����ɖ߂�
	graphics.SetViewport(screen_size.x, screen_size.y);

	Texture* texture = GameDataBase::Instance().GetTimingTexture();
	sprite_shader->Begin(context);
	sprite->Render(context,
		texture,
		0.0f, 0.0f,
		screen_size.x, screen_size.y,
		0, 0,
		(float)texture->GetWidth(), (float)texture->GetHeight());
	sprite->Render(context,
		clear_texture.get(),
		400, 100,
		(float)clear_texture->GetWidth(), (float)clear_texture->GetHeight(),
		0, 0,
		(float)clear_texture->GetWidth(), (float)clear_texture->GetHeight());
	sprite->Render(context,
		anybutton_texture.get(),
		400, 400,
		(float)anybutton_texture->GetWidth(), (float)anybutton_texture->GetHeight(),
		0, 0,
		(float)anybutton_texture->GetWidth(), (float)anybutton_texture->GetHeight());
	sprite_shader->End(context);
}

//-------------------------------------
//�o�b�N�o�b�t�@�`��
//-------------------------------------
void SceneClear::BuckBufferRender(ID3D11DeviceContext* context, RenderContext* render_context, const DirectX::XMFLOAT2& screen_size)
{
	Graphics& graphics = Graphics::Instance();
	ShaderManager& shader_manager = ShaderManager::Instance();
	// �X�v���C�g�V�F�[�_�[�擾
	std::shared_ptr<Shader> dissolve_shader = shader_manager.GetShader(ShaderManager::ShaderType::Dissolve);
	// �����_�[�^�[�Q�b�g�ݒ�
	{
		ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
		ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();
		graphics.SetRenderTargetView(&render_target_view, depth_stencil_view);
		graphics.ScreenClear(&render_target_view, depth_stencil_view, { 0.0f, 0.0f, 0.5f, 1.0f });
	}
	// �r���[�|�[�g�ݒ�
	graphics.SetViewport(screen_size.x, screen_size.y);

	std::shared_ptr<Shader> sprite_shader = shader_manager.GetShader(ShaderManager::ShaderType::Sprite);
	// �o�b�N�o�b�t�@�ɃX�N���[���e�N�X�`����`��
	sprite_shader->Begin(context);
	// �X�N���[���e�N�X�`��
	sprite->Render(context, graphics.GetTexture(),
		0, 0,
		screen_size.x, screen_size.y,
		0, 0,
		(float)graphics.GetTexture()->GetWidth(), (float)graphics.GetTexture()->GetHeight(),
		0,
		1, 1, 1, 1);
	sprite_shader->End(context);
}
