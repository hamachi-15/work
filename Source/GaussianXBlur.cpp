#include "GaussianXBlur.h"
#include "Graphics.h"

GaussianXBlur::GaussianXBlur(ID3D11Device* device, u_int buffer_width, u_int buffer_height, DXGI_FORMAT format)
{
	Create(device, "Shader\\GaussianXBlur_vs.cso", "Shader\\GaussianBlur_ps.cso", true);

	xblur_texture = std::make_unique<Texture>();
	xblur_texture->Create(buffer_width, buffer_height, format);
	depth_texture = std::make_unique<Texture>();
	depth_texture->CreateDepthStencil(buffer_width, buffer_height);

}

void GaussianXBlur::Begin(ID3D11DeviceContext* context, BlurRenderContext& bulr_render_context)
{
	ID3D11RenderTargetView* rtv[1] = { xblur_texture->GetRenderTargetView() };
	ID3D11DepthStencilView* dsv = depth_texture->GetDepthStencilView();
	context->OMSetRenderTargets(1, rtv, dsv);

	// 画面クリア
	float clear_color[4] = { 1.0f,1.0f,1.0f,1.0f };
	context->ClearRenderTargetView(rtv[0], clear_color);
	context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// ビューポートの設定
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)xblur_texture->GetWidth();
	vp.Height = (FLOAT)xblur_texture->GetHeight();
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	context->RSSetViewports(1, &vp);

	Activate(context);
}

void GaussianXBlur::End(ID3D11DeviceContext* context)
{
	Graphics& graphics = Graphics::Instance();
	
	InActivate(context);

	// レンダーターゲットの回復
	ID3D11RenderTargetView* backbuffer = graphics.GetRenderTargetView();
	context->OMSetRenderTargets(1, &backbuffer, graphics.GetDepthStencilView());
}
