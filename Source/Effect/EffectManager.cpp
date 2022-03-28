#include "Graphics.h"
#include "EffectManager.h"

//------------------------------------
// 初期化処理
//------------------------------------
void EffectManager::Initialize()
{
	Graphics& graphics = Graphics::Instance();

	// Effekseerレンダラー作成
	effekseer_renderer = EffekseerRendererDX11::Renderer::Create(
		graphics.GetDevice(), graphics.GetDeviceContext(), 2048);

	// Effekseerマネージャー生成
	effekseer_manager = Effekseer::Manager::Create(2048);

	// Effekseerレンダラの各種設定(特別なカスタマイズをしない場合はこの設定でOK)
	effekseer_manager->SetSpriteRenderer(effekseer_renderer->CreateSpriteRenderer());
	effekseer_manager->SetRibbonRenderer(effekseer_renderer->CreateRibbonRenderer());
	effekseer_manager->SetRingRenderer(effekseer_renderer->CreateRingRenderer());
	effekseer_manager->SetTrackRenderer(effekseer_renderer->CreateTrackRenderer());
	effekseer_manager->SetModelRenderer(effekseer_renderer->CreateModelRenderer());

	// Effekseer内でのローダー設定(特別なカスタマイズをしない場合はこの設定でOK)
	effekseer_manager->SetTextureLoader(effekseer_renderer->CreateTextureLoader());
	effekseer_manager->SetModelLoader(effekseer_renderer->CreateModelLoader());
	effekseer_manager->SetMaterialLoader(effekseer_renderer->CreateMaterialLoader());

	// Effekseerを左手座標系で計算する
	effekseer_manager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
}

//-----------------------------------
// 終了化処理
//-----------------------------------
void EffectManager::Finalize()
{
	if (effekseer_manager != nullptr)
	{
		effekseer_manager->Destroy();
		effekseer_manager = nullptr;
	}
	if (effekseer_renderer != nullptr)
	{
		effekseer_renderer->Destroy();
		effekseer_renderer = nullptr;
	}
}

//-----------------------------------
// 更新処理
//------------------------------------
void EffectManager::Update(float elapsed_time)
{
	// エフェクトの更新処理(引数はフレームの経過時間を渡す)
	effekseer_manager->Update(elapsed_time * 60.0f);
}

//-------------------------------------
// 描画処理
//-------------------------------------
void EffectManager::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
	// ビュー＆プロジェクション行列をEffekseerレンダラに設定
	effekseer_renderer->SetCameraMatrix(*reinterpret_cast<const Effekseer::Matrix44*>(&view));
	effekseer_renderer->SetProjectionMatrix(*reinterpret_cast<const Effekseer::Matrix44*>(&projection));

	// Effekseer描画開始
	effekseer_renderer->BeginRendering();

	// Effekseer描画実行
	// マネージャー単位で描画するので描画順を制御する場合はマネージャーを複数個作成し
	// Draw()関数を実行する順序で制御
	effekseer_manager->Draw();

	// Effekseer描画終了
	effekseer_renderer->EndRendering();
}

