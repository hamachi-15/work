#include <imgui.h>
#include <stdio.h>
// シーン
#include "SceneBattle.h"
#include "SceneGame.h"
#include "SceneOver.h"
#include "SceneClear.h"

// 算術関数
#include "Mathf.h"

// 描画
#include "Graphics.h"
#include "Light.h"

// カメラ
#include "Camera.h"
#include "CameraController.h"

// 入力
#include "Input.h"

// コンポーネント
#include "Movement.h"
#include "Actor.h"
#include "EnemySlime.h"
#include "Player.h"
#include "Stage.h"

// シェーダー
#include "BloomShader.h"

// マネージャー
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

// メニュー
#include "MenuSystem.h"

// メッセージ
#include "Messenger.h"
#include "MessageData.h"

// ゲームデータ
#include "GameDataBase.h"

// テクスチャ
#include "Texture.h"
#include "Sprite.h"

// AI
#include "MetaAI.h"

// UI
#include "PlayerUIHealth.h"
#include "PlayerCollision.h"


//---------------------------------
// コンストラクタ
//---------------------------------
SceneBattle::SceneBattle()
{
}

//---------------------------------
// デストラクタ
//---------------------------------
SceneBattle::~SceneBattle()
{
}

//---------------------------------
// 開始処理
//---------------------------------
void SceneBattle::Initialize()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11Device* device = graphics.GetDevice();

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

	// テクスチャの読み込み
	sprite = std::make_unique<Sprite>();
	sky = ResourceManager::Instance().LoadTexture("Data/Sprite/SkyBox/FS002_Night.png");
	anybutton_texture = ResourceManager::Instance().LoadTexture("Data/Sprite/PushAnyButton.png");

	// ステージ読み込み
	{
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetUpModel("Data/Model/Filde/Filde.mdl", nullptr);
		actor->SetName("Filde");
		actor->SetPosition(DirectX::XMFLOAT3(0, 0, 0));
		actor->SetAngle(DirectX::XMFLOAT3(0, Mathf::ConvartToRadian(-90), 0));
		actor->SetScale(DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f));
		actor->SetShaderType(ShaderManager::ShaderType::CascadeShadowMap);
	}
	
	// ステージオブジェクトの配置
	StageManager::Instance().ButtleObjectRegister();

	// プレイヤー読み込み
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
	
	// エンカウントデータから敵の生成
	EnemyManager::Instance().CreateEnemyEncountData();

	// BGM再生
	AudioManager::Instance().PlayBGM(BGMType::ButtleMap);
	AudioManager::Instance().SetBGMVolume(BGMType::ButtleMap, 0.5f);
}

//---------------------------------
// 終了処理
//---------------------------------
void SceneBattle::Finalize()
{
	// 敵マネージャーのクリア
	EnemyManager::Instance().AllRemove();

	// UIの全破棄
	UIManager::Instance().AllDelete();

	// アクターの破棄
	ActorManager::Instance().AllDestroy();

	// コリジョンの破棄
	CollisionManager::Instance().Destroy();

	// 火球マネージャー破棄
	FireBallManager::Instance().Destroy();

	// メッセンジャーのクリア
	Messenger::Instance().Clear();

	// BGM再生停止
	AudioManager::Instance().StopBGM(BGMType::ButtleMap);
}

//---------------------------------
// 更新処理
//---------------------------------
void SceneBattle::Update(float elapsed_time)
{
	// グラフィクス取得
	Graphics& graphics = Graphics::Instance();

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
	// ゲームがクリアになっていれば
	if (isgame_clear)
	{		
		// クリアシーンに遷移
		SceneManager::Instance().ChangeScene(new SceneClear());
		return;
	}
	// ゲームオーバーになっていれば
	if (isgame_over)
	{		
		// ゲームオーバーシーンに遷移
		SceneManager::Instance().ChangeScene(new SceneOver());
		return;
	}
	// 戦闘が終了したら
	if (isbuttle_end)
	{		
		// ゲームシーンに遷移
		SceneManager::Instance().ChangeScene(new SceneGame());
		return;
	}

	// プリミティブコンテキストのコンストラクタ更新
	if (primitive_context->timer < 40)
	{
		primitive_context->timer++;
	}
	if (primitive_context->timer >= 40)
	{
		primitive_falg = false;
	}


	// アクター更新処理
	ActorManager::Instance().Update(elapsed_time);
	ActorManager::Instance().UpdateTransform();

	// 火球更新処理
	FireBallManager::Instance().Update(elapsed_time);
	
	//エフェクト更新処理
	graphics.GetEffectManager()->Update(elapsed_time);

	// 当たり判定更新処理
	CollisionManager::Instance().Update();

	// UI更新処理
	UIManager::Instance().Update(elapsed_time);

	// カメラ更新処理
	camera_controller->Update(elapsed_time);
}

//-------------------------------------
// 描画処理
//-------------------------------------
void SceneBattle::Render()
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

	// バックバッファのクリア処理
	{
		ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
		ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();
		// 画面クリア
		graphics.ScreenClear(&render_target_view, depth_stencil_view);
	}


	// ポストテクスチャ描画
	PostRender(context, render_context.get(), screen_size);

	// スクリーンテクスチャに描画
	ScreenRender(context, render_context.get(), screen_size);

	// バックバッファに描画
	BuckBufferRender(context, render_context.get(), screen_size);
}


//-------------------------------------
// スクリーンテクスチャ描画
//-------------------------------------
void SceneBattle::ScreenRender(ID3D11DeviceContext* context, RenderContext* render_context, const DirectX::XMFLOAT2& screen_size)
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
		shader->End(context);
	}

	// アクター描画
	{
		// シャドウマップ作成
		ActorManager::Instance().ShadowRender(render_context, blur_render_context.get());

		// レンダーターゲットの回復
		graphics.SetRenderTargetView(&screen_texture, depth_stencil_view);

		// ビューポートの設定
		graphics.SetViewport(graphics.GetScreenWidth(), graphics.GetScreenHeight());

		// 描画
		ActorManager::Instance().Render(render_context);
	}
	//エフェクト描画処理
	graphics.GetEffectManager()->Render(render_context->view, render_context->projection);

	// 火球描画処理
	FireBallManager::Instance().Render(render_context->view, render_context->projection);
}

//-------------------------------------
// ポストテクスチャ描画
//-------------------------------------
void SceneBattle::PostRender(ID3D11DeviceContext* context, RenderContext* render_context, const DirectX::XMFLOAT2& screen_size)
{
	ShaderManager& shader_manager = ShaderManager::Instance();
	if (!sky_bloom_flag)
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
// ゲームオーバー・クリアの瞬間を描画
//-------------------------------------
void SceneBattle::MomentRender(ID3D11DeviceContext* context, const DirectX::XMFLOAT2& screen_size)
{
	Graphics& graphics = Graphics::Instance();
	ShaderManager& shader_manager = ShaderManager::Instance();
	GameDataBase& database = GameDataBase::Instance();
	// レンダーターゲット設定
	{
		ID3D11RenderTargetView* render_target_view = database.GetTimingTexture()->GetRenderTargetView();
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
}

//-------------------------------------
// バックバッファ描画
//-------------------------------------
void SceneBattle::BuckBufferRender(ID3D11DeviceContext* context, RenderContext* render_context, const DirectX::XMFLOAT2& screen_size)
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
	// UI描画処理
	UIManager::Instance().Draw(context);

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
// メッセージ処理
//-------------------------------------
bool SceneBattle::OnMessages(const Telegram& telegram)
{
	Graphics& graphics = Graphics::Instance();
	switch (telegram.message_box.message)
	{
	case MessageType::Message_Buttle_End:
		// 倒した敵を記録
		EnemyManager::Instance().SetDefeatTeritory(telegram.message_box.territory_tag, true);

		// クリアしているか判定
		if (IsGameClearJudgment())
		{
			// クリアならフラグを立てる
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
		// スクリーンサイズ取得
		DirectX::XMFLOAT2 screen_size = { graphics.GetScreenWidth(), graphics.GetScreenHeight() };
		// ゲームオーバー・クリアの瞬間を描画
		MomentRender(graphics.GetDeviceContext(), screen_size);
		return true;
	}
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
	ImGui::SliderInt("PrimitiveType", &primitive_context->number, 1, 10);
	ImGui::InputFloat("Timer", &primitive_context->timer);
	ImGui::Checkbox("PrimitiveFlag", &primitive_falg);

	ImGui::End();

}

//-------------------------------------
// ゲームクリア判定処理
//-------------------------------------
bool SceneBattle::IsGameClearJudgment()
{
	// エネミーマネージャー取得
	EnemyManager& enemy_manager = EnemyManager::Instance();
	
	// ドラゴンを全て撃破したらクリア
	return (enemy_manager.GetDefeatTeritory(EnemyTerritoryTag::Territory4) == true &&
		enemy_manager.GetDefeatTeritory(EnemyTerritoryTag::Territory5) == true);
}
