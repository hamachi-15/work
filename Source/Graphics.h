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

class Texture;

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

	// デプスステンシルステート取得
	ID3D11DepthStencilState* GetDepthStencilState(int state) { return depth_stencil_state[state].Get(); }

	// ラステライザーステート取得
	ID3D11RasterizerState* GetRasterizerState(int state) { return rasterizer_state[state].Get(); }

	// ブレンドステート取得
	ID3D11BlendState* GetBlendState(int state) { return blend_state[state].Get(); }

	// スクリーン高さ取得
	float GetScreenHeight() const { return screen_height; }

	// スクリーン幅取得
	float GetScreenWidth() const { return screen_width; }

	// ImGuiレンダラ取得
	ImGuiRenderer* GetImGuiRenderer() const { return imgui_renderer.get(); }

	// デバックレンダラー取得
	DebugRenderer* GetDebugRenderer() const { return debug_renderer.get(); }

	AudioSource* GetAudio() const { return BGM.get(); }

	// シーンテクスチャ取得
	Texture* GetTexture() const { return scene_texture.get(); }

	// ミューテクス取得
	std::mutex& GetMutex() { return mutex; }

	// ビューポート設定
	void SetViewport(float weidth, float height);

	// レンダーターゲット設定
	void SetRenderTargetView(ID3D11RenderTargetView* render_target_view[], ID3D11DepthStencilView* depth_stensil_view);

	// 画面クリア
	void ScreenClear(ID3D11RenderTargetView* render_target_view[], ID3D11DepthStencilView* depth_stensil_view, DirectX::XMFLOAT4 clear_color = {1.0f, 1.0f, 1.0f, 1.0f});

public:
	enum class DepthStencilState
	{
		False,
		True,
		Write_False
	};
	enum class RasterizerState
	{
		Cull_Back,
		Wire,
		Cull_Front,
		Cull_None
	};
	enum class BlendState
	{
		None,
		Alpha,
		Add,
		Subtract,
		Replace,
		Multiply,
		Lighten,
		Darken,
		Screen
	};
private:
	enum
	{
		Depth_Stencil_Type = 3,
		Rasterize_Type = 4,
		Blend_Type = 9
	};
	static Graphics* instance;
	Microsoft::WRL::ComPtr<ID3D11Device>			device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		context;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			swapchain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	render_target_view;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			depth_stencil_buffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	depth_stencil_view;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depth_stencil_state[Depth_Stencil_Type];
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_state[Rasterize_Type];
	Microsoft::WRL::ComPtr<ID3D11BlendState> blend_state[Blend_Type];
	
	// 深度ステンシルステート作成
	bool CreateDeptthStencilState();

	// ラステライザーステート作成
	bool CreateRasterizerState();

	// ブレンドステート作成
	bool CreateBlendState();

	std::unique_ptr<DebugRenderer>					debug_renderer;
	std::unique_ptr<ImGuiRenderer>					imgui_renderer;

	std::unique_ptr<AudioSource>					BGM;

	std::unique_ptr<Texture>						scene_texture;
	std::mutex										mutex;
	float screen_width;
	float screen_height;
};