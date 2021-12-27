#include <imgui.h>
#include "SceneGame.h"
#include "SceneBattle.h"
#include "SceneManager.h"

// データ系
#include "GameDataBase.h"
#include "Script.h"

#include "Graphics.h"
#include "Camera.h"
#include "CameraController.h"
#include "Template.h"
#include "Light.h"
#include "Input.h"

#include "ActorManager.h"
#include "Actor.h"
#include "EnemyManager.h"
#include "Player.h"
#include "EnemySlime.h"
#include "Stage.h"
#include "Movement.h"
#include "Collision.h"


#include "CascadeShadowMapShader.h"
#include "LambertShader.h"
#include "BloomShader.h"
#include "PhongShader.h"
#include "2DPrimitive.h"


#include "MenuSystem.h"
#include "Messenger.h"
#include "MessageData.h"
#include "UI.h"
#include "MetaAI.h"
#include "Texture.h"
#include "Sprite.h"

#include "EnemyTerritoryManager.h"

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
		DirectX::XMFLOAT3(-400, 16, -416),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0)
	);
	camera.SetPerspectiveFov(DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.01f,
		2000.0f);

	// カメラコントローラー初期化
	camera_controller = std::make_unique<CameraController>();
	camera_controller->SetCameraAngle({ DirectX::XMConvertToRadians(25), 0.0f, 0.0f });
	camera_controller->SetTarget(DirectX::XMFLOAT3(-400, 16, -416));

	// シェーダー初期化
	bloom = std::make_unique<Bloom>(device);
	primitive = std::make_unique<Primitive>(device);

	// テクスチャ作成
	bulr_texture = std::make_unique<Texture>();
	bulr_texture->Create((u_int)graphics.GetScreenWidth(), (u_int)graphics.GetScreenHeight(), DXGI_FORMAT_R16G16B16A16_FLOAT);
	depth_texture = std::make_unique<Texture>();
	depth_texture->CreateDepthStencil((u_int)graphics.GetScreenWidth(), (u_int)graphics.GetScreenHeight());

	// テクスチャの読み込み
	sprite = std::make_unique<Sprite>();
	sky = std::make_unique<Texture>();
	sky->Load("Data/Sprite/SkyBox/FS002_Night.png");

	// ステージ読み込み
	{
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetUpModel("Data/Model/Filde/Filde.mdl");
		actor->SetName("Filde");
		actor->SetPosition(DirectX::XMFLOAT3(0, 0, 0));
		actor->SetAngle(DirectX::XMFLOAT3( 0, DirectX::XMConvertToRadians(-90), 0));
		actor->SetScale(DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f));
		//actor->SetScale(DirectX::XMFLOAT3(5.f, 5.f, 5.f));
		actor->AddComponent<Stage>();
		actor->AddShader<CascadeShadowMap>(device);
	}
	{
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetUpModel("Data/Model/Filde/StageObjects.mdl");
		actor->SetName("FildeObjects");
		actor->SetPosition(DirectX::XMFLOAT3(0, 0, 0));
		actor->SetAngle(DirectX::XMFLOAT3(0, DirectX::XMConvertToRadians(-90), 0));
		actor->SetScale(DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f));
		actor->AddComponent<Stage>();
		actor->AddShader<LambertShader>(device);
	}
	// プレイヤー読み込み
	{
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetUpModel("Data/Model/RPG-Character/WorldMapPlayer.mdl");
		actor->SetName("Player");
		actor->SetAnimationNodeName("Motion");
		actor->SetPosition(DirectX::XMFLOAT3(-100, 6, -116));
		actor->SetAngle(DirectX::XMFLOAT3(0, 0, 0));
		actor->SetScale(DirectX::XMFLOAT3(0.04f, 0.04f, 0.04f));
		actor->AddComponent<Movement>();
		actor->AddComponent<Charactor>();
		actor->AddComponent<Player>();
		actor->AddShader<LambertShader>(device);
	}
	ActorManager::Instance().Update(0.01f);
	ActorManager::Instance().UpdateTransform();
	MetaAI::Instance();
}

void SceneGame::Finalize()
{
	// 敵マネージャーのクリア
	EnemyManager::Instance().AllRemove();

	// アクターの破棄
	ActorManager::Instance().AllDestroy();

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
	// バトルシーンへの遷移フラグが立っているかつタイマーが一定以上なら
	if (battle_flag && primitive_context.timer >= 40)
	{
		// バトルシーンへ遷移
		SceneManager::Instance().ChangeScene(new SceneBattle());
	}
	// プリミティブコンテキストのコンストラクタ更新
	if (primitive_context.timer < 40)
	{
		primitive_context.timer++;
	}
	if (battle_flag) return;

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
	
	// カメラ更新処理
	std::shared_ptr<Actor> actor = ActorManager::Instance().GetActor("Player");
	camera_controller->SetTarget({
		actor->GetPosition().x,
		actor->GetPosition().y + 1.0f,
		actor->GetPosition().z });
	camera_controller->Update(elapsed_time);

	// メタAIの更新処理
	MetaAI::Instance().Update(elapsed_time);

	// アクター更新処理
	ActorManager::Instance().Update(elapsed_time);
	ActorManager::Instance().UpdateTransform();

	// UI更新処理
	UIManager::Instance().Update(elapsed_time);

	// 当たり判定更新処理
	CollisionManager::Instance().Update();
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

	// スクリーンテクスチャに描画
	ScreenRender(context, render_context, screen_size);

	// ポストエフェクト用のテクスチャ描画
	PostRender(context, render_context, screen_size);

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
		graphics.GetSkyBoxShader()->Begin(context, render_context);
		sprite->Render(context,
			sky.get(),
			0, 0,
			screen_size.x, screen_size.y,
			0, 0,
			static_cast<float>(sky->GetWidth()), static_cast<float>(sky->GetHeight()),
			0,
			1, 1, 1, 1);
		graphics.GetSkyBoxShader()->End(context);
	}

	// デバッグプリミティブ描画
	{
		// 敵縄張りのデバッグプリミティブ描画
		EnemyTerritoryManager::Instance().Render();
		// 敵のデバッグプリミティブ描画
		EnemyManager::Instance().DrawDebugPrimitive();
		// 当たり判定ののデバッグプリミティブ描画
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
	bloom_texture = bloom->Render(context, render_context);


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
	graphics.GetSpriteShader()->Begin(context);
	// スクリーンテクスチャ
	sprite->Render(context, graphics.GetTexture(),
		0, 0,
		screen_size.x, screen_size.y,
		0, 0,
		(float)graphics.GetTexture()->GetWidth(), (float)graphics.GetTexture()->GetHeight(),
		0,
		1, 1, 1, 1);
	// 輝度抽出テクスチャを加算合成
	context->OMSetBlendState(graphics.GetBlendState((int)Graphics::BlendState::Add), nullptr, 0xFFFFFFFF);
	sprite->AddRender(context,
		bloom_texture,
		0, 0,
		screen_size.x, screen_size.y,
		0, 0,
		(float)bloom_texture->GetWidth(), (float)bloom_texture->GetHeight());

	// ブレンドステートをアルファ加算合成に設定
	context->OMSetBlendState(graphics.GetBlendState((int)Graphics::BlendState::Alpha), nullptr, 0xFFFFFFFF);
	
	// メニュー描画
	{
		if (MenuSystem::Instance().IsOpened())
		{
			MenuSystem::Instance().Render(context);
		}
	}
	graphics.GetSpriteShader()->End(context);

	// シャドウマップ
	graphics.GetSpriteShader()->Begin(context);
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
	sprite->Render(context,
		bloom_texture,
		0, 200,
		200, 200,
		0, 0,
		(float)bloom_texture->GetWidth(), (float)bloom_texture->GetHeight(),
		0,
		1, 1, 1, 1);
	graphics.GetSpriteShader()->End(context);

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

		BattleSceneDataHeadder headder;
		// ある地点と敵座標との距離を計算して一定範囲内の距離ならIDを追加
		DistanceBetweenEnemyAndPoint(headder, DirectX::XMLoadFloat3(&telegram.message_box.hit_position), enemy_search_range);		
		
		// スクリプトにデータを書き込む
		WriteScript::Instance().WriteSceneDataScript("./Data/Script/SendBattleSceneScript.txt", headder);		
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
	ImGui::TextColored(ImVec4(1, 1, 0, 1), u8"-------シャドウマップ用ブラー-------");
	ImGui::SliderInt("KarnelSize", &blur_render_context.karnel_size, 1, 15);

	ImGui::TextColored(ImVec4(1, 1, 0, 1), u8"-------シャドウマップ-------");
	ImGui::ColorEdit3("ShadowColor", (float*)&shadow_color);

	ImGui::TextColored(ImVec4(1, 1, 0, 1), u8"-------2Dプリミティブ-------");
	ImGui::SliderInt("PrimitiveType", &primitive_context.number, 1, 10);
	ImGui::InputFloat("Timer", &primitive_context.timer);
	ImGui::Checkbox("PrimitiveFlag", &primitive_falg);
	ImGui::End();
}

//------------------------------------------------------------------
// ある地点と敵座標との距離を計算して一定範囲内の距離ならIDを追加
//------------------------------------------------------------------
void SceneGame::DistanceBetweenEnemyAndPoint(BattleSceneDataHeadder& data_headder, const DirectX::XMVECTOR& origin, const float& range)
{
	int enemy_count = EnemyManager::Instance().GetEnemyCount();

	for (int i = 0; i < enemy_count; ++i)
	{
		std::shared_ptr<Enemy> enemy = EnemyManager::Instance().GetEnemy(i);
		DirectX::XMVECTOR enemy_position = DirectX::XMLoadFloat3(&enemy->GetActor()->GetPosition());
		
		// 原点から敵座標のベクトル算出
		DirectX::XMVECTOR distance = DirectX::XMVectorSubtract(enemy_position, origin);

		// ベクトルの大きさから距離を算出
		DirectX::XMVECTOR length = DirectX::XMVector3Length(distance);
		float float_length;
		DirectX::XMStoreFloat(&float_length, distance);

		// 範囲内に入っていれば敵データIDを追加
		if(fabsf(float_length) <= range)
		{ 
			data_headder.search_enemy_id.emplace_back(enemy->GetActor()->GetEnemyDataID());
		}
	}
}
