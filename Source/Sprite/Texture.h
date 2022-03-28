#pragma once
#include <wrl.h>
#include <d3d11.h>

class Texture
{
public:
	Texture();
	virtual ~Texture();
	bool Load(const char* filename = nullptr);
	bool LoadMipMap(const wchar_t* filename = nullptr);

	void Set(UINT slot = 0);

	UINT GetWidth() { return texture2d_desc.Width; }
	UINT GetHeight() { return texture2d_desc.Height; }
	bool Create(u_int width, u_int height, DXGI_FORMAT format);
	bool CreateCube(u_int width, u_int height, DXGI_FORMAT format, int miplevel);

	// 深度ステンシルバッファ作成
	bool CreateDepthStencil(u_int width, u_int height, DXGI_FORMAT format = DXGI_FORMAT_R24G8_TYPELESS);
	bool CreateCubeDepthStencil(u_int width, u_int height);

	ID3D11RenderTargetView* GetRenderTargetView() { return render_target_view.Get(); }
	ID3D11DepthStencilView* GetDepthStencilView() { return depth_stencil_view.Get(); }
protected:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_view = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> render_target_view = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depth_stencil_view = nullptr;

	// テクスチャ情報
	D3D11_TEXTURE2D_DESC texture2d_desc;

	int texture_width = 0;
	int texture_height = 0;

};