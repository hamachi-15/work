#include "SkyBoxShader.h"
#include "Misc.h"

//----------------------------------
// コンストラクタ
//----------------------------------
SkyBoxShader::SkyBoxShader(ID3D11Device* device)
{
	Create(device, "Shader\\SkyBox_vs.cso", "Shader\\SkyBox_ps.cso", true);
	
	// 定数バッファ
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.ByteWidth = sizeof(CBBuffers);
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = 0;

		HRESULT hr = device->CreateBuffer(&desc, NULL, skybox_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
}

//----------------------------------
// 描画開始
//----------------------------------
void SkyBoxShader::Begin(ID3D11DeviceContext* context, RenderContext* render_context)
{
	Activate(context);

	CBBuffers cb;
	DirectX::XMMATRIX v = DirectX::XMLoadFloat4x4(&render_context->view);
	DirectX::XMMATRIX p = DirectX::XMLoadFloat4x4(&render_context->projection);
	DirectX::XMStoreFloat4x4(&cb.inverse_view, XMMatrixInverse(nullptr, v));
	DirectX::XMStoreFloat4x4(&cb.inverse_projection, XMMatrixInverse(nullptr, p));
	cb.eye_position.x = cb.inverse_view._41;
	cb.eye_position.y = cb.inverse_view._42;
	cb.eye_position.z = cb.inverse_view._43;
	cb.eye_position.w = cb.inverse_view._44;
	context->UpdateSubresource(skybox_buffer.Get(), 0, NULL, &cb, 0, 0);
	context->VSSetConstantBuffers(0, 1, skybox_buffer.GetAddressOf());
	context->PSSetConstantBuffers(0, 1, skybox_buffer.GetAddressOf());
}

//----------------------------------
// 描画終了
//----------------------------------
void SkyBoxShader::End(ID3D11DeviceContext* context)
{
	InActivate(context);
}
