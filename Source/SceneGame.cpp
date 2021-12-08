#include <imgui.h>
#include "SceneGame.h"
#include "SceneBattle.h"
#include "SceneManager.h"

#include "Graphics.h"
#include "Camera.h"
#include "CameraController.h"
#include "GameDataBase.h"
#include "Template.h"
#include "Light.h"
#include "Input.h"
#include "Actor.h"
#include "ActorManager.h"
#include "Movement.h"

#include "PhongVarianceShadowMap.h"
#include "GaussianBlurShader.h"
#include "GaussianXBlur.h"
#include "GaussianYBlur.h"
#include "ReflectSeaShader.h"
#include "UseCubeMapShader.h"
#include "LambertShader.h"

#include "EnemyManager.h"
#include "Player.h"
#include "EnemySlime.h"
#include "Stage.h"

#include "MenuSystem.h"
#include "Messenger.h"
#include "MessageData.h"
#include "Collision.h"
#include "MetaAI.h"
#include "Texture.h"
#include "Sprite.h"
#include "UI.h"

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

	//
	bulr = std::make_unique<GaussianBlur>(device, DXGI_FORMAT_R16G16B16A16_FLOAT);

	bulr_texture = std::make_unique<Texture>();
	bulr_texture->Create(graphics.GetScreenWidth(), graphics.GetScreenHeight(), DXGI_FORMAT_R16G16B16A16_FLOAT);
	depth_texture = std::make_unique<Texture>();
	depth_texture->CreateDepthStencil(graphics.GetScreenWidth(), graphics.GetScreenHeight());

	// テクスチャの読み込み
	sprite = std::make_unique<Sprite>();
	sky = std::make_unique<Texture>();
	sky->Load("Data/Sprite/SkyBox/FS002_Night.png");

	// ステージ読み込み
	{
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetUpModel("Data/Model/Filde/Stage.mdl");
		//actor->SetUpModel("Data/Model/ExampleStage/ExampleStage.mdl");
		actor->SetName("Stage");
		actor->SetPosition(DirectX::XMFLOAT3(0, 0, 0));
		actor->SetAngle(DirectX::XMFLOAT3( 0, DirectX::XMConvertToRadians(-90), 0));
		actor->SetScale(DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f));
		//actor->SetScale(DirectX::XMFLOAT3(5.f, 5.f, 5.f));
		actor->AddComponent<Stage>();
		actor->AddShader<PhongVarianceShadowMap>(device);
	}

	// プレイヤー読み込み
	{
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetUpModel("Data/Model/RPG-Character/Swordman.mdl");
		actor->SetName("Player");
		actor->SetAnimationNodeName("Motion");
		actor->SetPosition(DirectX::XMFLOAT3(-100, 16, -116));
		actor->SetAngle(DirectX::XMFLOAT3(0, 0, 0));
		actor->SetScale(DirectX::XMFLOAT3(0.04f, 0.04f, 0.04f));
		actor->AddComponent<Movement>();
		actor->AddComponent<Charactor>();
		actor->AddComponent<Player>();
		actor->AddShader<PhongVarianceShadowMap>(device);
	}
	ActorManager::Instance().Update(0.01f);
	ActorManager::Instance().UpdateTransform();

	//MetaAI::Instance();
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

	// 当たり判定更新処理
	CollisionManager::Instance().Update();
	if (battle_flag)
	{
		SceneManager::Instance().ChangeScene(new SceneBattle());
		return;
	}
	// UI更新処理
	UIManager::Instance().Update(elapsed_time);
}

void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* context = graphics.GetDeviceContext();

	// 描画処理
	render_context.light_direction = Light::LightDir;
	render_context.ShadowParameter = { shadow_color.x, shadow_color.y, shadow_color.z, 0.000001f };

	// カメラパラメータ設定
	Camera& camera = Camera::Instance();
	render_context.view = camera.GetView();
	render_context.projection = camera.GetProjection();

	// 深度ステンシルバッファ設定
	ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();
	// バックバッファのクリア処理
	{
		ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();

		// 画面クリア
		graphics.ScreenClear(&render_target_view, depth_stencil_view);
	}
	float screen_width = graphics.GetScreenWidth();
	float screen_height = graphics.GetScreenHeight();
	// スクリーンテクスチャをレンダーターゲットに設定して画面クリア
	{
		// レンダーターゲットの回復
		ID3D11RenderTargetView* screen_texture = graphics.GetTexture()->GetRenderTargetView();
		graphics.SetRenderTargetView(&screen_texture, depth_stencil_view);

		// 画面クリア
		graphics.ScreenClear(&screen_texture, depth_stencil_view);
	}
	// ビューポートを元に戻す
	graphics.SetViewport(static_cast<float>(graphics.GetScreenWidth()), static_cast<float>(graphics.GetScreenHeight()));

	// スカイボックス描画
	{
		graphics.GetSkyBoxShader()->Begin(context, render_context);
		sprite->Render(context,
			sky.get(),
			0, 0,
			screen_width, screen_height,
			0, 0,
			static_cast<float>(sky->GetWidth()), static_cast<float>(sky->GetHeight()),
			0,
			1, 1, 1, 1);
		graphics.GetSkyBoxShader()->End(context);
	}

	// デバッグプリミティブ描画
	{
		EnemyManager::Instance().DrawDebugPrimitive();
		CollisionManager::Instance().Draw();
		graphics.GetDebugRenderer()->Render(context, render_context.view, render_context.projection);
	}

	// アクター描画
	{
		// シャドウマップ作成
		ActorManager::Instance().ShadowRender(render_context, blur_render_context);

		// 描画
		ActorManager::Instance().Render(render_context);
	}

	Texture* texture = bulr->Render(graphics.GetTexture());
	{
		// レンダーターゲットをシャドウマップに設定
		ID3D11RenderTargetView* render_target_view[1] = { bulr_texture->GetRenderTargetView() };
		ID3D11DepthStencilView* depth_stencil_view = depth_texture->GetDepthStencilView();
		graphics.SetRenderTargetView(render_target_view, depth_stencil_view);

		// 画面クリア
		graphics.ScreenClear(render_target_view, depth_stencil_view);
	}
	// ビューポートの設定
	graphics.SetViewport(graphics.GetScreenWidth(), graphics.GetScreenHeight());

	graphics.GetSpriteShader()->Begin(context);
	sprite->Render(context,
		texture,
		0, 0,
		screen_width, screen_height,
		0, 0,
		texture->GetWidth(), texture->GetHeight());
	graphics.GetSpriteShader()->End(context);

	{
		ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
		ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();
		
		// レンダーターゲット設定
		graphics.SetRenderTargetView(&render_target_view, depth_stencil_view);

		// 画面クリア
		graphics.ScreenClear(&render_target_view, depth_stencil_view);
	}

	// ビューポート設定
	graphics.SetViewport(screen_width, screen_height);

	// バックバッファにスクリーンテクスチャに描画
	graphics.GetSpriteShader()->Begin(context);
	//sprite->Render(context,
	//	bulr_texture.get(),
	//	0, 0,
	//	screen_width, screen_height,
	//	0, 0,
	//	bulr_texture->GetWidth(), bulr_texture->GetHeight());
	sprite->Render(context, graphics.GetTexture(),
		0, 0,
		screen_width, screen_height,
		0, 0,
		(float)graphics.GetTexture()->GetWidth(), (float)graphics.GetTexture()->GetHeight(),
		0,
		1, 1, 1, 1);
	graphics.GetSpriteShader()->End(context);

	// メニュー描画
	{
		if (MenuSystem::Instance().IsOpened())
		{
			graphics.GetSpriteShader()->Begin(context);
			MenuSystem::Instance().Render(context);
			graphics.GetSpriteShader()->End(context);
		}
	}

	// GUI描画
	//OnGui();
}

bool SceneGame::OnMessages(const Telegram& telegram)
{
	switch (telegram.message_box.message)
	{
	case MessageType::Message_Hit_Boddy:
		battle_flag = true;
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
	ImGui::End();

	const char* listbox_items[] = { "Apple", "Banana", "Cherry", "Kiwi", "Mango", "Orange", "Pineapple", "Strawberry", "Watermelon" };
	static int listbox_item_current = 1;
	ImGui::ListBox("listbox\n(single select)", &listbox_item_current, listbox_items, IM_ARRAYSIZE(listbox_items), 4);

	static float f1 = 1.00f, f2 = 0.0067f;
	ImGui::DragFloat("drag float", &f1, 0.005f);
	static int e = 0;
	ImGui::RadioButton("radio a", &e, 0); ImGui::SameLine();
	ImGui::RadioButton("radio b", &e, 1); ImGui::SameLine();
	ImGui::RadioButton("radio c", &e, 2);

	static bool selected[4] = { false, true, false, false };
	ImGui::Selectable("1. I am selectable", &selected[0]);
	ImGui::Selectable("2. I am selectable", &selected[1]);
	ImGui::Text("3. I am not selectable");
	ImGui::Selectable("4. I am selectable", &selected[2]);
	if (ImGui::Selectable("5. I am double clickable", selected[3], ImGuiSelectableFlags_AllowDoubleClick))
		if (ImGui::IsMouseDoubleClicked(0))
			selected[3] = !selected[3];


	//bool* p_open = (bool*)1;
	//ImGui::SetNextWindowPos(ImVec2(10, 10));
	//if (!ImGui::Begin("Example: Fixed Overlay", p_open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
	//{
	//	ImGui::End();
	//	return;
	//}
	//ImGui::Text("Simple overlay\non the top-left side of the screen.");
	//ImGui::Separator();
	//ImGui::Text("Mouse Position: (%.1f,%.1f)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
	//ImGui::End();

	//static int i1 = 0;
	//ImGui::SliderInt("slider int", &i1, -1, 3);
	//ImGui::SameLine();

	ImGui::SetNextWindowPos(ImVec2(550, 50), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::TreeNode("Borders"))
	{
		ImGui::Text("Width %.2f", ImGui::GetColumnWidth());
		if (ImGui::TreeNode("Border"))
			ImGui::TreePop();
		// NB: Future columns API should allow automatic horizontal borders.
		static bool h_borders = true;
		static bool v_borders = true;
		static int columns_count = 4;
		const int lines_count = 3;
		ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);
		ImGui::DragInt("##columns_count", &columns_count, 0.1f, 2, 10, "%d columns");
		if (columns_count < 2)
			columns_count = 2;
		ImGui::SameLine();
		ImGui::Checkbox("horizontal", &h_borders);
		ImGui::SameLine();
		ImGui::Checkbox("vertical", &v_borders);
		ImGui::Columns(columns_count, NULL, v_borders);
		for (int i = 0; i < columns_count * lines_count; i++)
		{
			if (h_borders && ImGui::GetColumnIndex() == 0)
				ImGui::Separator();
			ImGui::Text("%c%c%c", 'a' + i, 'a' + i, 'a' + i);
			ImGui::Text("Width %.2f", ImGui::GetColumnWidth());
			ImGui::Text("Avail %.2f", ImGui::GetContentRegionAvail().x);
			ImGui::Text("Offset %.2f", ImGui::GetColumnOffset());
			ImGui::Text("Long text that is likely to clip");
			ImGui::RadioButton("radio c", &e, 2);
			ImGui::Checkbox("vertical", &v_borders);
			ImGui::NextColumn();
		}
		ImGui::Columns(1);
		if (h_borders)
			ImGui::Separator();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Mixed items"))
	{
		ImGui::Columns(3, "mixed");
		ImGui::Separator();

		ImGui::Text("Hello");
		ImGui::Button("Banana");
		ImGui::NextColumn();

		ImGui::Text("ImGui");
		ImGui::Button("Apple");
		static float foo = 1.0f;
		ImGui::InputFloat("red", &foo, 0.05f, 0, "%.3f");
		ImGui::Text("An extra line here.");
		ImGui::NextColumn();

		ImGui::Text("Sailor");
		ImGui::Button("Corniflower");
		static float bar = 1.0f;
		ImGui::InputFloat("blue", &bar, 0.05f, 0, "%.3f");
		ImGui::NextColumn();

		if (ImGui::CollapsingHeader("Category A")) { ImGui::Text("Blah blah blah"); } ImGui::NextColumn();
		if (ImGui::CollapsingHeader("Category B")) { ImGui::Text("Blah blah blah"); } ImGui::NextColumn();
		if (ImGui::CollapsingHeader("Category C")) { ImGui::Text("Blah blah blah"); } ImGui::NextColumn();
		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::TreePop();
	}

	ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.4f);
	ImGui::GetStyle().Colors[ImGuiCol_Border].x = 1.0f;
	ImGui::GetStyle().Colors[ImGuiCol_TitleBg] = ImVec4(0.0f, 1.0f, 1.0f, 0.7f);
	ImGui::GetStyle().Colors[ImGuiCol_TitleBgActive] = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
	ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 0.7f);
}
