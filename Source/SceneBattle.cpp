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

	// シーン名設定
	SetName("SceneBattle");

	// プリミティブのコンスタントバッファの初期設定
	primitive_falg = true;
	primitive_context.number = 2;
	primitive_context.timer = 0.0f;

	// ライト初期化
	Light::Initialize();

	// カメラ初期設定
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

	// カメラコントローラー初期化
	camera_controller = std::make_unique<CameraController>();
	camera_controller->SetCameraAngle({ Mathf::ConvartToRadian(25), 0.0f, 0.0f });

	// シェーダー初期化
	bloom = std::make_unique<Bloom>(device);
	primitive = std::make_unique<Primitive>(device);

	// テクスチャの読み込み
	sprite = std::make_unique<Sprite>();
	sky = std::make_unique<Texture>();
	sky->Load("Data/Sprite/SkyBox/FS002_Night.png");
	anybutton_texture = std::make_unique<Texture>();
	anybutton_texture->Load("Data/Sprite/PushAnyButton.png");
	clear_texture = std::make_unique<Texture>();
	clear_texture->Load("Data/Sprite/GameClear.png");
	over_texture = std::make_unique<Texture>();
	over_texture->Load("Data/Sprite/YouDead.png");
	// ステージ読み込み
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

	// プレイヤー読み込み
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
	
	// スクリプトから敵の生成
	EnemyManager::Instance().CreateEnemyEncountData();
}

void SceneBattle::Finalize()
{
	// UIの全破棄
	UIManager::Instance().AllDelete();
	
	// 敵マネージャーのクリア
	EnemyManager::Instance().AllRemove();

	// アクターの破棄
	ActorManager::Instance().AllDestroy();

	// コリジョンの破棄
	CollisionManager::Instance().Destroy();

	// メッセンジャーのクリア
	Messenger::Instance().Clear();
}

void SceneBattle::Update(float elapsed_time)
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
	// ゲームがクリアかゲームオーバーになっていれば
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

	// プリミティブコンテキストのコンストラクタ更新
	if (primitive_context.timer < 40)
	{
		primitive_context.timer++;
	}
	if (primitive_context.timer >= 40)
	{
		primitive_falg = false;
	}

	//ライト
	static float light_angle = DirectX::XM_PI;
	if (GetKeyState('E') < 0) light_angle += elapsed_time * 2.0f;
	if (GetKeyState('Q') < 0) light_angle -= elapsed_time * 2.0f;

	//ライト方向
	LightDir.x = sinf(light_angle);
	LightDir.y = -1.0f;
	LightDir.z = cosf(light_angle);
	Light::SetDirLight(LightDir, DirectX::XMFLOAT3(0.6f, 0.6f, 0.6f));
	Light::SetAmbient(DirectX::XMFLOAT3(0.2f, 0.2f, 0.2f));

	// アクター更新処理
	ActorManager::Instance().Update(elapsed_time);
	ActorManager::Instance().UpdateTransform();

	// 当たり判定更新処理
	CollisionManager::Instance().Update();

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

void SceneBattle::Render()
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

	// バックバッファのクリア処理
	{
		ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
		ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();
		// 画面クリア
		graphics.ScreenClear(&render_target_view, depth_stencil_view);
	}

	// スクリーンテクスチャに描画
	ScreenRender(context, render_context, screen_size);

	// ポストテクスチャ描画
	PostRender(context, render_context, screen_size);

	// バックバッファに描画
	BuckBufferRender(context, render_context, screen_size);
}


//-------------------------------------
// スクリーンテクスチャ描画
//-------------------------------------
void SceneBattle::ScreenRender(ID3D11DeviceContext* context, RenderContext& render_context, const DirectX::XMFLOAT2& screen_size)
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

	// デバッグプリミティブ描画
	{
		// 敵縄張りのデバッグプリミティブ描画
		EnemyTerritoryManager::Instance().Render();
		EnemyManager::Instance().DrawDebugPrimitive();
		CollisionManager::Instance().Draw();
		graphics.GetDebugRenderer()->Render(context, render_context.view, render_context.projection);
	}

	// アクター描画
	{
		// シャドウマップ作成
		ActorManager::Instance().ShadowRender(render_context, blur_render_context);

		// レンダーターゲットの回復
		graphics.SetRenderTargetView(&screen_texture, depth_stencil_view);

		// ビューポートの設定
		graphics.SetViewport(graphics.GetScreenWidth(), graphics.GetScreenHeight());

		// 描画
		ActorManager::Instance().Render(render_context);
	}
}

//-------------------------------------
// ポストテクスチャ描画
//-------------------------------------
void SceneBattle::PostRender(ID3D11DeviceContext* context, RenderContext& render_context, const DirectX::XMFLOAT2& screen_size)
{
	bloom_texture = bloom->Render(context, render_context);
}

//-------------------------------------
// バックバッファ描画
//-------------------------------------
void SceneBattle::BuckBufferRender(ID3D11DeviceContext* context, RenderContext& render_context, const DirectX::XMFLOAT2& screen_size)
{
	Graphics& graphics = Graphics::Instance();
	ShaderManager& shader_manager = ShaderManager::Instance();

	// レンダーターゲット設定
	{
		ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
		ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();
		graphics.SetRenderTargetView(&render_target_view, depth_stencil_view);
	}
	// ビューポート設定
	graphics.SetViewport(screen_size.x, screen_size.y);

	//バックバッファにスクリーンテクスチャを描画
	std::shared_ptr<Shader> sprite_shader = shader_manager.GetShader(ShaderManager::ShaderType::Sprite);
	sprite_shader->Begin(context);
	// スクリーンテクスチャ
	sprite->Render(context, graphics.GetTexture(),
		0, 0,
		screen_size.x, screen_size.y,
		0, 0,
		(float)graphics.GetTexture()->GetWidth(), (float)graphics.GetTexture()->GetHeight(),
		0,
		1, 1, 1, 1);
	sprite_shader->End(context);

	// メニュー描画
	{
		if (MenuSystem::Instance().IsOpened())
		{
			sprite_shader->Begin(context);
			MenuSystem::Instance().Render(context);
			sprite_shader->End(context);
		}
	}

	// UI描画処理
	UIManager::Instance().Draw(context);

	// シャドウマップ
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

	// 2Dプリミティブ描画
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
// メッセージ処理
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
// GUI描画
//-------------------------------------
void SceneBattle::OnGui()
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

//-------------------------------------
// クリアかゲームオーバー描画
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
// ゲームクリア判定処理
//-------------------------------------
bool SceneBattle::IsGameClearJudgment()
{
	// 撃破フラグを格納したコンテナ取得
	std::map<EnemyTerritoryTag, bool> defeat_teritory = EnemyManager::Instance().GetDefeatTeritory();
	
	// ドラゴンを全て撃破したらクリア
	return (defeat_teritory[EnemyTerritoryTag::ButtlePosition4] == true &&
		defeat_teritory[EnemyTerritoryTag::ButtlePosition5] == true);
}
