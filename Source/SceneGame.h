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
class Bloom;

class SceneGame : public Scene
{
public:
	SceneGame();
	~SceneGame() override;
private:

	// ����������
	void Initialize() override;

	// �I������
	void Finalize() override;

	// �X�V����
	void Update(float elapsed_time) override;

	// �`�揈��
	void Render() override;

	bool OnMessages(const Telegram& telegram) override;

	// ImGui�`��
	void OnGui();
private:
	RenderContext render_context;
	BlurRenderContext blur_render_context;
	std::unique_ptr<Shader> sprite_cube;

	std::unique_ptr<Sprite> sprite;
	std::unique_ptr<Texture> sky;


	std::unique_ptr<CameraController>camera_controller;
	//���C�g
	DirectX::XMFLOAT3 LightDir = { 0,0,0 };

	DirectX::XMFLOAT3 shadow_color = { 0.5f, 0.5f, 0.5f };
	std::unique_ptr<GaussianBlur>		 bulr;
	std::unique_ptr<Bloom>		 bloom;
	std::unique_ptr<Texture>			bulr_texture;
	std::unique_ptr<Texture> depth_texture;
	bool battle_flag = false;

};