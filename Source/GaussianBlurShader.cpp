#include "GaussianBlurShader.h"
#include "Misc.h"
#include "GaussianXBlur.h"
#include "GaussianYBlur.h"
#include "Graphics.h"
#include "Sprite.h"

//------------------------------------
// コンストラクタ
//------------------------------------
GaussianBlur::GaussianBlur(ID3D11Device* device, DXGI_FORMAT format)
{
	Graphics& graphics = Graphics::Instance();
	HRESULT hr = S_OK;
	// スクリーンサイズ取得
	float screen_width = graphics.GetScreenWidth();
	float screen_height = graphics.GetScreenHeight();

	// 縦ブラーと横ブラー初期化
	xblur = std::make_unique<GaussianXBlur>(device, static_cast<u_int>(1024 * 0.5f), static_cast<u_int>(1024), format);
	yblur = std::make_unique<GaussianYBlur>(device, static_cast<u_int>(1024 * 0.5f), static_cast<u_int>(1024* 0.5f), format);

	// 定数バッファ
	{
		D3D11_BUFFER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.ByteWidth = sizeof(CBForPerGaussainBlur);
		desc.StructureByteStride = 0;
		hr = device->CreateBuffer(&desc, 0, blur_constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		CalcGaussianFilter(constant_buffer.weight, blur_power);
	}
	// ブレンドステート
	{
		D3D11_BLEND_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.IndependentBlendEnable = false;
		desc.AlphaToCoverageEnable = false;
		desc.RenderTarget[0].BlendEnable = false;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = device->CreateBlendState(&desc, blend_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	}
	// ラステライザーステート
	{
		D3D11_RASTERIZER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_NONE;
		desc.FrontCounterClockwise = FALSE;
		desc.DepthBias = 0;
		desc.DepthBiasClamp = 0;
		desc.SlopeScaledDepthBias = 0;
		desc.DepthClipEnable = TRUE;
		desc.ScissorEnable = FALSE;
		desc.MultisampleEnable = FALSE;
		desc.AntialiasedLineEnable = FALSE;
		device->CreateRasterizerState(&desc, rasterizer_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
	// 深度ステンシルステート
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.DepthEnable = FALSE;
		hr = device->CreateDepthStencilState(&desc, depth_stencil_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

}

//------------------------------------
// デストラクタ
//------------------------------------
GaussianBlur::~GaussianBlur()
{
}

//------------------------------------
// 描画開始処理
//------------------------------------
void GaussianBlur::Begin(ID3D11DeviceContext* context,	BlurType type)
{
	if(BlurType::XBlur == type) 
	{
		xblur->Begin(context);
		//	ブレンドステート設定
		context->OMSetBlendState(blend_state.Get(), nullptr, 0xFFFFFFFF);

		//ラスタライザ―設定
		context->RSSetState(rasterizer_state.Get());

		//デプスステンシルステート設定
		context->OMSetDepthStencilState(depth_stencil_state.Get(), 1);

		//　GPUに定数バッファを更新する
		context->UpdateSubresource(blur_constant_buffer.Get(), 0, NULL, &constant_buffer, 0, 0);
		//context->VSSetConstantBuffers(0, 1, blur_constant_buffer.GetAddressOf());
		context->PSSetConstantBuffers(0, 1, blur_constant_buffer.GetAddressOf());
	}
	else if (BlurType::YBlur == type)
	{
		yblur->Begin(context, xblur->GetBlurXTexture());
	}
}

//------------------------------------
// ブラー処理
//------------------------------------
Texture* GaussianBlur::Render(Texture* texture)
{
	Sprite sprite;
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* context = graphics.GetDeviceContext();
	Texture* width_blur_texture = GetGaussianXBlurShader()->GetBlurXTexture();
	Texture* height_blur_texture = GetGaussianYBlurShader()->GetBlurYTexture();

	float width_blur_texture_width = static_cast<float>(width_blur_texture->GetWidth());
	float width_blur_texture_height = static_cast<float>(width_blur_texture->GetHeight());
	float height_blur_texture_width = static_cast<float>(height_blur_texture->GetWidth());
	float height_blur_texture_height = static_cast<float>(height_blur_texture->GetHeight());

	// X方向にブラーを掛ける
	Begin(context, BlurType::XBlur);
	sprite.Render(context, texture,
		0, 0,
		width_blur_texture_width, width_blur_texture_height,
		0, 0,
		(float)texture->GetWidth(), (float)texture->GetHeight(),
		0,
		1, 1, 1, 1);
	End(context, BlurType::XBlur);

	// Y方向にブラーを掛ける
	Begin(context, BlurType::YBlur);
	sprite.Render(context, 
		width_blur_texture,
		0, 0,
		height_blur_texture_width, height_blur_texture_height,
		0, 0,
		width_blur_texture_width, width_blur_texture_height);
	End(context, BlurType::YBlur);

	return height_blur_texture;
}

//------------------------------------
// 描画終了処理
//------------------------------------
void GaussianBlur::End(ID3D11DeviceContext* context, BlurType type)
{
	if (BlurType::XBlur == type)
	{
		xblur->End(context);
	}
	else if (BlurType::YBlur == type)
	{
		yblur->End(context);
	}
}

//------------------------------------
// 重みを計算
//------------------------------------
void GaussianBlur::CalcGaussianFilter(float* weight, float blur_power)
{
	float total = 0;
	for (int i = 0; i < NumWeights; ++i)
	{
		weight[i] = expf(-0.5f * (float)(i * i) / blur_power);
		total += 2.0f * weight[i];
	}
	// 規格化
	for (int i = 0; i < NumWeights; ++i)
	{
		weight[i] /= total;
	}
}
