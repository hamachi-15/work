#include "SceneTitle.h"
#include "Graphics.h"
#include "Input.h"

#include "Sprite.h"
#include "Texture.h"

#include "DissolveShader.h"
#include "ShaderManager.h"
#include "AudioManager.h"
#include "EffectManager.h"

#include "SceneManager.h"
#include "SceneLoading.h"
#include "SceneGame.h"
#include "Messenger.h"

#include "TitleButtonUI.h"
#include "UIManager.h"

//----------------------------------
// 初期化処理
//----------------------------------
void SceneTitle::Initialize()
{
	Graphics& graphics = Graphics::Instance();

	// テクスチャ読込み
	sprite = std::make_unique<Sprite>();
	mask_texture = std::make_unique<Texture>();
	mask_texture->Load("Data/Sprite/dissolve_animation2.png");

	title_bg = std::make_unique<Texture>();
	title_bg->Load("Data/Sprite/TitleBG.jpg");

	controll_texture = std::make_unique<Texture>();
	controll_texture->Load("Data/Sprite/Controll.png");
	UIManager::Instance().RegisterUI(std::make_shared<TitleUI>());

	AudioManager::Instance().PlayBGM(BGMType::Title);
	AudioManager::Instance().SetBGMVolume(BGMType::Title, 0.5f);
}

//----------------------------------
// 終了処理
//----------------------------------
void SceneTitle::Finalize()
{
	// UIの全破棄
	UIManager::Instance().AllDelete();

	// メッセンジャーのクリア
	Messenger::Instance().Clear();

}

//----------------------------------
// 更新処理
//----------------------------------
void SceneTitle::Update(float elapsed_time)
{
	// シーン遷移フラグが立ったら
	if (IsSceneChangeFlag())
	{
		timer += elapsed_time;
		if (timer >= 1.2f)
		{
			// BGM再生停止
			AudioManager::Instance().StopBGM(BGMType::Title);
			// ゲームシーンに遷移
			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame()));
		}
	}

	// UI更新処理
	UIManager::Instance().Update(elapsed_time);
}

//----------------------------------
// スクリーンテクスチャ描画
//----------------------------------
void SceneTitle::ScreenRender(ID3D11DeviceContext* context, RenderContext* render_context, const DirectX::XMFLOAT2& screen_size)
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

	std::shared_ptr<Shader> sprite_shader = shader_manager.GetShader(ShaderManager::ShaderType::Sprite);
	
	// 洋皮紙描画
	{
		sprite_shader->Begin(context);
		sprite->Render(context,
			title_bg.get(),
			0, 0,
			screen_size.x, screen_size.y,
			0, 0,
			static_cast<float>(title_bg->GetWidth()), static_cast<float>(title_bg->GetHeight()));
		sprite_shader->End(context);
	}

	// タイトル画面のUI描画
	UIManager::Instance().Draw(context);

	// 操作説明描画
	sprite_shader->Begin(context);
	sprite->Render(context,
		controll_texture.get(),
		render_start_position.x, render_start_position.y,
		controll_texture_size.x, controll_texture_size.y,
		0, 0,
		controll_texture_size.x, controll_texture_size.y);
		sprite_shader->End(context);

}

//----------------------------------
// バックバッファ描画
//----------------------------------
void SceneTitle::BuckBufferRender(ID3D11DeviceContext* context, RenderContext* render_context, const DirectX::XMFLOAT2& screen_size)
{
	Graphics& graphics = Graphics::Instance();
	ShaderManager& shader_manager = ShaderManager::Instance();
	// シェーダー取得
	std::shared_ptr<Shader> sprite_shader = shader_manager.GetShader(ShaderManager::ShaderType::Sprite);
	std::shared_ptr<Shader> dissolve_shader = shader_manager.GetShader(ShaderManager::ShaderType::Dissolve);
	// レンダーターゲット設定
	{
		ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
		ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();
		graphics.SetRenderTargetView(&render_target_view, depth_stencil_view);
		graphics.ScreenClear(&render_target_view, depth_stencil_view, { 0.0f, 0.0f, 0.5f, 1.0f });
	}
	// ビューポート設定
	graphics.SetViewport(screen_size.x, screen_size.y);

	// バックバッファにスクリーンテクスチャを描画
	dissolve_shader->Begin(context, timer);
	dynamic_cast<DissolveShader*>(dissolve_shader.get())->Render(context, graphics.GetTexture(), mask_texture.get());
	dissolve_shader->End(context);
}

//----------------------------------
// 描画処理
//----------------------------------
void SceneTitle::Render()
{
	ShaderManager& shader_manager = ShaderManager::Instance();
	std::shared_ptr<Shader> sprite_shader = shader_manager.GetShader(ShaderManager::ShaderType::Sprite);
	
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* context = graphics.GetDeviceContext();
	ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();

	// 画面クリア
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };
	context->ClearRenderTargetView(render_target_view, color);
	context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);

	float screen_width = graphics.GetScreenWidth();
	float screen_height = graphics.GetScreenHeight();

	ScreenRender(context, nullptr, { screen_width, screen_height });

	BuckBufferRender(context, nullptr, { screen_width, screen_height });
	
	//OnGui();
}

//----------------------------
// ボタン描画
//----------------------------
void SceneTitle::RenderButton(ID3D11DeviceContext* context, Texture* texture, int index)
{

	Graphics& graphics = Graphics::Instance();
	float screen_width = graphics.GetScreenWidth();
	float screen_height = graphics.GetScreenHeight();
	sprite->Render(context,
		texture,
		screen_width * 0.25f, screen_height - (screen_height * 0.25f),
		250, 100,
		0, 0,
		(float)texture->GetWidth(), (float)texture->GetHeight(),
		0,
		1, 1, 1, 1);
}

//----------------------------
// GUI描画
//----------------------------
void SceneTitle::OnGui()
{
	ImGui::SetNextWindowPos(ImVec2(550, 50), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	ImGui::Begin("Shader", nullptr, ImGuiWindowFlags_None);
	ImGui::SliderFloat("Timer", &timer, 0.0f, 1.5f);

	ImGui::End();
}
