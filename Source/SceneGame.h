#pragma once
#include <memory>
#include <DirectXMath.h>
#include "Shader.h"
#include "Scene.h"
#include "RenderContext.h"

class Texture;
class Sprite;
class CameraController;
class GaussianBlur;

class SceneGame : public Scene
{
public:
	SceneGame();
	~SceneGame() override;
private:

	// 初期化処理
	void Initialize() override;

	// 終了処理
	void Finalize() override;

	// 更新処理
	void Update(float elapsed_time) override;

	// 描画処理
	void Render() override;

	// ImGui描画
	void OnGui();
private:
	RenderContext render_context;
	BlurRenderContext blur_render_context;
	std::unique_ptr<Shader> sprite_cube;

	std::unique_ptr<Sprite> sprite;
	std::unique_ptr<Texture> sky;


	std::unique_ptr<CameraController>camera_controller;
	//ライト
	DirectX::XMFLOAT3 LightDir = { 0,0,0 };

	DirectX::XMFLOAT3 shadow_color = { 0.5f, 0.5f, 0.5f };
	std::unique_ptr<GaussianBlur>		 bulr;

};