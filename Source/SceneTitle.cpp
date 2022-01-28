#include "SceneTitle.h"
#include "Graphics.h"
#include "Camera.h"
#include "Light.h"
#include "Input.h"
#include "Actor.h"
#include "Charactor.h"
#include "Player.h"
#include "Movement.h"
#include "LambertShader.h"
#include "DissolveShader.h"
#include "ShaderManager.h"

#include "ActorManager.h"
#include "SceneManager.h"
#include "SceneLoading.h"
#include "SceneGame.h"
#include "Messenger.h"
#include "SceneBattle.h"

#include "TitleButtonUI.h"
#include "UIManager.h"

//----------------------------------
// ����������
//----------------------------------
void SceneTitle::Initialize()
{
	Graphics& graphics = Graphics::Instance();

	// �V�[�����ݒ�
	SetName("SceneTitle");

	// ���C�g������
	Light::Initialize();

	// �J���������ݒ�
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 18, -20),
		DirectX::XMFLOAT3(0, 15, 0),
		DirectX::XMFLOAT3(0, 1, 0)
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		1000.0f);

	// �e�N�X�`���Ǎ���
	sprite = std::make_unique<Sprite>();
	mask_texture = std::make_unique<Texture>();
	mask_texture->Load("Data/Sprite/dissolve_animation2.png");

	sky = std::make_unique<Texture>();
	sky->Load("Data/Sprite/SkyBox/FS002_Night.png");

	UIManager::Instance().RegisterUI(std::make_shared<TitleUI>());
}

//----------------------------------
// �I������
//----------------------------------
void SceneTitle::Finalize()
{
	// UI�̑S�j��
	UIManager::Instance().AllDelete();

	// ���b�Z���W���[�̃N���A
	Messenger::Instance().Clear();
}

//----------------------------------
// �X�V����
//----------------------------------
void SceneTitle::Update(float elapsed_time)
{
	// �V�[���J�ڃt���O����������
	if (IsSceneChangeFlag())
	{
		timer += elapsed_time;
		if (timer >= 1.2f)
		{
			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame()));
		}
	}

	// UI�X�V����
	UIManager::Instance().Update(elapsed_time);
}

//----------------------------------
// �X�N���[���e�N�X�`���`��
//----------------------------------
void SceneTitle::ScreenRender(ID3D11DeviceContext* context, RenderContext& render_context, const DirectX::XMFLOAT2& screen_size)
{
	Graphics& graphics = Graphics::Instance();
	ShaderManager& shader_manager = ShaderManager::Instance();
	
	// �X�N���[���e�N�X�`���������_�[�^�[�Q�b�g�ɐݒ肵�ĉ�ʃN���A
	ID3D11RenderTargetView* screen_texture = graphics.GetTexture()->GetRenderTargetView();
	ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();
	{
		graphics.SetRenderTargetView(&screen_texture, depth_stencil_view);
		graphics.ScreenClear(&screen_texture, depth_stencil_view);
	}
	// �r���[�|�[�g�����ɖ߂�
	graphics.SetViewport(screen_size.x, screen_size.y);

	std::shared_ptr<Shader> sprite_shader = shader_manager.GetShader(ShaderManager::ShaderType::Sprite);
	
	// �X�J�C�{�b�N�X�`��
	{
		std::shared_ptr<Shader> skybox_shader = shader_manager.GetShader(ShaderManager::ShaderType::SkyBox);
		skybox_shader->Begin(context, render_context);
		sprite->Render(context,
			sky.get(),
			0, 0,
			screen_size.x, screen_size.y,
			0, 0,
			static_cast<float>(sky->GetWidth()), static_cast<float>(sky->GetHeight()));
		skybox_shader->End(context);
	}

	// �^�C�g����ʂ�UI�`��
	UIManager::Instance().Draw(context);

}

//----------------------------------
// �o�b�N�o�b�t�@�`��
//----------------------------------
void SceneTitle::BuckBufferRender(ID3D11DeviceContext* context, RenderContext& render_context, const DirectX::XMFLOAT2& screen_size)
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

	// �o�b�N�o�b�t�@�ɃX�N���[���e�N�X�`����`��
	dissolve_shader->Begin(context, timer);
	dynamic_cast<DissolveShader*>(dissolve_shader.get())->Render(context, graphics.GetTexture(), mask_texture.get());
	dissolve_shader->End(context);
}

//----------------------------------
// �`�揈��
//----------------------------------
void SceneTitle::Render()
{
	ShaderManager& shader_manager = ShaderManager::Instance();
	std::shared_ptr<Shader> sprite_shader = shader_manager.GetShader(ShaderManager::ShaderType::Sprite);
	
	render_context.light_color = Light::DirLightColor;
	render_context.ambient_color = Light::Ambient;
	render_context.light_direction = Light::LightDir;

	// �J�����p�����[�^�ݒ�
	Camera& camera = Camera::Instance();
	render_context.view = camera.GetView();
	render_context.projection = camera.GetProjection();

	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* context = graphics.GetDeviceContext();
	ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();

	// ��ʃN���A
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };
	context->ClearRenderTargetView(render_target_view, color);
	context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);

	float screen_width = graphics.GetScreenWidth();
	float screen_height = graphics.GetScreenHeight();

	ScreenRender(context, render_context, { screen_width, screen_height });

	BuckBufferRender(context, render_context, { screen_width, screen_height });
	
	//OnGui();
}

//----------------------------
// �{�^���`��
//----------------------------
void SceneTitle::RenderButton(ID3D11DeviceContext* context, Texture* texture, int index)
{

	Graphics& graphics = Graphics::Instance();
	float screen_width = graphics.GetScreenWidth();
	float screen_height = graphics.GetScreenHeight();
	sprite->Render(context,
		texture,
		screen_width * 0.25f, screen_height - (screen_height * 0.25f),
		250, 100,
		0, 0,
		(float)texture->GetWidth(), (float)texture->GetHeight(),
		0,
		1, 1, 1, 1);
}

//----------------------------
// GUI�`��
//----------------------------
void SceneTitle::OnGui()
{
	ImGui::SetNextWindowPos(ImVec2(550, 50), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	ImGui::Begin("Shader", nullptr, ImGuiWindowFlags_None);
	ImGui::SliderFloat("Timer", &timer, 0.0f, 1.5f);

	ImGui::End();
}
