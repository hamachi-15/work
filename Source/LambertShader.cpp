#include "Graphics.h"
#include "Misc.h"
#include "LambertShader.h"
#include "ModelResource.h"

//---------------------------------------
// コンストラクタ
//---------------------------------------
LambertShader::LambertShader(ID3D11Device* device)
{
	HRESULT hr = S_OK;
	Create(device, "Shader\\LambertShader_vs.cso", "Shader\\LambertShader_ps.cso", false);

	// 定数バッファ
	{
		// シーン用バッファ
		D3D11_BUFFER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.ByteWidth = sizeof(CbScene);
		desc.StructureByteStride = 0;

		hr = device->CreateBuffer(&desc, 0, scene_constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// メッシュ、サブセットバッファの生成
		CreateBuffer(device);
	}

	// サンプラステート
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
}

//---------------------------------------
// 描画開始
//---------------------------------------
void LambertShader::Begin(ID3D11DeviceContext* context, RenderContext& rc)
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

	//ラスタライザ―設定
	context->RSSetState(graphics.GetRasterizerState(static_cast<int>(Graphics::RasterizerState::Cull_Back)));
	//デプスステンシルステート設定
	context->OMSetDepthStencilState(graphics.GetDepthStencilState(static_cast<int>(Graphics::DepthStencilState::True)), 1);

	// サンプラーステート設定
	context->PSSetSamplers(0, 1, sampler_state.GetAddressOf());

	// シーン用定数バッファ更新
	CbScene cbScene;
	DirectX::XMMATRIX V = DirectX::XMLoadFloat4x4(&rc.view);
	DirectX::XMMATRIX P = DirectX::XMLoadFloat4x4(&rc.projection);
	DirectX::XMStoreFloat4x4(&cbScene.view_projection, V * P);

	cbScene.light_direction = rc.light_direction;
	context->UpdateSubresource(scene_constant_buffer.Get(), 0, 0, &cbScene, 0, 0);
}

//---------------------------------------
// 描画終了
//---------------------------------------
void LambertShader::End(ID3D11DeviceContext* context)
{
	InActivate(context);
}
