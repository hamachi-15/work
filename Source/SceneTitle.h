#pragma once
#include <memory>
#include "Scene.h"
#include "Sprite.h"
#include "Texture.h"
#include "CameraController.h"

class SceneTitle : public Scene
{
public:
	// コンストラクタ・デストラクタ
	SceneTitle() {};
	~SceneTitle()override{}

	// 初期化処理
	void Initialize() override;

	// 終了処理
	void Finalize() override;

	// 更新処理
	void Update(float elapsed_time) override;

	// スクリーンテクスチャ描画
	void ScreenRender(ID3D11DeviceContext* context, RenderContext& render_context, const DirectX::XMFLOAT2& screen_size);

	// バックバッファ描画
	void BuckBufferRender(ID3D11DeviceContext* context, RenderContext& render_context, const DirectX::XMFLOAT2& screen_size);
	
	// 描画処理
	void Render() override;

	// ボタン描画
	void RenderButton(ID3D11DeviceContext* context, Texture* texture, int index);

	// ImGui描画
	void OnGui();
private:
	RenderContext render_context;
	BlurRenderContext blur_render_context;

	std::unique_ptr<Sprite> sprite;
	std::unique_ptr<Texture> sky;
	std::unique_ptr<Texture> mask_texture;

	//ライト
	DirectX::XMFLOAT3 LightDir = { 0,0,0 };

	float timer = 0.0f; // ディソープに使う
};