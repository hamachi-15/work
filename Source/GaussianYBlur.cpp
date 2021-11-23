#include "GaussianYBlur.h"
#include "Graphics.h"

GaussianYBlur::GaussianYBlur(ID3D11Device* device, u_int buffer_width, u_int buffer_height, DXGI_FORMAT format)
{
	Create(device, "Shader\\GaussianYBlur_vs.cso", "Shader\\GaussianBlur_ps.cso", true);
	
	yblur_texture = std::make_unique<Texture>();
	yblur_texture->Create(buffer_width, buffer_height, format);
	depth_texture = std::make_unique<Texture>();
	depth_texture->CreateDepthStencil(buffer_width, buffer_height);

}

void GaussianYBlur::Begin(ID3D11DeviceContext* context, BlurRenderContext& bulr_render_context, Texture* texture)
{
	ID3D11RenderTargetView* rtv[1] = { yblur_texture->GetRenderTargetView() };
	//ID3D11RenderTargetView* rtv[1] = { texture.GetRenderTargetView() };
	ID3D11DepthStencilView* dsv = depth_texture->GetDepthStencilView();
	context->OMSetRenderTargets(1, rtv, dsv);

	// 画面クリア
	float clear_color[4] = { 1.0f,1.0f,1.0f,1.0f };
	context->ClearRenderTargetView(rtv[0], clear_color);
	context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// ビューポートの設定
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)yblur_texture->GetWidth();
	vp.Height = (FLOAT)yblur_texture->GetHeight();
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	context->RSSetViewports(1, &vp);
	
	Activate(context);
}

void GaussianYBlur::End(ID3D11DeviceContext* context)
{
	Graphics& graphics = Graphics::Instance();

	InActivate(context);

	// レンダーターゲットの回復
	ID3D11RenderTargetView* backbuffer = graphics.GetRenderTargetView();
	context->OMSetRenderTargets(1, &backbuffer, graphics.GetDepthStencilView());
}
