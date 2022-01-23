#include <imgui.h>
#include "SceneGame.h"
#include "SceneBattle.h"
#include "SceneManager.h"
#include "Mathf.h"

// データ系
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

	// プリミティブのコンスタントバッファの初期設定
	primitive_falg = true;
	primitive_context.number = 2;
	primitive_context.timer = 0.0f;

	// シーン名設定
	SetName("SceneWorldMap");

	// データベース初期化
	GameDataBase::Instance();
	
	// ライト初期化
	Light::Initialize();

	// カメラ初期設定
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

	// カメラコントローラー初期化
	camera_controller = std::make_unique<CameraController>();
	camera_controller->SetCameraAngle({ Mathf::ConvartToRadian(25), 0.0f, 0.0f });
	camera_controller->SetTarget(DirectX::XMFLOAT3(-400, 16, -416));

	// シェーダー初期化
	bloom = std::make_unique<Bloom>(device);

	// テクスチャ作成
	bulr_texture = std::make_unique<Texture>();
	bulr_texture->Create((u_int)graphics.GetScreenWidth(), (u_int)graphics.GetScreenHeight(), DXGI_FORMAT_R16G16B16A16_FLOAT);

	depth_texture = std::make_unique<Texture>();
	depth_texture->CreateDepthStencil((u_int)2048, (u_int)1024);

	// テクスチャの読み込み
	sprite = std::make_unique<Sprite>();
	sky = std::make_unique<Texture>();
	sky->Load("Data/Sprite/SkyBox/FS002_Night.png");
	sky_texture = std::make_unique<Texture>();
	sky_texture->Create(static_cast<float>(sky->GetWidth()), static_cast<float>(sky->GetHeight()), DXGI_FORMAT_R16G16B16A16_FLOAT);

	// ステージ読み込み
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
	// プレイヤー読み込み
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
	// 敵マネージャーのクリア
	EnemyManager::Instance().AllRemove();

	// アクターの破棄
	ActorManager::Instance().AllDestroy();

	// コリジョンの破棄
	CollisionManager::Instance().Destroy();

	// メッセンジャーのクリア
	Messenger::Instance().Clear();	

}

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
	if (primitive_context.timer < Primitive_Max_Time)
	{
		primitive_context.timer++;
	}

	// バトルシーンへの遷移フラグが立っている
	if (battle_flag)
	{
		// タイマーが一定以上なら
		if (primitive_context.timer >= Primitive_Max_Time)
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
	if (primitive_context.timer >= Primitive_Max_Time)
	{
		// プリミティブプラグをfalseに
		primitive_falg = false;
	}

	//ライト
	static float light_angle = DirectX::XM_PI;
	if (GetKeyState('E') < 0) light_angle += elapsed_time * 2.0f;
	if (GetKeyState('Q') < 0) light_angle -= elapsed_time * 2.0f;

	Light::SetAmbient(DirectX::XMFLOAT3(0.2f, 0.2f, 0.2f));
	//ライト方向
	LightDir.x = sinf(light_angle);
	LightDir.y = -1.0f;
	LightDir.z = cosf(light_angle);
	Light::SetDirLight(LightDir, DirectX::XMFLOAT3(0.6f, 0.6f, 0.6f));
	

	// メタAIの更新処理
	MetaAI::Instance().Update(elapsed_time);

	// 当たり判定更新処理
	CollisionManager::Instance().Update();

	// アクター更新処理
	ActorManager::Instance().Update(elapsed_time);
	ActorManager::Instance().UpdateTransform();

	// UI更新処理
	UIManager::Instance().Update(elapsed_time);

	// カメラ更新処理
	std::shared_ptr<Actor> actor = ActorManager::Instance().GetActor("Player");
	camera_controller->SetTarget({
		actor->GetPosition().x,
		actor->GetPosition().y + 1.0f,
		actor->GetPosition().z });
	camera_controller->Update(elapsed_time);
}

// 描画処理	
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* context = graphics.GetDeviceContext();
	// スクリーンサイズ取得
	DirectX::XMFLOAT2 screen_size = { graphics.GetScreenWidth(), graphics.GetScreenHeight() };

	// 描画処理
	render_context.light_direction = Light::LightDir;
	render_context.ShadowParameter = { shadow_color.x, shadow_color.y, shadow_color.z, 0.001f };

	// カメラパラメータ設定
	Camera& camera = Camera::Instance();
	render_context.view = camera.GetView();
	render_context.projection = camera.GetProjection();

	// ポストエフェクト用のテクスチャ描画
	PostRender(context, render_context, screen_size);

	// スクリーンテクスチャに描画
	ScreenRender(context, render_context, screen_size);

	// バックバッファに描画
	BuckBufferRender(context, render_context, screen_size);

	// GUI描画
	OnGui();
}

//-------------------------------------
// スクリーンテクスチャに描画
//-------------------------------------
void SceneGame::ScreenRender(ID3D11DeviceContext* context, RenderContext& render_context, const DirectX::XMFLOAT2& screen_size)
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

	// デバッグプリミティブ描画
	{
		// 敵縄張りのデバッグプリミティブ描画
		EnemyTerritoryManager::Instance().Render();
		// 敵のデバッグプリミティブ描画
		EnemyManager::Instance().DrawDebugPrimitive();
		// 当たり判定ののデバッグプリミティブ描画
		CollisionManager::Instance().Draw();
		for (int i = 0; i < 4; ++i)
		{
			graphics.GetDebugRenderer()->DrawSphere(camera_controller->far_position[i], 0.4f, color);
			graphics.GetDebugRenderer()->DrawSphere(camera_controller->near_position[i], 0.4f, color);
		}
		graphics.GetDebugRenderer()->Render(context, render_context.view, render_context.projection);
	}

	// アクター描画
	{
		// シャドウマップ作成
		ActorManager::Instance().ShadowRender(render_context, blur_render_context);
		
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
void SceneGame::PostRender(ID3D11DeviceContext* context, RenderContext& render_context, const DirectX::XMFLOAT2& screen_size)
{
	Graphics& graphics = Graphics::Instance();
	ShaderManager& shader_manager = ShaderManager::Instance();
	std::shared_ptr<Shader> bloom_shader = shader_manager.GetShader(ShaderManager::ShaderType::Bloom);

	// 空のテクスチャのブルーム処理
	bloom_texture = dynamic_cast<Bloom*>(bloom_shader.get())->Render(context, render_context, sky.get());

	// レンダーターゲット設定
	{
		ID3D11RenderTargetView* render_target_view = sky_texture->GetRenderTargetView();
		ID3D11DepthStencilView* depth_stencil_view = depth_texture->GetDepthStencilView();
		graphics.SetRenderTargetView(&render_target_view, depth_stencil_view);
		graphics.ScreenClear(&render_target_view, depth_stencil_view);
	}
	// ビューポート設定
	graphics.SetViewport(sky_texture->GetWidth(), sky_texture->GetHeight());

	// 空テクスチャと空テクスチャの輝度を抽出したテクスチャを加算合成
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
	//// ビューポートの設定
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
// バックバッファに描画
//-------------------------------------
void SceneGame::BuckBufferRender(ID3D11DeviceContext* context, RenderContext& render_context, const DirectX::XMFLOAT2& screen_size)
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
	// 輝度抽出テクスチャを加算合成
	//context->OMSetBlendState(graphics.GetBlendState((int)Graphics::BlendState::Add), nullptr, 0xFFFFFFFF);
	//sprite->AddRender(context,
	//	bloom_texture,
	//	0, 0,
	//	screen_size.x, screen_size.y,
	//	0, 0,
	//	(float)bloom_texture->GetWidth(), (float)bloom_texture->GetHeight());

	//// ブレンドステートをアルファ加算合成に設定
	//context->OMSetBlendState(graphics.GetBlendState((int)Graphics::BlendState::Alpha), nullptr, 0xFFFFFFFF);
	
	// メニュー描画
	{
		if (MenuSystem::Instance().IsOpened())
		{
			MenuSystem::Instance().Render(context);
		}
	}

	// シャドウマップ
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

	// 2Dプリミティブ描画
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
// メッセージ受信処理
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

		// TODO エンカウントした敵のテリトリーに属ずる敵を探す
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
	ImGui::SliderInt("PrimitiveType", &primitive_context.number, 1, 10);
	ImGui::InputFloat("Timer", &primitive_context.timer);
	ImGui::Checkbox("PrimitiveFlag", &primitive_falg);


	ImGui::End();
}

