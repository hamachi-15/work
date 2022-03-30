#include <imgui.h>
// �V�[��
#include "SceneGame.h"
#include "SceneBattle.h"

// �Z�p�֐�
#include "Mathf.h"

// �Q�[���f�[�^
#include "GameDataBase.h"
#include "ActorType.h"

// �`��
#include "Graphics.h"
#include "Light.h"

// �J����
#include "Camera.h"
#include "CameraController.h"

// ����
#include "Input.h"

// �}�l�[�W���[
#include "ResourceManager.h"
#include "ActorManager.h"
#include "EnemyManager.h"
#include "CollisionManager.h"
#include "ShaderManager.h"
#include "UIManager.h"
#include "EnemyTerritoryManager.h"
#include "EffectManager.h"
#include "SceneManager.h"
#include "AudioManager.h"
#include "StageManager.h"

// �R���|�[�l���g
#include "Actor.h"
#include "Player.h"
#include "EnemySlime.h"
#include "Stage.h"
#include "Movement.h"

// �����蔻��
#include "PlayerCollision.h"
#include "CullingCollision.h"

// �V�F�[�_�[
#include "CascadeShadowMapShader.h"
#include "LambertShader.h"
#include "BloomShader.h"
#include "PhongShader.h"
#include "2DPrimitive.h"

// ���j���[
#include "MenuSystem.h"

// ���b�Z�[�W
#include "Messenger.h"
#include "MessageData.h"

// AI
#include "MetaAI.h"

// �e�N�X�`��
#include "Texture.h"
#include "Sprite.h"

//-------------------------------------
// �R���X�g���N�^
//-------------------------------------
SceneGame::SceneGame()
{
	
}

//-------------------------------------
// �f�X�g���N�^
//-------------------------------------
SceneGame::~SceneGame()
{

}

//-------------------------------------
// ����������
//-------------------------------------
void SceneGame::Initialize()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11Device* device = graphics.GetDevice();
	
	// GPU�ɓn���f�[�^������
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
		DirectX::XMFLOAT3(-100, 1, -136),
		DirectX::XMFLOAT3(-100, 0, -116),
		DirectX::XMFLOAT3(0, 1, 0));
	camera.SetPerspectiveFov(Mathf::ConvartToRadian(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		1,
		2000.0f);

	// �J�����R���g���[���[������
	camera_controller = std::make_unique<CameraController>();
	camera_controller->SetCameraAngle({ Mathf::ConvartToRadian(25), 0.0f, 0.0f });
	camera_controller->SetTarget(DirectX::XMFLOAT3(-400, 16, -416));

	// �e�N�X�`���̓ǂݍ���
	sprite = std::make_unique<Sprite>();
	sky = ResourceManager::Instance().LoadTexture("Data/Sprite/SkyBox/FS002_Night.png");
	// �X�e�[�W�ǂݍ���
	{
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetUpModel("Data/Model/Filde/Filde.mdl", nullptr);
		actor->SetName("Filde");
		actor->SetPosition(DirectX::XMFLOAT3(0, 0, 0));
		actor->SetAngle(DirectX::XMFLOAT3( 0, Mathf::ConvartToRadian(-90), 0));
		actor->SetScale(DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f));
		actor->SetShaderType(ShaderManager::ShaderType::CascadeShadowMap);
	}

	// �X�e�[�W�I�u�W�F�N�g�̔z�u
	StageManager::Instance().WorldObjectRegister();

	// �v���C���[�ǂݍ���
	{
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetUpModel("Data/Model/RPG-Character/WorldMapPlayer.mdl", "Motion");
		actor->SetName("Player");
		actor->SetPosition(DirectX::XMFLOAT3(-100, 6, 116));
		actor->SetAngle(DirectX::XMFLOAT3(0, 0, 0));
		actor->SetScale(DirectX::XMFLOAT3(0.04f, 0.04f, 0.04f));
		// �R���|�[�l���g�ǉ�
		actor->AddComponent<Movement>();
		actor->AddComponent<Charactor>(static_cast<int>(MetaAI::Identity::Player));
		actor->AddComponent<Player>();
		actor->AddComponent<PlayerCollision>();
		// �v���C���[�̃J�����O�R���W������ǉ�
		CollisionManager::Instance().RegisterCulling(std::make_shared<CullingCollision>(ActorType::None, actor));
		actor->SetShaderType(ShaderManager::ShaderType::Phong);
	}
	// �G�̐���
	MetaAI::Instance().AppearanceEnemy();

	// �A�N�^�[�X�V����
	ActorManager::Instance().Update(0.01f);
	ActorManager::Instance().UpdateTransform();

	// BGM�Đ�
	AudioManager::Instance().PlayBGM(BGMType::WorldMap);
	AudioManager::Instance().SetBGMVolume(BGMType::WorldMap, 0.6f);
}

//-------------------------------------
// �I������
//-------------------------------------
void SceneGame::Finalize()
{
	// �G�}�l�[�W���[�̃N���A
	EnemyManager::Instance().AllRemove();

	// UI�̑S�j��
	UIManager::Instance().AllDelete();

	// �A�N�^�[�̔j��
	ActorManager::Instance().AllDestroy();

	// �R���W�����̔j��
	CollisionManager::Instance().Destroy();

	// ���b�Z���W���[�̃N���A
	Messenger::Instance().Clear();

	// BGM�Đ���~
	AudioManager::Instance().StopBGM(BGMType::WorldMap);
}

//-------------------------------------
// �X�V����
//-------------------------------------
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
	if (primitive_context->timer < Primitive_Max_Time)
	{
		primitive_context->timer++;
	}

	// �o�g���V�[���ւ̑J�ڃt���O�������Ă���
	if (battle_flag)
	{
		// �^�C�}�[�����ȏ�Ȃ�
		if (primitive_context->timer >= Primitive_Max_Time)
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
	if (primitive_context->timer >= Primitive_Max_Time)
	{
		// �v���~�e�B�u�v���O��false��
		primitive_falg = false;
	}

	// �����蔻��X�V����
	CollisionManager::Instance().Update();

	// �A�N�^�[�X�V����
	ActorManager::Instance().Update(elapsed_time);
	ActorManager::Instance().UpdateTransform();

	// UI�X�V����
	UIManager::Instance().Update(elapsed_time);

	// �J�����X�V����
	camera_controller->Update(elapsed_time);
}

//-------------------------------------
// �`�揈��	
//-------------------------------------
void SceneGame::Render()
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

	// �|�X�g�e�N�X�`���`��
	PostRender(context, render_context.get(), screen_size);

	// �X�N���[���e�N�X�`���ɕ`��
	ScreenRender(context, render_context.get(), screen_size);

	// �o�b�N�o�b�t�@�ɕ`��
	BuckBufferRender(context, render_context.get(), screen_size);
}

//-------------------------------------
// �X�N���[���e�N�X�`���ɕ`��
//-------------------------------------
void SceneGame::ScreenRender(ID3D11DeviceContext* context, RenderContext* render_context, const DirectX::XMFLOAT2& screen_size)
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
		skybox_shader->End(context);
	}

	// �A�N�^�[�`��
	{
		// �V���h�E�}�b�v�쐬
		ActorManager::Instance().ShadowRender(render_context, blur_render_context.get());
		
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
void SceneGame::PostRender(ID3D11DeviceContext* context, RenderContext* render_context, const DirectX::XMFLOAT2& screen_size)
{
	ShaderManager& shader_manager = ShaderManager::Instance();
	if(!sky_bloom_flag)
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
// �o�b�N�o�b�t�@�ɕ`��
//-------------------------------------
void SceneGame::BuckBufferRender(ID3D11DeviceContext* context, RenderContext* render_context, const DirectX::XMFLOAT2& screen_size)
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
// ���b�Z�[�W��M����
//-------------------------------------
bool SceneGame::OnMessages(const Telegram& telegram)
{
	switch (telegram.message_box.message)
	{
	case MessageType::Message_Hit_Boddy:
		battle_flag = true;
		primitive_falg = true;
		primitive_context->number = 3;
		primitive_context->timer = 0.0f;

		// �G���J�E���g�����G�̃e���g���[�ɑ�����G��T��
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
	ImGui::SliderInt("PrimitiveType", &primitive_context->number, 1, 10);
	ImGui::InputFloat("Timer", &primitive_context->timer);
	ImGui::Checkbox("PrimitiveFlag", &primitive_falg);


	ImGui::End();
}
