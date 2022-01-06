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

#include "MenuSystem.h"
#include "UI.h"

#include "Messenger.h"
#include "MessageData.h"
#include "GameDataBase.h"

#include "Collision.h"
#include "Texture.h"
#include "Sprite.h"
#include "MetaAI.h"
#include "ShaderManager.h"

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
		DirectX::XMFLOAT3(0, 1, 0)
	);
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
		actor->SetPosition(DirectX::XMFLOAT3(-100, 4, -116));
		actor->SetAngle(DirectX::XMFLOAT3(0, 0, 0));
		actor->SetScale(DirectX::XMFLOAT3(0.04f, 0.04f, 0.04f));
		actor->AddComponent<Movement>();
		actor->AddComponent<Charactor>();
		actor->AddComponent<Player>();
		actor->SetShaderType(ShaderManager::ShaderType::Lambert);
	}
	ActorManager::Instance().Update(0.01f);
	ActorManager::Instance().UpdateTransform();

	// スクリプトから敵の生成
	EnemyManager::Instance().CreateEnemyScriptData();
}

void SceneBattle::Finalize()
{
	// 敵マネージャーのクリア
	EnemyManager::Instance().AllRemove();

	// アクターの破棄
	ActorManager::Instance().AllDestroy();

	// コリジョンの破棄
	CollisionManager::Instance().Destroy();

	// メッセンジャーのクリア
	Messenger::Instance().Clear();

	// スクリプトを削除
	DeleteFileA("./Data/Script/SendBattleSceneScript.txt");
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

	// メタAIの更新処理
	MetaAI::Instance().Update(elapsed_time);

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

	// GUI描画
	OnGui();
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

	//Texture* texture = bulr->Render(graphics.GetTexture());
	//{
	//	ID3D11RenderTargetView* render_target_view[1] = { bulr_texture->GetRenderTargetView() };
	//	graphics.SetRenderTargetView(render_target_view, depth_stencil_view);
	//}
	//// ビューポートの設定
	//graphics.SetViewport(screen_width, screen_height);
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
	// 輝度抽出テクスチャを加算合成
	//sprite->AddRender(context,
	//	bloom_texture,
	//	0, 0,
	//	screen_size.x, screen_size.y,
	//	0, 0,
	//	(float)bloom_texture->GetWidth(), (float)bloom_texture->GetHeight());
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

	//const char* listbox_items[] = { "Apple", "Banana", "Cherry", "Kiwi", "Mango", "Orange", "Pineapple", "Strawberry", "Watermelon" };
	//static int listbox_item_current = 1;
	//ImGui::ListBox("listbox\n(single select)", &listbox_item_current, listbox_items, IM_ARRAYSIZE(listbox_items), 4);

	//static float f1 = 1.00f, f2 = 0.0067f;
	//ImGui::DragFloat("drag float", &f1, 0.005f);
	//static int e = 0;
	//ImGui::RadioButton("radio a", &e, 0); ImGui::SameLine();
	//ImGui::RadioButton("radio b", &e, 1); ImGui::SameLine();
	//ImGui::RadioButton("radio c", &e, 2);

	//static bool selected[4] = { false, true, false, false };
	//ImGui::Selectable("1. I am selectable", &selected[0]);
	//ImGui::Selectable("2. I am selectable", &selected[1]);
	//ImGui::Text("3. I am not selectable");
	//ImGui::Selectable("4. I am selectable", &selected[2]);
	//if (ImGui::Selectable("5. I am double clickable", selected[3], ImGuiSelectableFlags_AllowDoubleClick))
	//	if (ImGui::IsMouseDoubleClicked(0))
	//		selected[3] = !selected[3];


	////bool* p_open = (bool*)1;
	////ImGui::SetNextWindowPos(ImVec2(10, 10));
	////if (!ImGui::Begin("Example: Fixed Overlay", p_open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
	////{
	////	ImGui::End();
	////	return;
	////}
	////ImGui::Text("Simple overlay\non the top-left side of the screen.");
	////ImGui::Separator();
	////ImGui::Text("Mouse Position: (%.1f,%.1f)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
	////ImGui::End();

	////static int i1 = 0;
	////ImGui::SliderInt("slider int", &i1, -1, 3);
	////ImGui::SameLine();

	//ImGui::SetNextWindowPos(ImVec2(550, 50), ImGuiCond_FirstUseEver);
	//ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	//if (ImGui::TreeNode("Borders"))
	//{
	//	ImGui::Text("Width %.2f", ImGui::GetColumnWidth());
	//	if (ImGui::TreeNode("Border"))
	//		ImGui::TreePop();
	//	// NB: Future columns API should allow automatic horizontal borders.
	//	static bool h_borders = true;
	//	static bool v_borders = true;
	//	static int columns_count = 4;
	//	const int lines_count = 3;
	//	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);
	//	ImGui::DragInt("##columns_count", &columns_count, 0.1f, 2, 10, "%d columns");
	//	if (columns_count < 2)
	//		columns_count = 2;
	//	ImGui::SameLine();
	//	ImGui::Checkbox("horizontal", &h_borders);
	//	ImGui::SameLine();
	//	ImGui::Checkbox("vertical", &v_borders);
	//	ImGui::Columns(columns_count, NULL, v_borders);
	//	for (int i = 0; i < columns_count * lines_count; i++)
	//	{
	//		if (h_borders && ImGui::GetColumnIndex() == 0)
	//			ImGui::Separator();
	//		ImGui::Text("%c%c%c", 'a' + i, 'a' + i, 'a' + i);
	//		ImGui::Text("Width %.2f", ImGui::GetColumnWidth());
	//		ImGui::Text("Avail %.2f", ImGui::GetContentRegionAvail().x);
	//		ImGui::Text("Offset %.2f", ImGui::GetColumnOffset());
	//		ImGui::Text("Long text that is likely to clip");
	//		ImGui::RadioButton("radio c", &e, 2);
	//		ImGui::Checkbox("vertical", &v_borders);
	//		ImGui::NextColumn();
	//	}
	//	ImGui::Columns(1);
	//	if (h_borders)
	//		ImGui::Separator();
	//	ImGui::TreePop();
	//}
	//if (ImGui::TreeNode("Mixed items"))
	//{
	//	ImGui::Columns(3, "mixed");
	//	ImGui::Separator();

	//	ImGui::Text("Hello");
	//	ImGui::Button("Banana");
	//	ImGui::NextColumn();

	//	ImGui::Text("ImGui");
	//	ImGui::Button("Apple");
	//	static float foo = 1.0f;
	//	ImGui::InputFloat("red", &foo, 0.05f, 0, "%.3f");
	//	ImGui::Text("An extra line here.");
	//	ImGui::NextColumn();

	//	ImGui::Text("Sailor");
	//	ImGui::Button("Corniflower");
	//	static float bar = 1.0f;
	//	ImGui::InputFloat("blue", &bar, 0.05f, 0, "%.3f");
	//	ImGui::NextColumn();

	//	if (ImGui::CollapsingHeader("Category A")) { ImGui::Text("Blah blah blah"); } ImGui::NextColumn();
	//	if (ImGui::CollapsingHeader("Category B")) { ImGui::Text("Blah blah blah"); } ImGui::NextColumn();
	//	if (ImGui::CollapsingHeader("Category C")) { ImGui::Text("Blah blah blah"); } ImGui::NextColumn();
	//	ImGui::Columns(1);
	//	ImGui::Separator();
	//	ImGui::TreePop();
	//}
	//if (ImGui::TreeNode("Word-wrapping"))
	//{
	//	ImGui::Columns(2, "word-wrapping");
	//	ImGui::Separator();
	//	ImGui::TextWrapped("The quick brown fox jumps over the lazy dog.");
	//	ImGui::TextWrapped("Hello Left");
	//	ImGui::NextColumn();
	//	ImGui::TextWrapped("The quick brown fox jumps over the lazy dog.");
	//	ImGui::TextWrapped("Hello Right");
	//	ImGui::Columns(1);
	//	ImGui::Separator();
	//	ImGui::TreePop();
	//}
	//if (ImGui::CollapsingHeader("Filtering"))
	//{
	//	// Helper class to easy setup a text filter.
	//	// You may want to implement a more feature-full filtering scheme in your own application.
	//	static ImGuiTextFilter filter;
	//	ImGui::Text("Filter usage:\n"
	//		"  \"\"         display all lines\n"
	//		"  \"xxx\"      display lines containing \"xxx\"\n"
	//		"  \"xxx,yyy\"  display lines containing \"xxx\" or \"yyy\"\n"
	//		"  \"-xxx\"     hide lines containing \"xxx\"");
	//	filter.Draw();
	//	const char* lines[] = { "aaa1.c", "bbb1.c", "ccc1.c", "aaa2.cpp", "bbb2.cpp", "ccc2.cpp", "abc.h", "hello, world" };
	//	for (int i = 0; i < IM_ARRAYSIZE(lines); i++)
	//		if (filter.PassFilter(lines[i]))
	//			ImGui::BulletText("%s", lines[i]);
	//}
	//if (ImGui::TreeNode("Tabbing"))
	//{
	//	ImGui::Text("Use TAB/SHIFT+TAB to cycle through keyboard editable fields.");
	//	static char buf[32] = "hello";
	//	ImGui::InputText("1", buf, IM_ARRAYSIZE(buf));
	//	ImGui::InputText("2", buf, IM_ARRAYSIZE(buf));
	//	ImGui::InputText("3", buf, IM_ARRAYSIZE(buf));
	//	ImGui::PushAllowKeyboardFocus(false);
	//	ImGui::InputText("4 (tab skip)", buf, IM_ARRAYSIZE(buf));
	//	//ImGui::SameLine(); HelpMarker("Use ImGui::PushAllowKeyboardFocus(bool) to disable tabbing through certain widgets.");
	//	ImGui::PopAllowKeyboardFocus();
	//	ImGui::InputText("5", buf, IM_ARRAYSIZE(buf));
	//	ImGui::TreePop();
	//}
	//if (ImGui::TreeNode("Focus from code"))
	//{
	//	bool focus_1 = ImGui::Button("Focus on 1"); ImGui::SameLine();
	//	bool focus_2 = ImGui::Button("Focus on 2"); ImGui::SameLine();
	//	bool focus_3 = ImGui::Button("Focus on 3");
	//	int has_focus = 0;
	//	static char buf[128] = "click on a button to set focus";
	//	if (focus_1) ImGui::SetKeyboardFocusHere();
	//	ImGui::InputText("1", buf, IM_ARRAYSIZE(buf));
	//	if (ImGui::IsItemActive()) has_focus = 1;
	//	if (focus_2) ImGui::SetKeyboardFocusHere();
	//	ImGui::InputText("2", buf, IM_ARRAYSIZE(buf));
	//	if (ImGui::IsItemActive()) has_focus = 2;
	//	ImGui::PushAllowKeyboardFocus(false);
	//	if (focus_3) ImGui::SetKeyboardFocusHere();
	//	ImGui::InputText("3 (tab skip)", buf, IM_ARRAYSIZE(buf));
	//	if (ImGui::IsItemActive()) has_focus = 3;
	//	ImGui::PopAllowKeyboardFocus();
	//	if (has_focus)
	//		ImGui::Text("Item with focus: %d", has_focus);
	//	else
	//		ImGui::Text("Item with focus: <none>");
	//	// Use >= 0 parameter to SetKeyboardFocusHere() to focus an upcoming item
	//	static float f3[3] = { 0.0f, 0.0f, 0.0f };
	//	int focus_ahead = -1;
	//	if (ImGui::Button("Focus on X")) { focus_ahead = 0; } ImGui::SameLine();
	//	if (ImGui::Button("Focus on Y")) { focus_ahead = 1; } ImGui::SameLine();
	//	if (ImGui::Button("Focus on Z")) { focus_ahead = 2; }
	//	if (focus_ahead != -1) ImGui::SetKeyboardFocusHere(focus_ahead);
	//	ImGui::SliderFloat3("Float3", &f3[0], 0.0f, 1.0f);

	//	ImGui::TextWrapped("NB: Cursor & selection are preserved when refocusing last used item in code.");
	//	ImGui::TreePop();
	//}
	//ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.4f);
	//ImGui::GetStyle().Colors[ImGuiCol_Border].x = 1.0f;
	//ImGui::GetStyle().Colors[ImGuiCol_TitleBg] = ImVec4(0.0f, 1.0f, 1.0f, 0.7f);
	//ImGui::GetStyle().Colors[ImGuiCol_TitleBgActive] = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
	//ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 0.7f);
}
