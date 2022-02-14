#include "SceneOver.h"
#include "ResourceManager.h"
#include "ShaderManager.h"
#include "Graphics.h"
#include "Sprite.h"

#include "GameDatabase.h"
//-------------------------------------
// 初期化処理
//-------------------------------------
void SceneOver::Initialize()
{
	// テクスチャ読み込み
	sprite = std::make_unique<Sprite>();
	anybutton_texture = ResourceManager::Instance().LoadTexture("Data/Sprite/PushAnyButton.png");
	over_texture = std::make_unique<Texture>();
	over_texture->Load("Data/Sprite/YouDead.png");
}

//-------------------------------------
// 終了化処理
//-------------------------------------
void SceneOver::Finalize()
{
}

//-------------------------------------
// 更新処理
//-------------------------------------
void SceneOver::Update(float elapsed_time)
{
}

//-------------------------------------
// 描画処理
//-------------------------------------
void SceneOver::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* context = graphics.GetDeviceContext();
	// スクリーンサイズ取得
	DirectX::XMFLOAT2 screen_size = { graphics.GetScreenWidth(), graphics.GetScreenHeight() };

	// バックバッファのクリア処理
	{
		ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
		ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();
		// 画面クリア
		graphics.ScreenClear(&render_target_view, depth_stencil_view);
	}

	// スクリーンテクスチャに描画
	ScreenRender(context, nullptr, screen_size);

	// バックバッファに描画
	BuckBufferRender(context, nullptr, screen_size);
}

//-------------------------------------
// スクリーンテクスチャ描画
//-------------------------------------
void SceneOver::ScreenRender(ID3D11DeviceContext* context, RenderContext* render_context, const DirectX::XMFLOAT2& screen_size)
{
	Graphics& graphics = Graphics::Instance();

	ShaderManager& shader_manager = ShaderManager::Instance();
	std::shared_ptr<Shader> sprite_shader = shader_manager.GetShader(ShaderManager::ShaderType::Sprite);
	
	// スクリーンテクスチャをレンダーターゲットに設定して画面クリア
	ID3D11RenderTargetView* screen_texture = graphics.GetTexture()->GetRenderTargetView();
	ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();
	{
		graphics.SetRenderTargetView(&screen_texture, depth_stencil_view);
		graphics.ScreenClear(&screen_texture, depth_stencil_view);
	}
	// ビューポートを元に戻す
	graphics.SetViewport(screen_size.x, screen_size.y);

	Texture* texture = GameDataBase::Instance().GetTimingTexture();
	sprite_shader->Begin(context);
	sprite->Render(context,
		texture,
		0.0f, 0.0f,
		screen_size.x, screen_size.y,
		0, 0,
		(float)texture->GetWidth(), (float)texture->GetHeight());
	sprite->Render(context,
		over_texture.get(),
		400, 100,
		(float)over_texture->GetWidth(), (float)over_texture->GetHeight(),
		0, 0,
		(float)over_texture->GetWidth(), (float)over_texture->GetHeight());
	sprite->Render(context,
		anybutton_texture.get(),
		400, 400,
		(float)anybutton_texture->GetWidth(), (float)anybutton_texture->GetHeight(),
		0, 0,
		(float)anybutton_texture->GetWidth(), (float)anybutton_texture->GetHeight());
	sprite_shader->End(context);
}

//-------------------------------------
//バックバッファ描画
//-------------------------------------
void SceneOver::BuckBufferRender(ID3D11DeviceContext* context, RenderContext* render_context, const DirectX::XMFLOAT2& screen_size)
{
	Graphics& graphics = Graphics::Instance();
	ShaderManager& shader_manager = ShaderManager::Instance();
	// スプライトシェーダー取得
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

	std::shared_ptr<Shader> sprite_shader = shader_manager.GetShader(ShaderManager::ShaderType::Sprite);
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
	sprite_shader->End(context);
}
