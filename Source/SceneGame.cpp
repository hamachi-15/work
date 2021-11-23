#include <imgui.h>
#include "SceneGame.h"
#include "Graphics.h"
#include "Camera.h"
#include "CameraController.h"
#include "Template.h"
#include "Light.h"
#include "Input.h"
#include "Movement.h"
#include "Actor.h"
#include "ActorManager.h"
#include "PhongVarianceShadowMap.h"
#include "GaussianBlurShader.h"
#include "ReflectSeaShader.h"
#include "UseCubeMapShader.h"
#include "LambertShader.h"
#include "Player.h"
#include "EnemySlime.h"
#include "Stage.h"
#include "MenuSystem.h"
#include "Messenger.h"
#include "MessageData.h"
#include "Collision.h"
#include "Texture.h"
#include "Sprite.h"
#include "GameDataBase.h"
#include "EnemyManager.h"
#include "MetaAI.h"
SceneGame::SceneGame()
{
}

SceneGame::~SceneGame()
{
}

void SceneGame::Initialize()
{
	Graphics& graphics = Graphics::Instance();

	GameDataBase::Instance();
	Light::Initialize();
	u_int BUFFER = 1024;

	// �J���������ݒ�
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(-400, 16, -416),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0)
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		800.0f);

	// �J�����R���g���[���[������
	camera_controller = std::make_unique<CameraController>();
	camera_controller->SetCameraAngle({ DirectX::XMConvertToRadians(25), 0.0f, 0.0f });
	camera_controller->SetTarget(DirectX::XMFLOAT3(-400, 16, -416));

	// �e�N�X�`���̓ǂݍ���
	sprite = std::make_unique<Sprite>();
	sky = std::make_unique<Texture>();
	sky->Load("Data/Sprite/SkyBox/FS002_Night.png");

	// �X�e�[�W�ǂݍ���
	{
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetUpModel("Data/Model/Filde/Stage.mdl");
		//actor->SetUpModel("Data/Model/ExampleStage/ExampleStage.mdl");
		actor->SetName("Stage");
		actor->SetPosition(DirectX::XMFLOAT3(0, 0, 0));
		actor->SetAngle(DirectX::XMFLOAT3( 0, DirectX::XMConvertToRadians(-90), 0));
		actor->SetScale(DirectX::XMFLOAT3(0.2f, 0.2f, 0.2f));
		//actor->SetScale(DirectX::XMFLOAT3(5.f, 5.f, 5.f));
		actor->AddComponent<Stage>();
		actor->AddShader<PhongVarianceShadowMap>(Graphics::Instance().GetDevice());

	}

	// �v���C���[�ǂݍ���
	{
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetUpModel("Data/Model/RPG-Character/Swordman.mdl");
		actor->SetName("Player");
		actor->SetAnimationNodeName("Motion");
		actor->SetPosition(DirectX::XMFLOAT3(-400, 16, -416));
		//actor->SetPosition(DirectX::XMFLOAT3(0, 0, 0));
		actor->SetAngle(DirectX::XMFLOAT3(0, 0, 0));
		actor->SetScale(DirectX::XMFLOAT3(0.07f, 0.07f, 0.07f));
		actor->AddComponent<Movement>();
		actor->AddComponent<Charactor>();
		actor->AddComponent<Player>();
		actor->AddShader<PhongVarianceShadowMap>(Graphics::Instance().GetDevice());
	}
	ActorManager::Instance().Update(0.01f);
	ActorManager::Instance().UpdateTransform();

	MetaAI::Instance();
}

void SceneGame::Finalize()
{
	// �G�}�l�[�W���[�̃N���A
	EnemyManager::Instance().AllRemove();

	// �A�N�^�[�̔j��
	ActorManager::Instance().AllDestroy();

	// ���b�Z���W���[�̃N���A
	Messenger::Instance().Clear();
}

void SceneGame::Update(float elapsed_time)
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

	//���C�g
	static float light_angle = DirectX::XM_PI;
	if (GetKeyState('A') < 0) light_angle += elapsed_time * 2.0f;
	if (GetKeyState('D') < 0) light_angle -= elapsed_time * 2.0f;

	Light::SetAmbient(DirectX::XMFLOAT3(0.2f, 0.2f, 0.2f));
	//���C�g����
	//LightDir.x = sinf(light_angle);
	//LightDir.y = -1.0f;
	//LightDir.z = cosf(light_angle);
	//Light::SetDirLight(LightDir, DirectX::XMFLOAT3(0.6f, 0.6f, 0.6f));
	
	// ���^AI�̍X�V����
	MetaAI::Instance().Update(elapsed_time);

	// �A�N�^�[�X�V����
	ActorManager::Instance().Update(elapsed_time);
	ActorManager::Instance().UpdateTransform();

	// �����蔻��X�V����
	CollisionManager::Instance().Update();

	// �J�����X�V����
	std::shared_ptr<Actor> actor = ActorManager::Instance().GetActor("Player");
	camera_controller->SetTarget({
		actor->GetPosition().x,
		actor->GetPosition().y + 1.0f,
		actor->GetPosition().z });
	camera_controller->Update(elapsed_time);
}

void SceneGame::Render()
{
	// �`�揈��
	render_context.light_direction = Light::LightDir;
	render_context.ShadowParameter = { shadow_color.x, shadow_color.y, shadow_color.z, 0.000001f };

	// �J�����p�����[�^�ݒ�
	Camera& camera = Camera::Instance();
	render_context.view = camera.GetView();
	render_context.projection = camera.GetProjection();

	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* context = graphics.GetDeviceContext();
	ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();

	// ��ʃN���A
	FLOAT color[] = { 1.0f, 0.9f, 0.0f, 1.0f };
	context->ClearRenderTargetView(render_target_view, color);
	context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);

	float screen_width = graphics.GetScreenWidth();
	float screen_height = graphics.GetScreenHeight();
	// �X�J�C�{�b�N�X�`��
	{
		graphics.GetSkyBoxShader()->Begin(context, render_context);
		sprite->Render(context,
			sky.get(),
			0, 0,
			screen_width, screen_height,
			0, 0,
			(float)sky->GetWidth(), (float)sky->GetHeight(),
			0,
			1, 1, 1, 1);
		graphics.GetSkyBoxShader()->End(context);
	}
	// �A�N�^�[�`��
	{
		// �V���h�E�}�b�v�쐬
		ActorManager::Instance().ShadowRender(render_context, blur_render_context);

		// �`��
		ActorManager::Instance().Render(render_context);
	}

	// ���j���[�`��
	{
		if (MenuSystem::Instance().IsOpened())
		{
			graphics.GetSpriteShader()->Begin(context);
			MenuSystem::Instance().Render(context);
			graphics.GetSpriteShader()->End(context);
		}
	}

	// �f�o�b�O�v���~�e�B�u�`��
	{
		EnemyManager::Instance().DrawDebugPrimitive();
		CollisionManager::Instance().Draw();
		graphics.GetDebugRenderer()->Render(context, render_context.view, render_context.projection);
	}
	OnGui();
}

void SceneGame::OnGui()
{
	ImGui::SetNextWindowPos(ImVec2(550, 50), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	ImGui::Begin("Shader", nullptr, ImGuiWindowFlags_None);
	ImGui::TextColored(ImVec4(1, 1, 0, 1), u8"-------�V���h�E�}�b�v�p�u���[-------");
	ImGui::SliderInt("KarnelSize", &blur_render_context.karnel_size, 1, 15);

	ImGui::TextColored(ImVec4(1, 1, 0, 1), u8"-------�V���h�E�}�b�v-------");
	ImGui::ColorEdit3("ShadowColor", (float*)&shadow_color);
	ImGui::End();
}
