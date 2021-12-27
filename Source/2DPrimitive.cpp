#include "Graphics.h"

#include "2DPrimitive.h"

//------------------------------
// コンストラクタ
//------------------------------
Primitive::Primitive(ID3D11Device* device)
{
	Graphics& graphics = Graphics::Instance();
	HRESULT hr = S_OK;

	Create(device, "Shader/2DPrimitive_vs.cso", "Shader/2DPrimitive_ps.cso", true);

	// 定数バッファ設定
	{
		D3D11_BUFFER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.ByteWidth = sizeof(ConstantBuffer);
		desc.StructureByteStride = 0;
		hr = device->CreateBuffer(&desc, 0, constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
}

//------------------------------
// 描画開始処理
//------------------------------
void Primitive::Begin(ID3D11DeviceContext* context, PrimitiveContext& cprimitive_context)
{
	Graphics& graphics = Graphics::Instance();
	Activate(context);

	//	ブレンドステート設定
	context->OMSetBlendState(graphics.GetBlendState(static_cast<int>(Graphics::BlendState::Alpha)), nullptr, 0xFFFFFFFF);

	//ラスタライザ―設定
	context->RSSetState(graphics.GetRasterizerState(static_cast<int>(Graphics::RasterizerState::Cull_Back)));

	//デプスステンシルステート設定
	context->OMSetDepthStencilState(graphics.GetDepthStencilState(static_cast<int>(Graphics::DepthStencilState::True)), 1);
	
	// コンスタントバッファ更新
	ConstantBuffer cbscene;
	cbscene.timer = cprimitive_context.timer;
	cbscene.number = cprimitive_context.number;

	context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
	context->PSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
	context->UpdateSubresource(constant_buffer.Get(), 0, 0, &cbscene, 0, 0);
}

//------------------------------
// 描画終了処理
//------------------------------
void Primitive::End(ID3D11DeviceContext* context)
{
	InActivate(context);
}
