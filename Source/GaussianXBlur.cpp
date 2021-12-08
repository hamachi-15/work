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

void GaussianXBlur::Begin(ID3D11DeviceContext* context)
{
	Graphics& graphics = Graphics::Instance();
	// �����_�[�^�[�Q�b�g�ݒ�
	ID3D11RenderTargetView* render_target_view[1] = { xblur_texture->GetRenderTargetView() };
	ID3D11DepthStencilView* depth_stensil_view = depth_texture->GetDepthStencilView();
	graphics.SetRenderTargetView(render_target_view, depth_stensil_view);

	// ��ʃN���A
	graphics.ScreenClear(render_target_view, depth_stensil_view);

	// �r���[�|�[�g�̐ݒ�
	graphics.SetViewport(static_cast<float>(xblur_texture->GetWidth()), static_cast<float>(xblur_texture->GetHeight()));

	// �V�F�[�_�[�̐ݒ�
	Activate(context);
}

void GaussianXBlur::End(ID3D11DeviceContext* context)
{
	Graphics& graphics = Graphics::Instance();
	
	// �V�F�[�_�[�̐ݒ�
	InActivate(context);

	// �����_�[�^�[�Q�b�g�̉�
	ID3D11RenderTargetView* backbuffer = graphics.GetRenderTargetView();
	graphics.SetRenderTargetView(&backbuffer, graphics.GetDepthStencilView());
}
