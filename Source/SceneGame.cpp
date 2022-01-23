#include <imgui.h>
#include "SceneGame.h"
#include "SceneBattle.h"
#include "SceneManager.h"
#include "Mathf.h"

// �f�[�^�n
#include "GameDataBase.h"

#include "Graphics.h"
#include "Camera.h"
#include "CameraController.h"
#include "Template.h"
#include "Light.h"
#include "Input.h"

#include "ActorManager.h"
#include "EnemyManager.h"
#include "CollisionManager.h"
#include "ShaderManager.h"
#include "UIManager.h"
#include "EnemyTerritoryManager.h"

#include "Actor.h"
#include "Player.h"
#include "EnemySlime.h"
#include "Stage.h"
#include "Movement.h"


#include "CascadeShadowMapShader.h"
#include "LambertShader.h"
#include "BloomShader.h"
#include "PhongShader.h"
#include "2DPrimitive.h"

#include "MenuSystem.h"
#include "Messenger.h"
#include "MessageData.h"
#include "MetaAI.h"
#include "Texture.h"
#include "Sprite.h"

SceneGame::SceneGame()
{
	
}

SceneGame::~SceneGame()
{

}

void SceneGame::Initialize()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11Device* device = graphics.GetDevice();

	// �v���~�e�B�u�̃R���X�^���g�o�b�t�@�̏����ݒ�
	primitive_falg = true;
	primitive_context.number = 2;
	primitive_context.timer = 0.0f;

	// �V�[�����ݒ�
	SetName("SceneWorldMap");

	// �f�[�^�x�[�X������
	GameDataBase::Instance();
	
	// ���C�g������
	Light::Initialize();

	// �J���������ݒ�
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(-100, 6, -136),
		DirectX::XMFLOAT3(-100, 6, -116),
		DirectX::XMFLOAT3(0, 1, 0)
	);
	camera.SetPerspectiveFov(Mathf::ConvartToRadian(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		1,
		2000.0f);

	// �J�����R���g���[���[������
	camera_controller = std::make_unique<CameraController>();
	camera_controller->SetCameraAngle({ Mathf::ConvartToRadian(25), 0.0f, 0.0f });
	camera_controller->SetTarget(DirectX::XMFLOAT3(-400, 16, -416));

	// �V�F�[�_�[������
	bloom = std::make_unique<Bloom>(device);

	// �e�N�X�`���쐬
	bulr_texture = std::make_unique<Texture>();
	bulr_texture->Create((u_int)graphics.GetScreenWidth(), (u_int)graphics.GetScreenHeight(), DXGI_FORMAT_R16G16B16A16_FLOAT);

	depth_texture = std::make_unique<Texture>();
	depth_texture->CreateDepthStencil((u_int)2048, (u_int)1024);

	// �e�N�X�`���̓ǂݍ���
	sprite = std::make_unique<Sprite>();
	sky = std::make_unique<Texture>();
	sky->Load("Data/Sprite/SkyBox/FS002_Night.png");
	sky_texture = std::make_unique<Texture>();
	sky_texture->Create(static_cast<float>(sky->GetWidth()), static_cast<float>(sky->GetHeight()), DXGI_FORMAT_R16G16B16A16_FLOAT);

	// �X�e�[�W�ǂݍ���
	{
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetUpModel("Data/Model/Filde/Filde.mdl", nullptr);
		actor->SetName("Filde");
		actor->SetPosition(DirectX::XMFLOAT3(0, 0, 0));
		actor->SetAngle(DirectX::XMFLOAT3( 0, Mathf::ConvartToRadian(-90), 0));
		actor->SetScale(DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f));
		//actor->SetScale(DirectX::XMFLOAT3(1.f, 1.f, 1.f));
		actor->AddComponent<Stage>();
		actor->SetShaderType(ShaderManager::ShaderType::CascadeShadowMap);
	}
	{
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetUpModel("Data/Model/Filde/StageObjects.mdl", nullptr);
		actor->SetName("FildeObjects");
		actor->SetPosition(DirectX::XMFLOAT3(0, 0, 0));
		actor->SetAngle(DirectX::XMFLOAT3(0, Mathf::ConvartToRadian(-90), 0));
		actor->SetScale(DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f));
		//actor->SetScale(DirectX::XMFLOAT3(1.f, 1.f, 1.f));
		actor->AddComponent<Stage>();
		actor->SetShaderType(ShaderManager::ShaderType::Lambert);
	}
	// �v���C���[�ǂݍ���
	{
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetUpModel("Data/Model/RPG-Character/WorldMapPlayer.mdl", "Motion");
		actor->SetName("Player");
		actor->SetPosition(DirectX::XMFLOAT3(-100, 6, -116));
		actor->SetAngle(DirectX::XMFLOAT3(0, 0, 0));
		actor->SetScale(DirectX::XMFLOAT3(0.04f, 0.04f, 0.04f));
		//actor->SetScale(DirectX::XMFLOAT3(0.8f, 0.8f, 0.8f));
		actor->AddComponent<Movement>();
		actor->AddComponent<Charactor>();
		actor->AddComponent<Player>();
		actor->SetShaderType(ShaderManager::ShaderType::Lambert);
	}
	//ActorManager::Instance().Update(0.01f);
	ActorManager::Instance().UpdateTransform();

	MetaAI::Instance();
}

void SceneGame::Finalize()
{
	// �G�}�l�[�W���[�̃N���A
	EnemyManager::Instance().AllRemove();

	// �A�N�^�[�̔j��
	ActorManager::Instance().AllDestroy();

	// �R���W�����̔j��
	CollisionManager::Instance().Destroy();

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

	// �v���~�e�B�u�R���e�L�X�g�̃R���X�g���N�^�X�V
	if (primitive_context.timer < Primitive_Max_Time)
	{
		primitive_context.timer++;
	}

	// �o�g���V�[���ւ̑J�ڃt���O�������Ă���
	if (battle_flag)
	{
		// �^�C�}�[�����ȏ�Ȃ�
		if (primitive_context.timer >= Primitive_Max_Time)
		{
			// �o�g���V�[���֑J��
			SceneManager::Instance().ChangeScene(new SceneBattle());
			return;
		}
		else
		{
			return;
		}
	}

	// �v���~�e�B�u�p�̃^�C�}�[�����ȏ�Ȃ�
	if (primitive_context.timer >= Primitive_Max_Time)
	{
		// �v���~�e�B�u�v���O��false��
		primitive_falg = false;
	}

	//���C�g
	static float light_angle = DirectX::XM_PI;
	if (GetKeyState('E') < 0) light_angle += elapsed_time * 2.0f;
	if (GetKeyState('Q') < 0) light_angle -= elapsed_time * 2.0f;

	Light::SetAmbient(DirectX::XMFLOAT3(0.2f, 0.2f, 0.2f));
	//���C�g����
	LightDir.x = sinf(light_angle);
	LightDir.y = -1.0f;
	LightDir.z = cosf(light_angle);
	Light::SetDirLight(LightDir, DirectX::XMFLOAT3(0.6f, 0.6f, 0.6f));
	

	// ���^AI�̍X�V����
	MetaAI::Instance().Update(elapsed_time);

	// �����蔻��X�V����
	CollisionManager::Instance().Update();

	// �A�N�^�[�X�V����
	ActorManager::Instance().Update(elapsed_time);
	ActorManager::Instance().UpdateTransform();

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

// �`�揈��	
void SceneGame::Render()
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

	// �|�X�g�G�t�F�N�g�p�̃e�N�X�`���`��
	PostRender(context, render_context, screen_size);

	// �X�N���[���e�N�X�`���ɕ`��
	ScreenRender(context, render_context, screen_size);

	// �o�b�N�o�b�t�@�ɕ`��
	BuckBufferRender(context, render_context, screen_size);

	// GUI�`��
	OnGui();
}

//-------------------------------------
// �X�N���[���e�N�X�`���ɕ`��
//-------------------------------------
void SceneGame::ScreenRender(ID3D11DeviceContext* context, RenderContext& render_context, const DirectX::XMFLOAT2& screen_size)
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
		std::shared_ptr<Shader> skybox_shader = shader_manager.GetShader(ShaderManager::ShaderType::SkyBox);
		skybox_shader->Begin(context, render_context);
		sprite->Render(context,
			sky_texture.get(),
			0, 0,
			screen_size.x, screen_size.y,
			0, 0,
			static_cast<float>(sky_texture->GetWidth()), static_cast<float>(sky_texture->GetHeight()),
			0,
			1, 1, 1, 1);
		skybox_shader->End(context);
	}

	// �f�o�b�O�v���~�e�B�u�`��
	{
		// �G�꒣��̃f�o�b�O�v���~�e�B�u�`��
		EnemyTerritoryManager::Instance().Render();
		// �G�̃f�o�b�O�v���~�e�B�u�`��
		EnemyManager::Instance().DrawDebugPrimitive();
		// �����蔻��̂̃f�o�b�O�v���~�e�B�u�`��
		CollisionManager::Instance().Draw();
		for (int i = 0; i < 4; ++i)
		{
			graphics.GetDebugRenderer()->DrawSphere(camera_controller->far_position[i], 0.4f, color);
			graphics.GetDebugRenderer()->DrawSphere(camera_controller->near_position[i], 0.4f, color);
		}
		graphics.GetDebugRenderer()->Render(context, render_context.view, render_context.projection);
	}

	// �A�N�^�[�`��
	{
		// �V���h�E�}�b�v�쐬
		ActorManager::Instance().ShadowRender(render_context, blur_render_context);
		
		// �����_�[�^�[�Q�b�g�̉�
		graphics.SetRenderTargetView(&screen_texture, depth_stencil_view);

		// �r���[�|�[�g�̐ݒ�
		graphics.SetViewport(screen_size.x, screen_size.y);

		// �`��
		ActorManager::Instance().Render(render_context);
	}
}

//-------------------------------------
// �|�X�g�G�t�F�N�g�Ɏg���e�N�X�`���`��
//-------------------------------------
void SceneGame::PostRender(ID3D11DeviceContext* context, RenderContext& render_context, const DirectX::XMFLOAT2& screen_size)
{
	Graphics& graphics = Graphics::Instance();
	ShaderManager& shader_manager = ShaderManager::Instance();
	std::shared_ptr<Shader> bloom_shader = shader_manager.GetShader(ShaderManager::ShaderType::Bloom);

	// ��̃e�N�X�`���̃u���[������
	bloom_texture = dynamic_cast<Bloom*>(bloom_shader.get())->Render(context, render_context, sky.get());

	// �����_�[�^�[�Q�b�g�ݒ�
	{
		ID3D11RenderTargetView* render_target_view = sky_texture->GetRenderTargetView();
		ID3D11DepthStencilView* depth_stencil_view = depth_texture->GetDepthStencilView();
		graphics.SetRenderTargetView(&render_target_view, depth_stencil_view);
		graphics.ScreenClear(&render_target_view, depth_stencil_view);
	}
	// �r���[�|�[�g�ݒ�
	graphics.SetViewport(sky_texture->GetWidth(), sky_texture->GetHeight());

	// ��e�N�X�`���Ƌ�e�N�X�`���̋P�x�𒊏o�����e�N�X�`�������Z����
	std::shared_ptr<Shader> sprite_shader = shader_manager.GetShader(ShaderManager::ShaderType::Sprite);
	sprite_shader->Begin(context);
	sprite->Render(context,
			sky.get(),
			0, 0,
			static_cast<float>(sky_texture->GetWidth()), static_cast<float>(sky_texture->GetHeight()),
			0, 0,
			static_cast<float>(sky->GetWidth()), static_cast<float>(sky->GetHeight()),
			0,
			1, 1, 1, 1);
	context->OMSetBlendState(graphics.GetBlendState((int)Graphics::BlendState::Add), nullptr, 0xFFFFFFFF);
	sprite->AddRender(context,
			bloom_texture,
			0, 0,
			static_cast<float>(sky_texture->GetWidth()), static_cast<float>(sky_texture->GetHeight()),
			0, 0,
			static_cast<float>(bloom_texture->GetWidth()), static_cast<float>(bloom_texture->GetHeight()));
	sprite_shader->End(context);
	context->OMSetBlendState(graphics.GetBlendState((int)Graphics::BlendState::Alpha), nullptr, 0xFFFFFFFF);

	//std::shared_ptr<Shader> bloom_shader = shader_manager.GetShader(ShaderManager::ShaderType::Bulr);

	//Texture* texture = bulr->Render(graphics.GetTexture());
	//{
	//	ID3D11RenderTargetView* render_target_view[1] = { bulr_texture->GetRenderTargetView() };
	//	graphics.SetRenderTargetView(render_target_view, depth_stencil_view);
	//}
	//// �r���[�|�[�g�̐ݒ�
	//graphics.SetViewport(screen_size.x, screen_size.y);
	//graphics.GetSpriteShader()->Begin(context);
	//sprite->Render(context,
	//	texture,
	//	0, 0,
	//	screen_size.x, screen_size.y,
	//	0, 0,
	//	texture->GetWidth(), texture->GetHeight());
	//graphics.GetSpriteShader()->End(context);
}

//-------------------------------------
// �o�b�N�o�b�t�@�ɕ`��
//-------------------------------------
void SceneGame::BuckBufferRender(ID3D11DeviceContext* context, RenderContext& render_context, const DirectX::XMFLOAT2& screen_size)
{
	Graphics& graphics = Graphics::Instance();
	ShaderManager& shader_manager = ShaderManager::Instance();
	// �X�v���C�g�V�F�[�_�[�擾
	std::shared_ptr<Shader> sprite_shader = shader_manager.GetShader(ShaderManager::ShaderType::Sprite);

	// �����_�[�^�[�Q�b�g�ݒ�
	{
		ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
		ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();
		graphics.SetRenderTargetView(&render_target_view, depth_stencil_view);
		graphics.ScreenClear(&render_target_view, depth_stencil_view);
	}
	// �r���[�|�[�g�ݒ�
	graphics.SetViewport(screen_size.x, screen_size.y);

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
	// �P�x���o�e�N�X�`�������Z����
	//context->OMSetBlendState(graphics.GetBlendState((int)Graphics::BlendState::Add), nullptr, 0xFFFFFFFF);
	//sprite->AddRender(context,
	//	bloom_texture,
	//	0, 0,
	//	screen_size.x, screen_size.y,
	//	0, 0,
	//	(float)bloom_texture->GetWidth(), (float)bloom_texture->GetHeight());

	//// �u�����h�X�e�[�g���A���t�@���Z�����ɐݒ�
	//context->OMSetBlendState(graphics.GetBlendState((int)Graphics::BlendState::Alpha), nullptr, 0xFFFFFFFF);
	
	// ���j���[�`��
	{
		if (MenuSystem::Instance().IsOpened())
		{
			MenuSystem::Instance().Render(context);
		}
	}

	// �V���h�E�}�b�v
	if (isshadowmap)
	{
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
	}
	sprite->Render(context,
		bloom_texture,
		0, 200,
		200, 200,
		0, 0,
		(float)bloom_texture->GetWidth(), (float)bloom_texture->GetHeight(),
		0,
		1, 1, 1, 1);
	sprite_shader->End(context);

	// 2D�v���~�e�B�u�`��
	{
		if (primitive_falg)
		{
			std::shared_ptr<Shader> primitive = shader_manager.GetShader(ShaderManager::ShaderType::Primitive);
			primitive->Begin(context, primitive_context);
			sprite->Render(context,
				0, 0,
				screen_size.x, screen_size.y,
				0, 0,
				1, 1);
			primitive->End(context);
		}
	}
}

//-------------------------------------
// ���b�Z�[�W��M����
//-------------------------------------
bool SceneGame::OnMessages(const Telegram& telegram)
{
	switch (telegram.message_box.message)
	{
	case MessageType::Message_Hit_Boddy:
		battle_flag = true;
		primitive_falg = true;
		primitive_context.number = 3;
		primitive_context.timer = 0.0f;

		// TODO �G���J�E���g�����G�̃e���g���[�ɑ�����G��T��
		GameDataBase::Instance().EnemyFriendFromTerritory(telegram.message_box.territory_tag);
		
		// �v���C���[�R���|�[�l���g�擾
		std::shared_ptr<Actor> player = ActorManager::Instance().GetActor("Player");

		// �G���J�E���g���̃J�������[�V����
		DirectX::XMFLOAT3 start_position = Camera::Instance().GetEye();
		MessageData::CameraChangeMotionModeData motioncamera_data;
		// �I�C���[�p�E���W�擾
		DirectX::XMFLOAT3 angle = player->GetAngle();
		DirectX::XMFLOAT3 position = player->GetPosition();
		// �v���C���[�̑O�����Z�o
		float vx = sinf(angle.y) * 20;
		float vz = cosf(angle.y) * 20;
		motioncamera_data.data.push_back({ 0, {start_position.x, start_position.y, start_position.z}, position });
		motioncamera_data.data.push_back({ 20, {start_position.x + vx, start_position.y - 0.5f, start_position.z + vz }, position });
		Messenger::Instance().SendData(MessageData::CameraChangeMotionMode, &motioncamera_data);

		return true;
		break;
	}

	return false;
}

//-------------------------------------
// GUI�`��
//-------------------------------------
void SceneGame::OnGui()
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

