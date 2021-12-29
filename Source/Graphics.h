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
	// �R���X�g���N�^
	Graphics(HWND hwnd);
	
	// �f�X�g���N�^
	~Graphics();

	// �C���X�^���X�擾
	static Graphics& Instance() { return  *instance; }

	// �f�o�C�X�擾
	ID3D11Device* GetDevice() const { return device.Get(); }

	// �f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext* GetDeviceContext() const { return context.Get(); }

	// �X���b�v�`�F�[���擾
	IDXGISwapChain* GetSwapChain() const { return swapchain.Get(); }

	// �����_�[�^�[�Q�b�g�r���[�擾
	ID3D11RenderTargetView* GetRenderTargetView() const { return render_target_view.Get(); }

	// �f�v�X�X�e���V���r���[�擾
	ID3D11DepthStencilView* GetDepthStencilView() const { return depth_stencil_view.Get(); }

	// �f�v�X�X�e���V���X�e�[�g�擾
	ID3D11DepthStencilState* GetDepthStencilState(int state) { return depth_stencil_state[state].Get(); }

	// ���X�e���C�U�[�X�e�[�g�擾
	ID3D11RasterizerState* GetRasterizerState(int state) { return rasterizer_state[state].Get(); }

	// �u�����h�X�e�[�g�擾
	ID3D11BlendState* GetBlendState(int state) { return blend_state[state].Get(); }

	// �X�N���[�������擾
	float GetScreenHeight() const { return screen_height; }

	// �X�N���[�����擾
	float GetScreenWidth() const { return screen_width; }

	// ImGui�����_���擾
	ImGuiRenderer* GetImGuiRenderer() const { return imgui_renderer.get(); }

	// �f�o�b�N�����_���[�擾
	DebugRenderer* GetDebugRenderer() const { return debug_renderer.get(); }

	AudioSource* GetAudio() const { return BGM.get(); }

	// �V�[���e�N�X�`���擾
	Texture* GetTexture() const { return scene_texture.get(); }

	// �~���[�e�N�X�擾
	std::mutex& GetMutex() { return mutex; }

	// �r���[�|�[�g�ݒ�
	void SetViewport(float weidth, float height);

	// �����_�[�^�[�Q�b�g�ݒ�
	void SetRenderTargetView(ID3D11RenderTargetView* render_target_view[], ID3D11DepthStencilView* depth_stensil_view);

	// ��ʃN���A
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
	
	// �[�x�X�e���V���X�e�[�g�쐬
	bool CreateDeptthStencilState();

	// ���X�e���C�U�[�X�e�[�g�쐬
	bool CreateRasterizerState();

	// �u�����h�X�e�[�g�쐬
	bool CreateBlendState();

	std::unique_ptr<DebugRenderer>					debug_renderer;
	std::unique_ptr<ImGuiRenderer>					imgui_renderer;

	std::unique_ptr<AudioSource>					BGM;

	std::unique_ptr<Texture>						scene_texture;
	std::mutex										mutex;
	float screen_width;
	float screen_height;
};