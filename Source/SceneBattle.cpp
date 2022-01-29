#include <imgui.h>
#include <stdio.h>
#include "Mathf.h"
#include "SceneBattle.h"
#include "Graphics.h"
#include "Camera.h"
#include "CameraController.h"
#include "Template.h"
#include "Light.h"
#include "Input.h"
#include "Movement.h"

#include "2DPrimitive.h"
#include "CascadeShadowMapShader.h"
#include "LambertShader.h"
#include "BloomShader.h"
#include "GaussianBlurShader.h"

#include "Actor.h"
#include "ActorManager.h"
#include "EnemyManager.h"
#include "EnemySlime.h"
#include "Player.h"
#include "Stage.h"
#include "EnemyTerritoryManager.h"

#include "MenuSystem.h"
#include "UIManager.h"

#include "Messenger.h"
#include "MessageData.h"
#include "GameDataBase.h"

#include "CollisionManager.h"
#include "ShaderManager.h"

#include "Texture.h"
#include "Sprite.h"
#include "MetaAI.h"

#include "PlayerUIHealth.h"
//#include "SceneManager.h"
#include "SceneTitle.h"

SceneBattle::SceneBattle()
{
}

SceneBattle::~SceneBattle()
{
}

void SceneBattle::Initialize()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11Device* device = graphics.GetDevice();

	// �V�[�����ݒ�
	SetName("SceneBattle");

	// �v���~�e�B�u�̃R���X�^���g�o�b�t�@�̏����ݒ�
	primitive_falg = true;
	primitive_context.number = 2;
	primitive_context.timer = 0.0f;

	// ���C�g������
	Light::Initialize();

	// �J���������ݒ�
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(-400, 16, -416),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0));
	camera.SetPerspectiveFov(
		Mathf::ConvartToRadian(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		800.0f);

	// �J�����R���g���[���[������
	camera_controller = std::make_unique<CameraController>();
	camera_controller->SetCameraAngle({ Mathf::ConvartToRadian(25), 0.0f, 0.0f });

	// �V�F�[�_�[������
	bloom = std::make_unique<Bloom>(device);
	primitive = std::make_unique<Primitive>(device);

	// �e�N�X�`���̓ǂݍ���
	sprite = std::make_unique<Sprite>();
	sky = std::make_unique<Texture>();
	sky->Load("Data/Sprite/SkyBox/FS002_Night.png");
	anybutton_texture = std::make_unique<Texture>();
	anybutton_texture->Load("Data/Sprite/PushAnyButton.png");
	clear_texture = std::make_unique<Texture>();
	clear_texture->Load("Data/Sprite/GameClear.png");
	over_texture = std::make_unique<Texture>();
	over_texture->Load("Data/Sprite/YouDead.png");
	// �X�e�[�W�ǂݍ���
	{
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetUpModel("Data/Model/Filde/Filde.mdl", nullptr);
		actor->SetName("Filde");
		actor->SetPosition(DirectX::XMFLOAT3(0, 0, 0));
		actor->SetAngle(DirectX::XMFLOAT3(0, Mathf::ConvartToRadian(-90), 0));
		actor->SetScale(DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f));
		actor->AddComponent<Stage>();
		actor->SetShaderType(ShaderManager::ShaderType::CascadeShadowMap);
	}

	// �v���C���[�ǂݍ���
	{
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetUpModel("Data/Model/RPG-Character/Swordman.mdl", "Motion");
		actor->SetName("Player");
		actor->SetPosition(DirectX::XMFLOAT3(-278.0f, 5.5f, 34.4f));
		actor->SetAngle(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
		actor->SetScale(DirectX::XMFLOAT3(0.04f, 0.04f, 0.04f));
		actor->AddComponent<Movement>();
		actor->AddComponent<Charactor>();
		std::shared_ptr<PlayerHealthUI> ac = actor->AddComponent<PlayerHealthUI>();
		actor->AddComponent<Player>();
		UIManager::Instance().RegisterUI(ac);
		actor->SetShaderType(ShaderManager::ShaderType::Lambert);
	}
	ActorManager::Instance().Update(0.01f);
	ActorManager::Instance().UpdateTransform();
	
	// �X�N���v�g����G�̐���
	EnemyManager::Instance().CreateEnemyEncountData();
}

void SceneBattle::Finalize()
{
	// UI�̑S�j��
	UIManager::Instance().AllDelete();
	
	// �G�}�l�[�W���[�̃N���A
	EnemyManager::Instance().AllRemove();

	// �A�N�^�[�̔j��
	ActorManager::Instance().AllDestroy();

	// �R���W�����̔j��
	CollisionManager::Instance().Destroy();

	// ���b�Z���W���[�̃N���A
	Messenger::Instance().Clear();
}

void SceneBattle::Update(float elapsed_time)
{
	//	���j���[�I�[�v�����̓��j���[���X�V����
	MenuSystem::Instance().Update(elapsed_time);

	//	���j���[�I�[�v�����ł���΃Q�[�����̍X�V���s��Ȃ�
	if (MenuSystem::Instance().IsOpened())
	{
		return;
	}
	else
	{
		//	���j���[�I�[�v��
		GamePad& gamePad = Input::Instance().GetGamePad();
		if (gamePad.GetButtonDown() & GamePad::BTN_START)
		{
			MessageData::MenuOpenData data;
			data.menu_id = MenuId::TopMenu;
			Messenger::Instance().SendData(MessageData::MENUOPENEVENT, &data);
		}
	}
	// �Q�[�����N���A���Q�[���I�[�o�[�ɂȂ��Ă����
	if (isgame_clear || isgame_over)
	{
		GamePad& gamePad = Input::Instance().GetGamePad();
		const GamePadButton any_button =
			GamePad::BTN_A
			| GamePad::BTN_B
			| GamePad::BTN_X
			| GamePad::BTN_Y;
		if (gamePad.GetButtonDown() & any_button)
		{
			SceneManager::Instance().ChangeScene(new SceneTitle());
		}
		return;
	}

	// �v���~�e�B�u�R���e�L�X�g�̃R���X�g���N�^�X�V
	if (primitive_context.timer < 40)
	{
		primitive_context.timer++;
	}
	if (primitive_context.timer >= 40)
	{
		primitive_falg = false;
	}

	//���C�g
	static float light_angle = DirectX::XM_PI;
	if (GetKeyState('E') < 0) light_angle += elapsed_time * 2.0f;
	if (GetKeyState('Q') < 0) light_angle -= elapsed_time * 2.0f;

	//���C�g����
	LightDir.x = sinf(light_angle);
	LightDir.y = -1.0f;
	LightDir.z = cosf(light_angle);
	Light::SetDirLight(LightDir, DirectX::XMFLOAT3(0.6f, 0.6f, 0.6f));
	Light::SetAmbient(DirectX::XMFLOAT3(0.2f, 0.2f, 0.2f));

	// �A�N�^�[�X�V����
	ActorManager::Instance().Update(elapsed_time);
	ActorManager::Instance().UpdateTransform();

	// �����蔻��X�V����
	CollisionManager::Instance().Update();

	// UI�X�V����
	UIManager::Instance().Update(elapsed_time);

	// �J�����X�V����
	std::shared_ptr<Actor> actor = ActorManager::Instance().GetActor("Player");
	camera_controller->SetTarget({
		actor->GetPosition().x,
		actor->GetPosition().y + 1.0f,
		actor->GetPosition().z });
	camera_controller->Update(elapsed_time);
}

void SceneBattle::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* context = graphics.GetDeviceContext();

	// �X�N���[���T�C�Y�擾
	DirectX::XMFLOAT2 screen_size = { graphics.GetScreenWidth(), graphics.GetScreenHeight() };

	// �`�揈��
	render_context.light_direction = Light::LightDir;
	render_context.ShadowParameter = { shadow_color.x, shadow_color.y, shadow_color.z, 0.001f };

	// �J�����p�����[�^�ݒ�
	Camera& camera = Camera::Instance();
	render_context.view = camera.GetView();
	render_context.projection = camera.GetProjection();

	// �o�b�N�o�b�t�@�̃N���A����
	{
		ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
		ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();
		// ��ʃN���A
		graphics.ScreenClear(&render_target_view, depth_stencil_view);
	}

	// �X�N���[���e�N�X�`���ɕ`��
	ScreenRender(context, render_context, screen_size);

	// �|�X�g�e�N�X�`���`��
	PostRender(context, render_context, screen_size);

	// �o�b�N�o�b�t�@�ɕ`��
	BuckBufferRender(context, render_context, screen_size);
}


//-------------------------------------
// �X�N���[���e�N�X�`���`��
//-------------------------------------
void SceneBattle::ScreenRender(ID3D11DeviceContext* context, RenderContext& render_context, const DirectX::XMFLOAT2& screen_size)
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

	// �X�J�C�{�b�N�X�`��
	{
		std::shared_ptr<Shader> shader = shader_manager.GetShader(ShaderManager::ShaderType::SkyBox);
		
		shader->Begin(context, render_context);
		sprite->Render(context,
			sky.get(),
			0, 0,
			screen_size.x, screen_size.y,
			0, 0,
			static_cast<float>(sky->GetWidth()), static_cast<float>(sky->GetHeight()),
			0,
			1, 1, 1, 1);
		shader->End(context);
	}

	// �f�o�b�O�v���~�e�B�u�`��
	{
		// �G�꒣��̃f�o�b�O�v���~�e�B�u�`��
		EnemyTerritoryManager::Instance().Render();
		EnemyManager::Instance().DrawDebugPrimitive();
		CollisionManager::Instance().Draw();
		graphics.GetDebugRenderer()->Render(context, render_context.view, render_context.projection);
	}

	// �A�N�^�[�`��
	{
		// �V���h�E�}�b�v�쐬
		ActorManager::Instance().ShadowRender(render_context, blur_render_context);

		// �����_�[�^�[�Q�b�g�̉�
		graphics.SetRenderTargetView(&screen_texture, depth_stencil_view);

		// �r���[�|�[�g�̐ݒ�
		graphics.SetViewport(graphics.GetScreenWidth(), graphics.GetScreenHeight());

		// �`��
		ActorManager::Instance().Render(render_context);
	}
}

//-------------------------------------
// �|�X�g�e�N�X�`���`��
//-------------------------------------
void SceneBattle::PostRender(ID3D11DeviceContext* context, RenderContext& render_context, const DirectX::XMFLOAT2& screen_size)
{
	bloom_texture = bloom->Render(context, render_context);
}

//-------------------------------------
// �o�b�N�o�b�t�@�`��
//-------------------------------------
void SceneBattle::BuckBufferRender(ID3D11DeviceContext* context, RenderContext& render_context, const DirectX::XMFLOAT2& screen_size)
{
	Graphics& graphics = Graphics::Instance();
	ShaderManager& shader_manager = ShaderManager::Instance();

	// �����_�[�^�[�Q�b�g�ݒ�
	{
		ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
		ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();
		graphics.SetRenderTargetView(&render_target_view, depth_stencil_view);
	}
	// �r���[�|�[�g�ݒ�
	graphics.SetViewport(screen_size.x, screen_size.y);

	//�o�b�N�o�b�t�@�ɃX�N���[���e�N�X�`����`��
	std::shared_ptr<Shader> sprite_shader = shader_manager.GetShader(ShaderManager::ShaderType::Sprite);
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

	// ���j���[�`��
	{
		if (MenuSystem::Instance().IsOpened())
		{
			sprite_shader->Begin(context);
			MenuSystem::Instance().Render(context);
			sprite_shader->End(context);
		}
	}

	// UI�`�揈��
	UIManager::Instance().Draw(context);

	// �V���h�E�}�b�v
	if (isshadowmap)
	{
		sprite_shader->Begin(context);
		for (int i = 0; i < 3; ++i)
		{
			sprite->Render(context,
				ActorManager::Instance().GetShadowTexture(i),
				0 + 200 * (i), 0,
				200, 200,
				0, 0,
				(float)ActorManager::Instance().GetShadowTexture(i)->GetWidth(), (float)ActorManager::Instance().GetShadowTexture(i)->GetHeight(),
				0,
				1, 1, 1, 1);
		}
		sprite_shader->End(context);
	}

	// 2D�v���~�e�B�u�`��
	{
		if (primitive_falg)
		{
			primitive->Begin(context, primitive_context);
			sprite->Render(context,
				0, 0,
				screen_size.x, screen_size.y,
				0, 0,
				1, 1);
			primitive->End(context);
		}
	}

	// 
	ClearOrOverRender(context);
}

//-------------------------------------
// ���b�Z�[�W����
//-------------------------------------
bool SceneBattle::OnMessages(const Telegram& telegram)
{
	switch (telegram.message_box.message)
	{
	case MessageType::Message_GameClear:
		isgame_clear = true;
		break;
	case MessageType::Message_GameOver:
		isgame_over = true;
		break;
	}
	return false;
}

//-------------------------------------
// GUI�`��
//-------------------------------------
void SceneBattle::OnGui()
{
	ImGui::SetNextWindowPos(ImVec2(550, 50), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	ImGui::Begin("Shader", nullptr, ImGuiWindowFlags_None);
	ImGui::TextColored(ImVec4(1, 1, 0, 1), u8"-------�V���h�E�}�b�v�\���t���O-------");
	ImGui::Checkbox("ShadowmapDrawFlag", &isshadowmap);

	ImGui::TextColored(ImVec4(1, 1, 0, 1), u8"-------2D�v���~�e�B�u-------");
	ImGui::SliderInt("PrimitiveType", &primitive_context.number, 1, 10);
	ImGui::InputFloat("Timer", &primitive_context.timer);
	ImGui::Checkbox("PrimitiveFlag", &primitive_falg);

	ImGui::End();

}

//-------------------------------------
// �N���A���Q�[���I�[�o�[�`��
//-------------------------------------
void SceneBattle::ClearOrOverRender(ID3D11DeviceContext*context)
{
	ShaderManager& shader_manager = ShaderManager::Instance();
	std::shared_ptr<Shader> sprite_shader = shader_manager.GetShader(ShaderManager::ShaderType::Sprite);
	sprite_shader->Begin(context);
	if (isgame_clear)
	{
		sprite->Render(context,
			clear_texture.get(),
			400, 100,
			(float)clear_texture->GetWidth(), (float)clear_texture->GetHeight(),
			0, 0,
			(float)clear_texture->GetWidth(), (float)clear_texture->GetHeight(),
			0,
			1, 1, 1, 1);
		sprite->Render(context,
			anybutton_texture.get(),
			400, 400,
			(float)anybutton_texture->GetWidth(), (float)anybutton_texture->GetHeight(),
			0, 0,
			(float)anybutton_texture->GetWidth(), (float)anybutton_texture->GetHeight(),
			0,
			1, 1, 1, 1);
	}
	else if (isgame_over)
	{
		sprite->Render(context,
			over_texture.get(),
			400, 100,
			(float)over_texture->GetWidth(), (float)over_texture->GetHeight(),
			0, 0,
			(float)over_texture->GetWidth(), (float)over_texture->GetHeight(),
			0,
			1, 1, 1, 1);
		sprite->Render(context,
			anybutton_texture.get(),
			400, 400,
			(float)anybutton_texture->GetWidth(), (float)anybutton_texture->GetHeight(),
			0, 0,
			(float)anybutton_texture->GetWidth(), (float)anybutton_texture->GetHeight(),
			0,
			1, 1, 1, 1);
	}
	sprite_shader->End(context);

}

//-------------------------------------
// �Q�[���N���A���菈��
//-------------------------------------
bool SceneBattle::IsGameClearJudgment()
{
	// ���j�t���O���i�[�����R���e�i�擾
	std::map<EnemyTerritoryTag, bool> defeat_teritory = EnemyManager::Instance().GetDefeatTeritory();
	
	// �h���S����S�Č��j������N���A
	return (defeat_teritory[EnemyTerritoryTag::ButtlePosition4] == true &&
		defeat_teritory[EnemyTerritoryTag::ButtlePosition5] == true);
}
