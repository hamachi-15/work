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

void GaussianYBlur::Begin(ID3D11DeviceContext* context, Texture* texture)
{
	Graphics& graphics = Graphics::Instance();
	// レンダーターゲット設定
	ID3D11RenderTargetView* render_target_view[1] = { yblur_texture->GetRenderTargetView() };
	ID3D11DepthStencilView* depth_stensil_view = depth_texture->GetDepthStencilView();
	graphics.SetRenderTargetView(render_target_view, depth_stensil_view);

	// 画面クリア
	graphics.ScreenClear(render_target_view, depth_stensil_view);

	// ビューポートの設定
	graphics.SetViewport(static_cast<float>(yblur_texture->GetWidth()), static_cast<float>(yblur_texture->GetHeight()));

	// シェーダーの設定
	Activate(context);
}

void GaussianYBlur::End(ID3D11DeviceContext* context)
{
	// シェーダーの設定
	InActivate(context);
}
