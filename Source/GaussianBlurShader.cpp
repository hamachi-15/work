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
	xblur = std::make_unique<GaussianXBlur>(device, static_cast<u_int>(2048 * 0.5f), static_cast<u_int>(1024), format);
	yblur = std::make_unique<GaussianYBlur>(device, static_cast<u_int>(2048 * 0.5f), static_cast<u_int>(1024 * 0.5f), format);

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
	Graphics& graphics = Graphics::Instance();
	if(BlurType::XBlur == type)
	{
		xblur->Begin(context);
		//	ブレンドステート設定
		context->OMSetBlendState(graphics.GetBlendState(static_cast<int>(Graphics::BlendState::Alpha)), nullptr, 0xFFFFFFFF);

		//ラスタライザ―設定
		context->RSSetState(graphics.GetRasterizerState(static_cast<int>(Graphics::RasterizerState::Cull_None)));

		//デプスステンシルステート設定
		context->OMSetDepthStencilState(graphics.GetDepthStencilState(static_cast<int>(Graphics::DepthStencilState::False)), 1);

		//　GPUに定数バッファを更新する
		context->UpdateSubresource(blur_constant_buffer.Get(), 0, NULL, &constant_buffer, 0, 0);
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
		static_cast<float>(texture->GetWidth()), static_cast<float>(texture->GetHeight()),
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
