#include <WICTextureLoader.h>
#include "Texture.h"
#include "Graphics.h"
#include "Misc.h"

Texture::Texture() :shader_resource_view(nullptr)
{
}

   
Texture::~Texture()
{
}


bool Texture::Load(const char* filename)
{
	ID3D11Device* device = Graphics::Instance().GetDevice();

	// マルチバイト文字からワイド文字へ変換
	wchar_t wfilename[256];
	::MultiByteToWideChar(CP_ACP, 0, filename, -1, wfilename, 256);

	// テクスチャ読み込み
	Microsoft::WRL::ComPtr<ID3D11Resource> resource;
	HRESULT hr = DirectX::CreateWICTextureFromFile(device, wfilename, resource.GetAddressOf(), shader_resource_view.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	
	// テクスチャ情報の取得
	D3D11_TEXTURE2D_DESC desc;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2d;
	hr = resource->QueryInterface<ID3D11Texture2D>(texture2d.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	texture2d->GetDesc(&desc);

	texture2d_desc.Width = desc.Width;
	texture2d_desc.Height = desc.Height;

	return false;
}


bool Texture::LoadMipMap(const wchar_t* filename)
{
	return false;
}


void Texture::Set(UINT slot)
{
	ID3D11DeviceContext* context = Graphics::Instance().GetDeviceContext();

	if (shader_resource_view)
	{
		context->PSSetShaderResources(slot, 1, shader_resource_view.GetAddressOf());
	}
}


bool Texture::Create(u_int width, u_int height, DXGI_FORMAT format)
{
	ID3D11Device* device = Graphics::Instance().GetDevice();

	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D;
	HRESULT hr = S_OK;
	// テクスチャ作成
	ZeroMemory(&texture2d_desc, sizeof(texture2d_desc));
	texture2d_desc.Width = width;
	texture2d_desc.Height = height;
	texture2d_desc.MipLevels = 1;
	texture2d_desc.ArraySize = 1;
	texture2d_desc.Format = format;
	texture2d_desc.SampleDesc.Count = 1;
	texture2d_desc.Usage = D3D11_USAGE_DEFAULT;
	texture2d_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	texture2d_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	hr = device->CreateTexture2D(&texture2d_desc, NULL, texture2D.GetAddressOf());
	assert(SUCCEEDED(hr));

	//	レンダーターゲットビュー作成
	D3D11_RENDER_TARGET_VIEW_DESC rtvd;
	ZeroMemory(&rtvd, sizeof(rtvd));
	rtvd.Format = format;
	rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvd.Texture2D.MipSlice = 0;
	hr = device->CreateRenderTargetView(texture2D.Get(), &rtvd, render_target_view.GetAddressOf());
	assert(SUCCEEDED(hr));

	//	シェーダーリソースビュー作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	ZeroMemory(&srvd, sizeof(srvd));
	srvd.Format = format;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MostDetailedMip = 0;
	srvd.Texture2D.MipLevels = 1;
	hr = device->CreateShaderResourceView(texture2D.Get(), &srvd, shader_resource_view.GetAddressOf());

	assert(SUCCEEDED(hr));

	return true;
}

bool Texture::CreateCube(u_int width, u_int height, DXGI_FORMAT format, int miplevel)
{
	ID3D11Device* device = Graphics::Instance().GetDevice();

	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D;
	HRESULT hr = S_OK;
	// テクスチャ作成
	ZeroMemory(&texture2d_desc, sizeof(texture2d_desc));
	texture2d_desc.Width = width;
	texture2d_desc.Height = height;
	texture2d_desc.MipLevels = miplevel;
	texture2d_desc.ArraySize = 6;
	texture2d_desc.Format = format;
	texture2d_desc.SampleDesc.Count = 1;
	texture2d_desc.SampleDesc.Quality = 0;
	texture2d_desc.Usage = D3D11_USAGE_DEFAULT;
	texture2d_desc.CPUAccessFlags = 0;
	texture2d_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texture2d_desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS |
		D3D11_RESOURCE_MISC_TEXTURECUBE;

	hr = device->CreateTexture2D(&texture2d_desc, NULL, texture2D.GetAddressOf());
	assert(SUCCEEDED(hr));


	//	シェーダーリソースビュー作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	ZeroMemory(&srvd, sizeof(srvd));
	srvd.Format = format;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	srvd.TextureCube.MostDetailedMip = 0;
	srvd.TextureCube.MipLevels = miplevel;
	hr = device->CreateShaderResourceView(texture2D.Get(), &srvd, shader_resource_view.GetAddressOf());

	assert(SUCCEEDED(hr));

	//	レンダーターゲットビュー作成
	D3D11_RENDER_TARGET_VIEW_DESC rtvd;
	ZeroMemory(&rtvd, sizeof(rtvd));
	rtvd.Format = format;
	rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;//2Dテクスチャ配列
	rtvd.Texture2DArray.FirstArraySlice = 0;
	rtvd.Texture2DArray.MipSlice = 0;
	rtvd.Texture2DArray.ArraySize = 6;
	hr = device->CreateRenderTargetView(texture2D.Get(), &rtvd, render_target_view.GetAddressOf());
	assert(SUCCEEDED(hr));

	return true;
}

// 深度ステンシルバッファ作成
bool Texture::CreateDepthStencil(u_int width, u_int height, DXGI_FORMAT format)
{
	ID3D11Device* device = Graphics::Instance().GetDevice();

	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D;
	HRESULT hr = S_OK;
	//	テクスチャ作成
	ZeroMemory(&texture2d_desc, sizeof(texture2d_desc));
	texture2d_desc.Width = width;
	texture2d_desc.Height = height;
	texture2d_desc.MipLevels = 1;
	texture2d_desc.ArraySize = 1;
	texture2d_desc.Format = format;
	texture2d_desc.SampleDesc.Count = 1;
	texture2d_desc.SampleDesc.Quality = 0;
	texture2d_desc.Usage = D3D11_USAGE_DEFAULT;
	texture2d_desc.CPUAccessFlags = 0;
	texture2d_desc.MiscFlags = 0;
	texture2d_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

	hr = device->CreateTexture2D(&texture2d_desc, NULL, texture2D.GetAddressOf());
	assert(SUCCEEDED(hr));

	// 深度ステンシルビュー設定
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Texture2D.MipSlice = 0;
	hr = device->CreateDepthStencilView(texture2D.Get(), &dsvd, depth_stencil_view.GetAddressOf());
	assert(SUCCEEDED(hr));

	//	シェーダーリソースビュー作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	ZeroMemory(&srvd, sizeof(srvd));
	srvd.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MostDetailedMip = 0;
	srvd.Texture2D.MipLevels = 1;
	hr = device->CreateShaderResourceView(texture2D.Get(), &srvd, shader_resource_view.GetAddressOf());

	assert(SUCCEEDED(hr));

	return true;
}
// 深度ステンシルバッファ作成
bool Texture::CreateCubeDepthStencil(u_int width, u_int height)
{
	ID3D11Device* device = Graphics::Instance().GetDevice();

	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D;
	HRESULT hr = S_OK;
	//	テクスチャ作成
	ZeroMemory(&texture2d_desc, sizeof(texture2d_desc));
	texture2d_desc.Width = width;
	texture2d_desc.Height = height;
	texture2d_desc.MipLevels = 1;
	texture2d_desc.ArraySize = 6;
	texture2d_desc.Format = DXGI_FORMAT_D32_FLOAT;
	texture2d_desc.SampleDesc.Count = 1;
	texture2d_desc.SampleDesc.Quality = 0;
	texture2d_desc.Usage = D3D11_USAGE_DEFAULT;
	texture2d_desc.CPUAccessFlags = 0;

	texture2d_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texture2d_desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	hr = device->CreateTexture2D(&texture2d_desc, NULL, texture2D.GetAddressOf());
	assert(SUCCEEDED(hr));

	//	デプスステンシルビュー作成
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = DXGI_FORMAT_D32_FLOAT;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;//2Dテクスチャ配列
	dsvd.Texture2DArray.FirstArraySlice = 0;
	dsvd.Texture2DArray.ArraySize = 6;
	dsvd.Texture2DArray.MipSlice = 0;
	dsvd.Flags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	hr = device->CreateDepthStencilView(texture2D.Get(), NULL, depth_stencil_view.GetAddressOf());
	assert(SUCCEEDED(hr));

	return true;
}
