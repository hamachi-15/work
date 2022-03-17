#pragma once
#include <memory>
#include "Scene.h"
#include "CameraController.h"
#include "Sprite.h"
#include "Texture.h"

struct RenderContext;
struct BlurRenderContext;
//class Sprite;
//class Texture;

class SceneTitle : public Scene
{
public:
	// コンストラクタ・デストラクタ
	SceneTitle() {};
	~SceneTitle()override{}

	// シーン名取得
	std::string GetName() const override { return "SceneTitle"; }

	// 初期化処理
	void Initialize() override;

	// 終了処理
	void Finalize() override;

	// 更新処理
	void Update(float elapsed_time) override;

	// スクリーンテクスチャ描画
	void ScreenRender(ID3D11DeviceContext* context, RenderContext* render_context, const DirectX::XMFLOAT2& screen_size);

	//バックバッファ描画
	void BuckBufferRender(ID3D11DeviceContext* context, RenderContext* render_context, const DirectX::XMFLOAT2& screen_size);

	// 描画処理
	void Render() override;

	// ボタン描画
	void RenderButton(ID3D11DeviceContext* context, Texture* texture, int index);

	// ImGui描画
	void OnGui();
private:
	std::unique_ptr<RenderContext>						render_context;
	std::unique_ptr<BlurRenderContext>					blur_render_context;

	std::unique_ptr<Sprite> sprite;
	std::unique_ptr<Texture> sky;
	std::unique_ptr<Texture> title_bg;
	std::unique_ptr<Texture> mask_texture;
	std::unique_ptr<Texture> controll_texture;
	DirectX::XMFLOAT2 controll_texture_size = { 512, 512 };
	DirectX::XMFLOAT2 render_start_position = { -20, 250 };
	//ライト
	DirectX::XMFLOAT3 LightDir = { 0,0,0 };

	float timer = 0.0f; // ディソープに使う
};