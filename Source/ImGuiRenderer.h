#pragma once

#include <vector>
#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <imgui.h>

class ImGuiRenderer
{
public:
	ImGuiRenderer(HWND hwnd, ID3D11Device* device);
	~ImGuiRenderer();

public:
	// �t���[���J�n����
	void NewFrame();

	// �`�揈��
	void Render(ID3D11DeviceContext* context);

	// WIN32���b�Z�[�W�n���h���[
	LRESULT HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
	// �}�E�X�J�[�\���X�V
	bool UpdateMouseCursor();

	// �}�E�X���W�X�V
	bool UpdateMousePos();
private:
	struct ConstantBuffer
	{
		DirectX::XMFLOAT4X4		world_view_projection;
	};

	HWND												hwnd;
	INT64												time = 0;
	INT64												ticks_per_second = 0;
	ImGuiMouseCursor									last_mouse_cursor = ImGuiMouseCursor_COUNT;

	Microsoft::WRL::ComPtr<ID3D11Buffer>				vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>				index_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>				constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>			vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			input_layout;

	Microsoft::WRL::ComPtr<ID3D11BlendState>			blend_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		depth_stencil_state;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>			sampler_state;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	shader_resource_view;

	int													vertex_count = 0;
	int													index_count = 0;
};
