#pragma once
#include <mutex>
#include <memory>
#include <d3d11.h>
#include <wrl.h>
#include "Shader.h"
#include "SpriteShader.h"
#include "ImGuiRenderer.h"
#include "DebugRenderer.h"
#include "AudioSource.h"

class Graphics
{
public:
	// コンストラクタ
	Graphics(HWND hwnd);
	
	// デストラクタ
	~Graphics();

	// インスタンス取得
	static Graphics& Instance() { return  *instance; }

	// デバイス取得
	ID3D11Device* GetDevice() const { return device.Get(); }

	// デバイスコンテキスト取得
	ID3D11DeviceContext* GetDeviceContext() const { return context.Get(); }

	// スワップチェーン取得
	IDXGISwapChain* GetSwapChain() const { return swapchain.Get(); }

	// レンダーターゲットビュー取得
	ID3D11RenderTargetView* GetRenderTargetView() const { return render_target_view.Get(); }

	// デプスステンシルビュー取得
	ID3D11DepthStencilView* GetDepthStencilView() const { return depth_stencil_view.Get(); }

	// スクリーン高さ取得
	float GetScreenHeight() const { return screen_height; }

	// スクリーン幅取得
	float GetScreenWidth() const { return screen_width; }

	// ImGuiレンダラ取得
	ImGuiRenderer* GetImGuiRenderer() const { return imgui_renderer.get(); }

	// デバックレンダラー取得
	DebugRenderer* GetDebugRenderer() const { return debug_renderer.get(); }

	// スプライトシェーダー取得
	SpriteShader* GetSpriteShader() const { return sprite_shader.get(); }

	// スカイボックスシェーダー取得
	Shader* GetSkyBoxShader() const { return skybox_shader.get(); }

	AudioSource* GetAudio() const { return BGM.get(); }

	Texture* GetTexture() const { return scene_texture.get(); }

	// ミューテクス取得
	std::mutex& GetMutex() { return mutex; }

	// ビューポート設定
	void SetViewport(float weidth, float height);

	// レンダーターゲット設定
	void SetRenderTargetView(ID3D11RenderTargetView* render_target_view[], ID3D11DepthStencilView* depth_stensil_view);

	// 画面クリア
	void ScreenClear(ID3D11RenderTargetView* render_target_view[], ID3D11DepthStencilView* depth_stensil_view);

private:
	static Graphics* instance;
	Microsoft::WRL::ComPtr<ID3D11Device>			device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		context;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			swapchain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	render_target_view;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			depth_stencil_buffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	depth_stencil_view;

	std::unique_ptr<DebugRenderer>					debug_renderer;
	std::unique_ptr<ImGuiRenderer>					imgui_renderer;

	std::unique_ptr<SpriteShader>					sprite_shader;
	std::unique_ptr<AudioSource>					BGM;

	std::unique_ptr<Shader>							skybox_shader;

	std::unique_ptr<Texture>						scene_texture;
	std::mutex										mutex;
	float screen_width;
	float screen_height;
};