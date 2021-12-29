#include <thread>
#include "Graphics.h"
#include "SceneLoading.h"
#include "SceneManager.h"
#include "ShaderManager.h"

//----------------------------------
// コンストラクタ
//----------------------------------
SceneLoading::SceneLoading(Scene* next_scene)
{
	this->next_scene.reset(next_scene);
}

//----------------------------------
// 初期化処理
//----------------------------------
void SceneLoading::Initialize()
{
	sprite = std::make_unique<Sprite>("Data/Sprite/LoadingIcon.png");

	// スレッド開始
	std::thread thread(LoadingThread, this);

	// スレッドの管理を放棄
	thread.detach();
}

//----------------------------------
// 終了処理
//----------------------------------
void SceneLoading::Finalize()
{
}

//----------------------------------
// 更新処理
//----------------------------------
void SceneLoading::Update(float elapsed_time)
{
	constexpr float speed = 180;
	angle += speed * elapsed_time;

	if (next_scene->IsReady())
	{
		SceneManager::Instance().ChangeScene(next_scene.release());
	}
}

//----------------------------------
// 描画処理
//----------------------------------
void SceneLoading::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* context = graphics.GetDeviceContext();
	ShaderManager& shader_manager = ShaderManager::Instance();
	std::shared_ptr<Shader> sprite_shader = shader_manager.GetShader(ShaderManager::ShaderType::Sprite);

	ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();

	// 画面クリア
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };
	context->ClearRenderTargetView(render_target_view, color);
	context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);

	float screen_width = graphics.GetScreenWidth();
	float screen_height = graphics.GetScreenHeight();
	float texture_width = static_cast<float>(sprite->GetTextureWidth());
	float texture_height = static_cast<float>(sprite->GetTextureHeight());
	float positionX = screen_width - texture_width;
	float positionY = screen_height - texture_height;
	// TODO 深度テストが失敗する
	{
		sprite_shader->Begin(context);
		sprite->Render(context,
			positionX, positionY, texture_width, texture_height,
			0, 0, texture_width, texture_height,
			angle, Sprite::Pivot::CenterCenter);
		sprite_shader->End(context);
	}
}

//----------------------------------
// ローディングスレッド
//----------------------------------
void SceneLoading::LoadingThread(SceneLoading* scene)
{
	// COM関連の初期化でスレッド毎に呼ぶ
	HRESULT hr = CoInitialize(nullptr);

	// 次のシーンの初期化
	scene->next_scene->Initialize();

	// スレッドが終わる前にCOM関連の終了化
	CoUninitialize();

	// 次のシーンの準備完了設定
	scene->next_scene->SetReady();
}
