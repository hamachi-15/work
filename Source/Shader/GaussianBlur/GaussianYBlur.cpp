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
	// �����_�[�^�[�Q�b�g�ݒ�
	ID3D11RenderTargetView* render_target_view[1] = { yblur_texture->GetRenderTargetView() };
	ID3D11DepthStencilView* depth_stensil_view = depth_texture->GetDepthStencilView();
	graphics.SetRenderTargetView(render_target_view, depth_stensil_view);

	// ��ʃN���A
	graphics.ScreenClear(render_target_view, depth_stensil_view);

	// �r���[�|�[�g�̐ݒ�
	graphics.SetViewport(static_cast<float>(yblur_texture->GetWidth()), static_cast<float>(yblur_texture->GetHeight()));

	// �V�F�[�_�[�̐ݒ�
	Activate(context);
}

void GaussianYBlur::End(ID3D11DeviceContext* context)
{
	// �V�F�[�_�[�̐ݒ�
	InActivate(context);
}