#pragma once
#include <memory>
#include "Scene.h"
#include "CameraController.h"

class Sprite;
class Texture;
struct RenderContext;

class SceneClear : public Scene
{
public:
	// コンストラクタ
	SceneClear(){}

	// デストラクタ
	~SceneClear() override {}

	// シーン名取得
	std::string GetName() const override { return "SceneClear"; }

	// 初期化処理
	void Initialize() override;

	// 終了化処理
	void Finalize() override;

	// 更新処理
	void Update(float elapsed_time) override;

	// 描画処理
	void Render() override;

	// スクリーンテクスチャ描画
	void ScreenRender(ID3D11DeviceContext* context, RenderContext* render_context, const DirectX::XMFLOAT2& screen_size);

	// ポストテクスチャ描画
	void PostRender(ID3D11DeviceContext* context, RenderContext* render_context, const DirectX::XMFLOAT2& screen_size);

	//バックバッファ描画
	void BuckBufferRender(ID3D11DeviceContext* context, RenderContext* render_context, const DirectX::XMFLOAT2& screen_size);

	// メッセージ処理
	bool OnMessages(const Telegram& telegram) override { return true; }

	// GUI描画
	void OnGui() {}
private:
	std::unique_ptr<Sprite>				sprite;
	std::shared_ptr<Texture>			anybutton_texture;
	std::unique_ptr<Texture>			clear_texture;
	std::unique_ptr<Texture>			clear_timing_texture;
};
