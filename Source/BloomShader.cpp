#include "BloomShader.h"
#include "Graphics.h"
#include "Texture.h"
#include "GaussianBlurShader.h"
#include "ActorManager.h"
#include "Sprite.h"

//--------------------------------------
// コンストラクタ
//--------------------------------------
Bloom::Bloom(ID3D11Device* device)
{
	Graphics& graphics = Graphics::Instance();
	HRESULT hr = S_OK;
	// ガウシアンブラーシェーダー初期化
	bulr = std::make_unique<GaussianBlur>(device, DXGI_FORMAT_R16G16B16A16_FLOAT);

	// テクスチャ初期化
	bright_texture = std::make_unique<Texture>();
	bright_texture->Create(1280, 720, DXGI_FORMAT_R32G32B32A32_FLOAT);
	bloom_texture = std::make_unique<Texture>();
	bloom_texture->Create(1280, 720, DXGI_FORMAT_R32G32B32A32_FLOAT);
	depth_texture = std::make_unique<Texture>();
	depth_texture->CreateDepthStencil(1280, 720);


	Create(device, "Shader\\Bright_vs.cso", "Shader\\Bright_ps.cso", true);

	// 定数バッファ設定
	{
		D3D11_BUFFER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.ByteWidth = sizeof(ConstantBufferForBloom);
		desc.StructureByteStride = 0;
		hr = device->CreateBuffer(&desc, 0, constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
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

//--------------------------------------
// 描画開始処理
//--------------------------------------
void Bloom::Begin(ID3D11DeviceContext* context)
{
	Activate(context);

	//	ブレンドステート設定
	context->OMSetBlendState(blend_state.Get(), nullptr, 0xFFFFFFFF);

	//ラスタライザ―設定
	context->RSSetState(rasterizer_state.Get());

	//デプスステンシルステート設定
	context->OMSetDepthStencilState(depth_stencil_state.Get(), 1);

	ConstantBufferForBloom cbscene;
	cbscene.threshold = 0;

	context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
	context->PSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
	context->UpdateSubresource(constant_buffer.Get(), 0, 0, &cbscene, 0, 0);

}

//--------------------------------------
// 輝度抽出処理
//--------------------------------------
Texture* Bloom::Render(ID3D11DeviceContext* context, RenderContext& render_context)
{
	Graphics& graphics = Graphics::Instance();
	// 輝度抽出テクスチャにレンダーターゲットを設定
	ID3D11RenderTargetView* render_terget_view = bright_texture->GetRenderTargetView();
	ID3D11DepthStencilView* depth_stencil_view = depth_texture->GetDepthStencilView();
	graphics.SetRenderTargetView(&render_terget_view, depth_stencil_view);
	graphics.ScreenClear(&render_terget_view, depth_stencil_view, {0, 0, 0, 1.0f});
	// 輝度抽出
	ActorManager::Instance().BrightRender(render_context);

	Sprite sprite;
	{
		ID3D11RenderTargetView* render_terget_view = bloom_texture->GetRenderTargetView();
		ID3D11DepthStencilView* depth_stencil_view = depth_texture->GetDepthStencilView();
		graphics.SetRenderTargetView(&render_terget_view, depth_stencil_view);
		graphics.ScreenClear(&render_terget_view, depth_stencil_view, { 0, 0, 0, 1.0f });
	}

	Begin(context);
	sprite.Render(context,
		bright_texture.get(),
		0, 0,
		bright_texture->GetWidth(), bright_texture->GetHeight(),
		0, 0,
		bright_texture->GetWidth(), bright_texture->GetHeight());
	End(context);

	//bloom_texture.reset(bulr->Render(bright_texture.get()));

	return bulr->Render(bloom_texture.get());
}

//--------------------------------------
// 描画終了処理
//--------------------------------------
void Bloom::End(ID3D11DeviceContext* context)
{
	InActivate(context);
}
