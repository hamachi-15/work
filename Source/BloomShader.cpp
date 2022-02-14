#include "BloomShader.h"
#include "Graphics.h"
#include "Texture.h"
#include "GaussianBlurShader.h"
#include "ActorManager.h"
#include "Sprite.h"

//--------------------------------------
// �R���X�g���N�^
//--------------------------------------
Bloom::Bloom(ID3D11Device* device)
{
	Graphics& graphics = Graphics::Instance();
	HRESULT hr = S_OK;
	// �K�E�V�A���u���[�V�F�[�_�[������
	bulr = std::make_unique<GaussianBlur>(device, DXGI_FORMAT_R16G16B16A16_FLOAT);

	// �e�N�X�`��������
	bright_texture = std::make_unique<Texture>();
	bright_texture->Create(1280, 720, DXGI_FORMAT_R32G32B32A32_FLOAT);
	bloom_texture = std::make_unique<Texture>();
	bloom_texture->Create(1280, 720, DXGI_FORMAT_R32G32B32A32_FLOAT);
	depth_texture = std::make_unique<Texture>();
	depth_texture->CreateDepthStencil(1280, 720);

	Create(device, "Shader\\Bright_vs.cso", "Shader\\Bright_ps.cso", true);

	// �萔�o�b�t�@�ݒ�
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
}

//--------------------------------------
// �`��J�n����
//--------------------------------------
void Bloom::Begin(ID3D11DeviceContext* context)
{
	Graphics& graphics = Graphics::Instance();

	Activate(context);

	//	�u�����h�X�e�[�g�ݒ�
	context->OMSetBlendState(graphics.GetBlendState(static_cast<int>(Graphics::BlendState::Alpha)), nullptr, 0xFFFFFFFF);

	//���X�^���C�U�\�ݒ�
	context->RSSetState(graphics.GetRasterizerState(static_cast<int>(Graphics::RasterizerState::Cull_Back)));

	//�f�v�X�X�e���V���X�e�[�g�ݒ�
	context->OMSetDepthStencilState(graphics.GetDepthStencilState(static_cast<int>(Graphics::DepthStencilState::True)), 1);

	ConstantBufferForBloom cbscene;
	cbscene.threshold = 0.3f;

//	context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
	context->PSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
	context->UpdateSubresource(constant_buffer.Get(), 0, 0, &cbscene, 0, 0);
}

//--------------------------------------
// �P�x���o����
//--------------------------------------
Texture* Bloom::Render(ID3D11DeviceContext* context, RenderContext* render_context, Texture* texture)
{
	Graphics& graphics = Graphics::Instance();
	// �P�x���o�e�N�X�`���Ƀ����_�[�^�[�Q�b�g��ݒ�
	ID3D11RenderTargetView* render_terget_view = bright_texture->GetRenderTargetView();
	ID3D11DepthStencilView* depth_stencil_view = depth_texture->GetDepthStencilView();
	graphics.SetRenderTargetView(&render_terget_view, depth_stencil_view);
	graphics.ScreenClear(&render_terget_view, depth_stencil_view, {0, 0, 0, 1.0f});
	Sprite sprite;
	// �P�x���o
	if (texture == nullptr)
	{	// �A�N�^�[�̋P�x�Z�o
		ActorManager::Instance().BrightRender(render_context);
	}
	else
	{
		std::shared_ptr<Shader> print = ShaderManager::Instance().GetShader(ShaderManager::ShaderType::Sprite);
		print->Begin(context);
		sprite.Render(context,
			texture,
			0, 0,
			static_cast<float>(bright_texture->GetWidth()), static_cast<float>(bright_texture->GetHeight()),
			0, 0,
			static_cast<float>(texture->GetWidth()), static_cast<float>(texture->GetHeight()));
		print->End(context);
	}

	{
		ID3D11RenderTargetView* render_terget_view = bloom_texture->GetRenderTargetView();
		ID3D11DepthStencilView* depth_stencil_view = depth_texture->GetDepthStencilView();
		graphics.SetRenderTargetView(&render_terget_view, depth_stencil_view);
		graphics.ScreenClear(&render_terget_view, depth_stencil_view, { 0, 0, 0, 1.0f });
	}

	float bright_texture_width = static_cast<float>(bright_texture->GetWidth());
	float bright_texture_height = static_cast<float>(bright_texture->GetHeight());

	Begin(context);
	sprite.Render(context,
		bright_texture.get(),
		0, 0,
		bright_texture_width, bright_texture_height,
		0, 0,
		bright_texture_width, bright_texture_height);
	End(context);

	return bulr->Render(bloom_texture.get());
}

//--------------------------------------
// �`��I������
//--------------------------------------
void Bloom::End(ID3D11DeviceContext* context)
{
	InActivate(context);
}
