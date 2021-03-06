#include <imgui.h>
// シーン
#include "SceneGame.h"
#include "SceneBattle.h"

// 算術関数
#include "Mathf.h"

// ゲームデータ
#include "GameDataBase.h"
#include "ActorType.h"

// 描画
#include "Graphics.h"
#include "Light.h"

// カメラ
#include "Camera.h"
#include "CameraController.h"

// 入力
#include "Input.h"

// マネージャー
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

// コンポーネント
#include "Actor.h"
#include "Player.h"
#include "EnemySlime.h"
#include "Stage.h"
#include "Movement.h"

// 当たり判定
#include "PlayerCollision.h"
#include "CullingCollision.h"

// シェーダー
#include "CascadeShadowMapShader.h"
#include "LambertShader.h"
#include "BloomShader.h"
#include "PhongShader.h"
#include "2DPrimitive.h"

// メニュー
#include "MenuSystem.h"

// メッセージ
#include "Messenger.h"
#include "MessageData.h"

// AI
#include "MetaAI.h"

// テクスチャ
#include "Texture.h"
#include "Sprite.h"

//-------------------------------------
// コンストラクタ
//-------------------------------------
SceneGame::SceneGame()
{
	
}

//-------------------------------------
// デストラクタ
//-------------------------------------
SceneGame::~SceneGame()
{

}

//-------------------------------------
// 初期化処理
//-------------------------------------
void SceneGame::Initialize()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11Device* device = graphics.GetDevice();
	
	// GPUに渡すデータ初期化
	render_context = std::make_unique<RenderContext>();
	blur_render_context = std::make_unique<BlurRenderContext>();
	primitive_context = std::make_unique<PrimitiveContext>();

	// プリミティブのコンスタントバッファの初期設定
	primitive_falg = true;
	primitive_context->number = 2;
	primitive_context->timer = 0.0f;

	// ライト初期化
	Light::Initialize();

	// カメラ初期設定
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(-100, 1, -136),
		DirectX::XMFLOAT3(-100, 0, -116),
		DirectX::XMFLOAT3(0, 1, 0));
	camera.SetPerspectiveFov(Mathf::ConvartToRadian(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		1,
		2000.0f);

	// カメラコントローラー初期化
	camera_controller = std::make_unique<CameraController>();
	camera_controller->SetCameraAngle({ Mathf::ConvartToRadian(25), 0.0f, 0.0f });
	camera_controller->SetTarget(DirectX::XMFLOAT3(-400, 16, -416));

	// テクスチャの読み込み
	sprite = std::make_unique<Sprite>();
	sky = ResourceManager::Instance().LoadTexture("Data/Sprite/SkyBox/FS002_Night.png");
	// ステージ読み込み
	{
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetUpModel("Data/Model/Filde/Filde.mdl", nullptr);
		actor->SetName("Filde");
		actor->SetPosition(DirectX::XMFLOAT3(0, 0, 0));
		actor->SetAngle(DirectX::XMFLOAT3( 0, Mathf::ConvartToRadian(-90), 0));
		actor->SetScale(DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f));
		actor->SetShaderType(ShaderManager::ShaderType::CascadeShadowMap);
	}

	// ステージオブジェクトの配置
	StageManager::Instance().WorldObjectRegister();

	// プレイヤー読み込み
	{
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetUpModel("Data/Model/RPG-Character/WorldMapPlayer.mdl", "Motion");
		actor->SetName("Player");
		actor->SetPosition(DirectX::XMFLOAT3(-100, 6, 116));
		actor->SetAngle(DirectX::XMFLOAT3(0, 0, 0));
		actor->SetScale(DirectX::XMFLOAT3(0.04f, 0.04f, 0.04f));
		// コンポーネント追加
		actor->AddComponent<Movement>();
		actor->AddComponent<Charactor>(static_cast<int>(MetaAI::Identity::Player));
		actor->AddComponent<Player>();
		actor->AddComponent<PlayerCollision>();
		// プレイヤーのカリングコリジョンを追加
		CollisionManager::Instance().RegisterCulling(std::make_shared<CullingCollision>(ActorType::None, actor));
		actor->SetShaderType(ShaderManager::ShaderType::Phong);
	}
	// 敵の生成
	MetaAI::Instance().AppearanceEnemy();

	// アクター更新処理
	ActorManager::Instance().Update(0.01f);
	ActorManager::Instance().UpdateTransform();

	// BGM再生
	AudioManager::Instance().PlayBGM(BGMType::WorldMap);
	AudioManager::Instance().SetBGMVolume(BGMType::WorldMap, 0.6f);
}

//-------------------------------------
// 終了処理
//-------------------------------------
void SceneGame::Finalize()
{
	// 敵マネージャーのクリア
	EnemyManager::Instance().AllRemove();

	// UIの全破棄
	UIManager::Instance().AllDelete();

	// アクターの破棄
	ActorManager::Instance().AllDestroy();

	// コリジョンの破棄
	CollisionManager::Instance().Destroy();

	// メッセンジャーのクリア
	Messenger::Instance().Clear();

	// BGM再生停止
	AudioManager::Instance().StopBGM(BGMType::WorldMap);
}

//-------------------------------------
// 更新処理
//-------------------------------------
void SceneGame::Update(float elapsed_time)
{
	//	メニューオープン中はメニューを更新する
	MenuSystem::Instance().Update(elapsed_time);

	//	メニューオープン中であればゲーム側の更新を行わない
	if (MenuSystem::Instance().IsOpened())
	{
		return;
	}
	else
	{
		//	メニューオープン
		GamePad& gamePad = Input::Instance().GetGamePad();
		if (gamePad.GetButtonDown() & GamePad::BTN_START)
		{
			MessageData::MenuOpenData data;
			data.menu_id = MenuId::TopMenu;
			Messenger::Instance().SendData(MessageData::MENUOPENEVENT, &data);
		}
	}

	// プリミティブコンテキストのコンストラクタ更新
	if (primitive_context->timer < Primitive_Max_Time)
	{
		primitive_context->timer++;
	}

	// バトルシーンへの遷移フラグが立っている
	if (battle_flag)
	{
		// タイマーが一定以上なら
		if (primitive_context->timer >= Primitive_Max_Time)
		{
			// バトルシーンへ遷移
			SceneManager::Instance().ChangeScene(new SceneBattle());
			return;
		}
		else
		{
			return;
		}
	}

	// プリミティブ用のタイマーが一定以上なら
	if (primitive_context->timer >= Primitive_Max_Time)
	{
		// プリミティブプラグをfalseに
		primitive_falg = false;
	}

	// 当たり判定更新処理
	CollisionManager::Instance().Update();

	// アクター更新処理
	ActorManager::Instance().Update(elapsed_time);
	ActorManager::Instance().UpdateTransform();

	// UI更新処理
	UIManager::Instance().Update(elapsed_time);

	// カメラ更新処理
	camera_controller->Update(elapsed_time);
}

//-------------------------------------
// 描画処理	
//-------------------------------------
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* context = graphics.GetDeviceContext();

	// スクリーンサイズ取得
	DirectX::XMFLOAT2 screen_size = { graphics.GetScreenWidth(), graphics.GetScreenHeight() };

	// 描画処理
	render_context->light_direction = Light::LightDir;
	render_context->ShadowParameter = { shadow_color.x, shadow_color.y, shadow_color.z, 0.001f };

	// カメラパラメータ設定
	Camera& camera = Camera::Instance();
	render_context->view = camera.GetView();
	render_context->projection = camera.GetProjection();

	// ポストテクスチャ描画
	PostRender(context, render_context.get(), screen_size);

	// スクリーンテクスチャに描画
	ScreenRender(context, render_context.get(), screen_size);

	// バックバッファに描画
	BuckBufferRender(context, render_context.get(), screen_size);
}

//-------------------------------------
// スクリーンテクスチャに描画
//-------------------------------------
void SceneGame::ScreenRender(ID3D11DeviceContext* context, RenderContext* render_context, const DirectX::XMFLOAT2& screen_size)
{
	Graphics& graphics = Graphics::Instance();
	ShaderManager& shader_manager = ShaderManager::Instance();

	// スクリーンテクスチャをレンダーターゲットに設定して画面クリア
	ID3D11RenderTargetView* screen_texture = graphics.GetTexture()->GetRenderTargetView();
	ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();
	{
		graphics.SetRenderTargetView(&screen_texture, depth_stencil_view);
		graphics.ScreenClear(&screen_texture, depth_stencil_view);
	}
	// ビューポートを元に戻す
	graphics.SetViewport(screen_size.x, screen_size.y);

	// スカイボックス描画
	{
		std::shared_ptr<Shader> skybox_shader = shader_manager.GetShader(ShaderManager::ShaderType::SkyBox);
		skybox_shader->Begin(context, render_context);
		sprite->Render(context, sky.get(),
			0, 0,
			screen_size.x, screen_size.y,
			0, 0,
			(float)sky->GetWidth(), (float)sky->GetHeight());
		//ブレンドステート設定
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

	// アクター描画
	{
		// シャドウマップ作成
		ActorManager::Instance().ShadowRender(render_context, blur_render_context.get());
		
		// レンダーターゲットの回復
		graphics.SetRenderTargetView(&screen_texture, depth_stencil_view);

		// ビューポートの設定
		graphics.SetViewport(screen_size.x, screen_size.y);

		// 描画
		ActorManager::Instance().Render(render_context);
	}
}

//-------------------------------------
// ポストエフェクトに使うテクスチャ描画
//-------------------------------------
void SceneGame::PostRender(ID3D11DeviceContext* context, RenderContext* render_context, const DirectX::XMFLOAT2& screen_size)
{
	ShaderManager& shader_manager = ShaderManager::Instance();
	if(!sky_bloom_flag)
	{
		// ブルームシェーダー取得
		std::shared_ptr<Shader> bloom_shader = shader_manager.GetShader(ShaderManager::ShaderType::Bloom);
		// 空のテクスチャのブルーム処理
		bloom_texture = dynamic_cast<Bloom*>(bloom_shader.get())->Render(context, render_context, sky.get());
		// 空のブルームを作成したのでフラグを立てて描画しないようにする
		sky_bloom_flag = true;
	}
}

//-------------------------------------
// バックバッファに描画
//-------------------------------------
void SceneGame::BuckBufferRender(ID3D11DeviceContext* context, RenderContext* render_context, const DirectX::XMFLOAT2& screen_size)
{
	Graphics& graphics = Graphics::Instance();
	ShaderManager& shader_manager = ShaderManager::Instance();
	// スプライトシェーダー取得
	std::shared_ptr<Shader> sprite_shader = shader_manager.GetShader(ShaderManager::ShaderType::Sprite);

	// レンダーターゲット設定
	{
		ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
		ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();
		graphics.SetRenderTargetView(&render_target_view, depth_stencil_view);
		graphics.ScreenClear(&render_target_view, depth_stencil_view);
	}
	// ビューポート設定
	graphics.SetViewport(screen_size.x, screen_size.y);

	// バックバッファにスクリーンテクスチャを描画
	sprite_shader->Begin(context);
	// スクリーンテクスチャ
	sprite->Render(context, graphics.GetTexture(),
		0, 0,
		screen_size.x, screen_size.y,
		0, 0,
		(float)graphics.GetTexture()->GetWidth(), (float)graphics.GetTexture()->GetHeight(),
		0,
		1, 1, 1, 1);

	// メニュー描画
	{
		if (MenuSystem::Instance().IsOpened())
		{
			MenuSystem::Instance().Render(context);
		}
	}
	sprite_shader->End(context);

	// 2Dプリミティブ描画
	{
		if (primitive_falg)
		{
			// ブルームシェーダー取得
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
// メッセージ受信処理
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

		// エンカウントした敵のテリトリーに属する敵を探す
		GameDataBase::Instance().EnemyFriendFromTerritory(telegram.message_box.territory_tag);
		
		// プレイヤーコンポーネント取得
		std::shared_ptr<Actor> player = ActorManager::Instance().GetActor("Player");

		// エンカウント時のカメラモーション
		DirectX::XMFLOAT3 start_position = Camera::Instance().GetEye();
		MessageData::CameraChangeMotionModeData motioncamera_data;
		// オイラー角・座標取得
		DirectX::XMFLOAT3 angle = player->GetAngle();
		DirectX::XMFLOAT3 position = player->GetPosition();
		// プレイヤーの前方向算出
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
// GUI描画
//-------------------------------------
void SceneGame::OnGui()
{
	ImGui::SetNextWindowPos(ImVec2(550, 50), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	ImGui::Begin("Shader", nullptr, ImGuiWindowFlags_None);
	ImGui::TextColored(ImVec4(1, 1, 0, 1), u8"-------シャドウマップ表示フラグ-------");
	ImGui::Checkbox("ShadowmapDrawFlag", &isshadowmap);

	ImGui::TextColored(ImVec4(1, 1, 0, 1), u8"-------2Dプリミティブ-------");
	ImGui::SliderInt("PrimitiveType", &primitive_context->number, 1, 10);
	ImGui::InputFloat("Timer", &primitive_context->timer);
	ImGui::Checkbox("PrimitiveFlag", &primitive_falg);


	ImGui::End();
}

