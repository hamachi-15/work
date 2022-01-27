#include "DissolveShader.h"
#include "Graphics.h"
#include "Texture.h"
#include "Sprite.h"

//**********************************
// 
// ディソープシェーダー
// 
//**********************************
//----------------------------------
// コンストラクタ
//----------------------------------
DissolveShader::DissolveShader(ID3D11Device* device)
{
	Graphics& graphics = Graphics::Instance();
	HRESULT hr = S_OK;

	// テクスチャ初期化
	//mask_texture = std::make_unique<Texture>();
	//mask_texture->Load("Data/Sprite/dissolve_animation2.png");

	// シェーダー初期化
	Create(device, "Shader\\EnvMask_vs.cso", "Shader\\EnvMask_ps.cso", true);

	// 定数バッファ設定
	{
		D3D11_BUFFER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.ByteWidth = sizeof(ConstantBufferForDissolve);
		desc.StructureByteStride = 0;
		hr = device->CreateBuffer(&desc, 0, constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
}

//----------------------------------
// 描画開始処理
//----------------------------------
void DissolveShader::Begin(ID3D11DeviceContext* context, float timer)
{
	Graphics& graphics = Graphics::Instance();

	Activate(context);

	//	ブレンドステート設定
	context->OMSetBlendState(graphics.GetBlendState(static_cast<int>(Graphics::BlendState::Alpha)), nullptr, 0xFFFFFFFF);

	//ラスタライザ―設定
	context->RSSetState(graphics.GetRasterizerState(static_cast<int>(Graphics::RasterizerState::Cull_Back)));

	//デプスステンシルステート設定
	context->OMSetDepthStencilState(graphics.GetDepthStencilState(static_cast<int>(Graphics::DepthStencilState::True)), 1);

	
	ConstantBufferForDissolve cbscene;
	cbscene.threshold = timer;

	context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
	context->PSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
	context->UpdateSubresource(constant_buffer.Get(), 0, 0, &cbscene, 0, 0);
}

//----------------------------------
// 描画処理
//----------------------------------
void DissolveShader::Render(ID3D11DeviceContext* context, Texture* texture, Texture* mask_texture)
{
	// テクスチャを設定
	mask_texture->Set(1);
	Sprite sprite;
	float texture_width = texture->GetWidth();
	float texture_height = texture->GetHeight();
	sprite.Render(context,
		texture,
		0, 0,
		texture_width, texture_height,
		0, 0,
		texture_width, texture_height);
}

//----------------------------------
// 描画終了処理
//----------------------------------
void DissolveShader::End(ID3D11DeviceContext* context)
{
	InActivate(context);

	// シェーダーリソースをクリア
	ID3D11ShaderResourceView* rtv[1] = { NULL };
	ID3D11SamplerState* ss[1] = { NULL };
	context->PSSetShaderResources(1, 1, rtv);

}
