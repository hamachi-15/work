#include "VarianceShadowMap.h"
#include "Graphics.h"
#include "Camera.h"
#include "ModelResource.h"

VarianceShadowMap::VarianceShadowMap(ID3D11Device* device)
{
	HRESULT hr = S_OK;
	Create(device, "Shader\\VarianceShadowMap_vs.cso", "Shader\\VarianceShadowMap_ps.cso", false);

	// 定数バッファ
	{
		// シーン用バッファ生成
		D3D11_BUFFER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.ByteWidth = sizeof(CBScene);
		desc.StructureByteStride = 0;

		hr = device->CreateBuffer(&desc, 0, scene_constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		
		// メッシュ、サブセットバッファの生成
		CreateBuffer(device);
	}

	// サンプラーステート
	{
		D3D11_SAMPLER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.MipLODBias = 0.0f;
		desc.MaxAnisotropy = 1;
		desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		desc.MinLOD = -FLT_MAX;
		desc.MaxLOD = FLT_MAX;
		desc.BorderColor[0] = 1.0f;
		desc.BorderColor[1] = 1.0f;
		desc.BorderColor[2] = 1.0f;
		desc.BorderColor[3] = 1.0f;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

		hr = device->CreateSamplerState(&desc, sampler_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
	// テクスチャ作成
	{
		shadow_texture = std::make_unique<Texture>();
		shadow_texture->Create(ShadowBuffer, ShadowBuffer, DXGI_FORMAT_R16G16_FLOAT);
		shadow_texture = std::make_unique<Texture>();
		shadow_texture->Create(ShadowBuffer, ShadowBuffer, DXGI_FORMAT_R16G16_FLOAT);
		shadow_depth_texture = std::make_unique<Texture>();
		shadow_depth_texture->CreateDepthStencil(ShadowBuffer, ShadowBuffer);
	}
}

// 描画開始処理
void VarianceShadowMap::Begin(ID3D11DeviceContext* context, RenderContext& render_context)
{
	Graphics& graphics = Graphics::Instance();

	Activate(context);

	ID3D11Buffer* constantBuffers[] =
	{
		scene_constant_buffer.Get(),
		mesh_constant_buffer.Get(),
		subset_constant_buffer.Get()
	};
	context->VSSetConstantBuffers(0, ARRAYSIZE(constantBuffers), constantBuffers);
	context->PSSetConstantBuffers(0, ARRAYSIZE(constantBuffers), constantBuffers);
	
	//ブレンドステート設定
	context->OMSetBlendState(graphics.Instance().GetBlendState(static_cast<int>(Graphics::BlendState::Alpha)), nullptr, 0xFFFFFFFF);
	//デプスステンシルステート設定
	context->OMSetDepthStencilState(graphics.Instance().GetDepthStencilState(static_cast<int>(Graphics::DepthStencilState::True)), 1);
	//ラスタライザ―設定
	context->RSSetState(graphics.Instance().GetRasterizerState(static_cast<int>(Graphics::RasterizerState::Cull_Back)));
	// サンプラーステート設定
	context->PSSetSamplers(0, 1, sampler_state.GetAddressOf());

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// シーン用定数バッファ更新
	CBScene cbscene;
	// TODO 変更したカメラのビューとプロジェクションの設定
	//DirectX::XMStoreFloat4x4(&cbScene.view_projection, matview * pm);
	//render_context.light_view_projection = cbScene.view_projection;
	//DirectX::XMMATRIX V = DirectX::XMLoadFloat4x4(&render_context.view);
	//DirectX::XMMATRIX P = DirectX::XMLoadFloat4x4(&render_context.projection);
	//DirectX::XMStoreFloat4x4(&cbscene.view_projection, V * P);
	cbscene.view_projection = render_context.single_light_view_projection;
	context->UpdateSubresource(scene_constant_buffer.Get(), 0, 0, &cbscene, 0, 0);
}

// 描画終了処理
void VarianceShadowMap::End(ID3D11DeviceContext* context)
{
	InActivate(context);

}