#include <imgui.h>
#include <stdio.h>
// �V�[��
#include "SceneBattle.h"
#include "SceneGame.h"
#include "SceneOver.h"
#include "SceneClear.h"

// �Z�p�֐�
#include "Mathf.h"

// �`��
#include "Graphics.h"
#include "Light.h"

// �J����
#include "Camera.h"
#include "CameraController.h"

// ����
#include "Input.h"

// �R���|�[�l���g
#include "Movement.h"
#include "Actor.h"
#include "EnemySlime.h"
#include "Player.h"
#include "Stage.h"

// �V�F�[�_�[
#include "BloomShader.h"

// �}�l�[�W���[
#include "ActorManager.h"
#include "EnemyManager.h"
#include "FireBallManager.h"
#include "EffectManager.h"
#include "EnemyTerritoryManager.h"
#include "ResourceManager.h"
#include "CollisionManager.h"
#include "ShaderManager.h"
#include "StageManager.h"
#include "AudioManager.h"
#include "UIManager.h"

// ���j���[
#include "MenuSystem.h"

// ���b�Z�[�W
#include "Messenger.h"
#include "MessageData.h"

// �Q�[���f�[�^
#include "GameDataBase.h"

// �e�N�X�`��
#include "Texture.h"
#include "Sprite.h"

// AI
#include "MetaAI.h"

// UI
#include "PlayerUIHealth.h"
#include "PlayerCollision.h"


//---------------------------------
// �R���X�g���N�^
//---------------------------------
SceneBattle::SceneBattle()
{
}

//---------------------------------
// �f�X�g���N�^
//---------------------------------
SceneBattle::~SceneBattle()
{
}

//---------------------------------
// �J�n����
//---------------------------------
void SceneBattle::Initialize()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11Device* device = graphics.GetDevice();

	render_context = std::make_unique<RenderContext>();
	blur_render_context = std::make_unique<BlurRenderContext>();
	primitive_context = std::make_unique<PrimitiveContext>();
	
	// �v���~�e�B�u�̃R���X�^���g�o�b�t�@�̏����ݒ�
	primitive_falg = true;
	primitive_context->number = 2;
	primitive_context->timer = 0.0f;

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

	// �e�N�X�`���̓ǂݍ���
	sprite = std::make_unique<Sprite>();
	sky = ResourceManager::Instance().LoadTexture("Data/Sprite/SkyBox/FS002_Night.png");
	anybutton_texture = ResourceManager::Instance().LoadTexture("Data/Sprite/PushAnyButton.png");

	// �X�e�[�W�ǂݍ���
	{
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetUpModel("Data/Model/Filde/Filde.mdl", nullptr);
		actor->SetName("Filde");
		actor->SetPosition(DirectX::XMFLOAT3(0, 0, 0));
		actor->SetAngle(DirectX::XMFLOAT3(0, Mathf::ConvartToRadian(-90), 0));
		actor->SetScale(DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f));
		actor->SetShaderType(ShaderManager::ShaderType::CascadeShadowMap);
	}
	
	// �X�e�[�W�I�u�W�F�N�g�̔z�u
	StageManager::Instance().ButtleObjectRegister();

	// �v���C���[�ǂݍ���
	{
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetUpModel("Data/Model/RPG-Character/Swordman.mdl", "Motion");
		actor->SetName("Player");
		actor->SetPosition(DirectX::XMFLOAT3(-278.0f, 5.5f, 34.4f));
		actor->SetAngle(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
		actor->SetScale(DirectX::XMFLOAT3(0.04f, 0.04f, 0.04f));
		actor->AddComponent<Movement>();
		actor->AddComponent<Charactor>(static_cast<int>(MetaAI::Identity::Player));
		std::shared_ptr<PlayerHealthUI> ui = actor->AddComponent<PlayerHealthUI>();
		actor->AddComponent<Player>();
		actor->AddComponent<PlayerCollision>();
		UIManager::Instance().RegisterUI(ui);
		actor->SetShaderType(ShaderManager::ShaderType::Phong);
	}
	ActorManager::Instance().Update(0.01f);
	ActorManager::Instance().UpdateTransform();
	
	// �G���J�E���g�f�[�^����G�̐���
	EnemyManager::Instance().CreateEnemyEncountData();

	// BGM�Đ�
	AudioManager::Instance().PlayBGM(BGMType::ButtleMap);
	AudioManager::Instance().SetBGMVolume(BGMType::ButtleMap, 0.5f);
}

//---------------------------------
// �I������
//---------------------------------
void SceneBattle::Finalize()
{
	// �G�}�l�[�W���[�̃N���A
	EnemyManager::Instance().AllRemove();

	// UI�̑S�j��
	UIManager::Instance().AllDelete();

	// �A�N�^�[�̔j��
	ActorManager::Instance().AllDestroy();

	// �R���W�����̔j��
	CollisionManager::Instance().Destroy();

	// �΋��}�l�[�W���[�j��
	FireBallManager::Instance().Destroy();

	// ���b�Z���W���[�̃N���A
	Messenger::Instance().Clear();

	// BGM�Đ���~
	AudioManager::Instance().StopBGM(BGMType::ButtleMap);
}

//---------------------------------
// �X�V����
//---------------------------------
void SceneBattle::Update(float elapsed_time)
{
	// �O���t�B�N�X�擾
	Graphics& graphics = Graphics::Instance();

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
	// �Q�[�����N���A�ɂȂ��Ă����
	if (isgame_clear)
	{		
		// �N���A�V�[���ɑJ��
		SceneManager::Instance().ChangeScene(new SceneClear());
		return;
	}
	// �Q�[���I�[�o�[�ɂȂ��Ă����
	if (isgame_over)
	{		
		// �Q�[���I�[�o�[�V�[���ɑJ��
		SceneManager::Instance().ChangeScene(new SceneOver());
		return;
	}
	// �퓬���I��������
	if (isbuttle_end)
	{		
		// �Q�[���V�[���ɑJ��
		SceneManager::Instance().ChangeScene(new SceneGame());
		return;
	}

	// �v���~�e�B�u�R���e�L�X�g�̃R���X�g���N�^�X�V
	if (primitive_context->timer < 40)
	{
		primitive_context->timer++;
	}
	if (primitive_context->timer >= 40)
	{
		primitive_falg = false;
	}


	// �A�N�^�[�X�V����
	ActorManager::Instance().Update(elapsed_time);
	ActorManager::Instance().UpdateTransform();

	// �΋��X�V����
	FireBallManager::Instance().Update(elapsed_time);
	
	//�G�t�F�N�g�X�V����
	graphics.GetEffectManager()->Update(elapsed_time);

	// �����蔻��X�V����
	CollisionManager::Instance().Update();

	// UI�X�V����
	UIManager::Instance().Update(elapsed_time);

	// �J�����X�V����
	camera_controller->Update(elapsed_time);
}

//-------------------------------------
// �`�揈��
//-------------------------------------
void SceneBattle::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* context = graphics.GetDeviceContext();

	// �X�N���[���T�C�Y�擾
	DirectX::XMFLOAT2 screen_size = { graphics.GetScreenWidth(), graphics.GetScreenHeight() };

	// �`�揈��
	render_context->light_direction = Light::LightDir;
	render_context->ShadowParameter = { shadow_color.x, shadow_color.y, shadow_color.z, 0.001f };

	// �J�����p�����[�^�ݒ�
	Camera& camera = Camera::Instance();
	render_context->view = camera.GetView();
	render_context->projection = camera.GetProjection();

	// �o�b�N�o�b�t�@�̃N���A����
	{
		ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
		ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();
		// ��ʃN���A
		graphics.ScreenClear(&render_target_view, depth_stencil_view);
	}


	// �|�X�g�e�N�X�`���`��
	PostRender(context, render_context.get(), screen_size);

	// �X�N���[���e�N�X�`���ɕ`��
	ScreenRender(context, render_context.get(), screen_size);

	// �o�b�N�o�b�t�@�ɕ`��
	BuckBufferRender(context, render_context.get(), screen_size);
}


//-------------------------------------
// �X�N���[���e�N�X�`���`��
//-------------------------------------
void SceneBattle::ScreenRender(ID3D11DeviceContext* context, RenderContext* render_context, const DirectX::XMFLOAT2& screen_size)
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
		sprite->Render(context, sky.get(),
			0, 0,
			screen_size.x, screen_size.y,
			0, 0,
			(float)sky->GetWidth(), (float)sky->GetHeight());
		//�u�����h�X�e�[�g�ݒ�
		context->OMSetBlendState(graphics.GetBlendState((int)Graphics::BlendState::Add), nullptr, 0xFFFFFFFF);
		sprite->AddRender(context,
			bloom_texture,
			0, 0,
			screen_size.x, screen_size.y,
			0, 0,
			static_cast<float>(bloom_texture->GetWidth()), static_cast<float>(bloom_texture->GetHeight()),
			0,
			1, 1, 1, 1);
		shader->End(context);
	}

	// �A�N�^�[�`��
	{
		// �V���h�E�}�b�v�쐬
		ActorManager::Instance().ShadowRender(render_context, blur_render_context.get());

		// �����_�[�^�[�Q�b�g�̉�
		graphics.SetRenderTargetView(&screen_texture, depth_stencil_view);

		// �r���[�|�[�g�̐ݒ�
		graphics.SetViewport(graphics.GetScreenWidth(), graphics.GetScreenHeight());

		// �`��
		ActorManager::Instance().Render(render_context);
	}
	//�G�t�F�N�g�`�揈��
	graphics.GetEffectManager()->Render(render_context->view, render_context->projection);

	// �΋��`�揈��
	FireBallManager::Instance().Render(render_context->view, render_context->projection);
}

//-------------------------------------
// �|�X�g�e�N�X�`���`��
//-------------------------------------
void SceneBattle::PostRender(ID3D11DeviceContext* context, RenderContext* render_context, const DirectX::XMFLOAT2& screen_size)
{
	ShaderManager& shader_manager = ShaderManager::Instance();
	if (!sky_bloom_flag)
	{
		// �u���[���V�F�[�_�[�擾
		std::shared_ptr<Shader> bloom_shader = shader_manager.GetShader(ShaderManager::ShaderType::Bloom);
		// ��̃e�N�X�`���̃u���[������
		bloom_texture = dynamic_cast<Bloom*>(bloom_shader.get())->Render(context, render_context, sky.get());
		// ��̃u���[�����쐬�����̂Ńt���O�𗧂Ăĕ`�悵�Ȃ��悤�ɂ���
		sky_bloom_flag = true;
	}
}

//-------------------------------------
// �Q�[���I�[�o�[�E�N���A�̏u�Ԃ�`��
//-------------------------------------
void SceneBattle::MomentRender(ID3D11DeviceContext* context, const DirectX::XMFLOAT2& screen_size)
{
	Graphics& graphics = Graphics::Instance();
	ShaderManager& shader_manager = ShaderManager::Instance();
	GameDataBase& database = GameDataBase::Instance();
	// �����_�[�^�[�Q�b�g�ݒ�
	{
		ID3D11RenderTargetView* render_target_view = database.GetTimingTexture()->GetRenderTargetView();
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
}

//-------------------------------------
// �o�b�N�o�b�t�@�`��
//-------------------------------------
void SceneBattle::BuckBufferRender(ID3D11DeviceContext* context, RenderContext* render_context, const DirectX::XMFLOAT2& screen_size)
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
	// UI�`�揈��
	UIManager::Instance().Draw(context);

	// ���j���[�`��
	{
		if (MenuSystem::Instance().IsOpened())
		{
			MenuSystem::Instance().Render(context);
		}
	}
	sprite_shader->End(context);


	// 2D�v���~�e�B�u�`��
	{
		if (primitive_falg)
		{
			// �u���[���V�F�[�_�[�擾
			std::shared_ptr<Shader> primitive_shader = shader_manager.GetShader(ShaderManager::ShaderType::Primitive);
			primitive_shader->Begin(context, primitive_context.get());
			sprite->Render(context,
				0, 0,
				screen_size.x, screen_size.y,
				0, 0,
				1, 1);
			primitive_shader->End(context);
		}
	}
}

//-------------------------------------
// ���b�Z�[�W����
//-------------------------------------
bool SceneBattle::OnMessages(const Telegram& telegram)
{
	Graphics& graphics = Graphics::Instance();
	switch (telegram.message_box.message)
	{
	case MessageType::Message_Buttle_End:
		// �|�����G���L�^
		EnemyManager::Instance().SetDefeatTeritory(telegram.message_box.territory_tag, true);

		// �N���A���Ă��邩����
		if (IsGameClearJudgment())
		{
			// �N���A�Ȃ�t���O�𗧂Ă�
			isgame_clear = true;
			return true;
		}
		isbuttle_end = true;
		break;
	case MessageType::Message_GameClear:
		isgame_clear = true;
		return true;
		break;
	case MessageType::Message_GameOver:
		isgame_over = true;
		return true;
		break;
	case MessageType::Message_Moment_Render:
	{
		// �X�N���[���T�C�Y�擾
		DirectX::XMFLOAT2 screen_size = { graphics.GetScreenWidth(), graphics.GetScreenHeight() };
		// �Q�[���I�[�o�[�E�N���A�̏u�Ԃ�`��
		MomentRender(graphics.GetDeviceContext(), screen_size);
		return true;
	}
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
	ImGui::SliderInt("PrimitiveType", &primitive_context->number, 1, 10);
	ImGui::InputFloat("Timer", &primitive_context->timer);
	ImGui::Checkbox("PrimitiveFlag", &primitive_falg);

	ImGui::End();

}

//-------------------------------------
// �Q�[���N���A���菈��
//-------------------------------------
bool SceneBattle::IsGameClearJudgment()
{
	// �G�l�~�[�}�l�[�W���[�擾
	EnemyManager& enemy_manager = EnemyManager::Instance();
	
	// �h���S����S�Č��j������N���A
	return (enemy_manager.GetDefeatTeritory(EnemyTerritoryTag::Territory4) == true &&
		enemy_manager.GetDefeatTeritory(EnemyTerritoryTag::Territory5) == true);
}
