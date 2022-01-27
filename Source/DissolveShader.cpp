#include "DissolveShader.h"
#include "Graphics.h"
#include "Texture.h"
#include "Sprite.h"

//**********************************
// 
// �f�B�\�[�v�V�F�[�_�[
// 
//**********************************
//----------------------------------
// �R���X�g���N�^
//----------------------------------
DissolveShader::DissolveShader(ID3D11Device* device)
{
	Graphics& graphics = Graphics::Instance();
	HRESULT hr = S_OK;

	// �e�N�X�`��������
	//mask_texture = std::make_unique<Texture>();
	//mask_texture->Load("Data/Sprite/dissolve_animation2.png");

	// �V�F�[�_�[������
	Create(device, "Shader\\EnvMask_vs.cso", "Shader\\EnvMask_ps.cso", true);

	// �萔�o�b�t�@�ݒ�
	{
		D3D11_BUFFER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.ByteWidth = sizeof(ConstantBufferForDissolve);
		desc.StructureByteStride = 0;
		hr = device->CreateBuffer(&desc, 0, constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
}

//----------------------------------
// �`��J�n����
//----------------------------------
void DissolveShader::Begin(ID3D11DeviceContext* context, float timer)
{
	Graphics& graphics = Graphics::Instance();

	Activate(context);

	//	�u�����h�X�e�[�g�ݒ�
	context->OMSetBlendState(graphics.GetBlendState(static_cast<int>(Graphics::BlendState::Alpha)), nullptr, 0xFFFFFFFF);

	//���X�^���C�U�\�ݒ�
	context->RSSetState(graphics.GetRasterizerState(static_cast<int>(Graphics::RasterizerState::Cull_Back)));

	//�f�v�X�X�e���V���X�e�[�g�ݒ�
	context->OMSetDepthStencilState(graphics.GetDepthStencilState(static_cast<int>(Graphics::DepthStencilState::True)), 1);

	
	ConstantBufferForDissolve cbscene;
	cbscene.threshold = timer;

	context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
	context->PSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
	context->UpdateSubresource(constant_buffer.Get(), 0, 0, &cbscene, 0, 0);
}

//----------------------------------
// �`�揈��
//----------------------------------
void DissolveShader::Render(ID3D11DeviceContext* context, Texture* texture, Texture* mask_texture)
{
	// �e�N�X�`����ݒ�
	mask_texture->Set(1);
	Sprite sprite;
	float texture_width = texture->GetWidth();
	float texture_height = texture->GetHeight();
	sprite.Render(context,
		texture,
		0, 0,
		texture_width, texture_height,
		0, 0,
		texture_width, texture_height);
}

//----------------------------------
// �`��I������
//----------------------------------
void DissolveShader::End(ID3D11DeviceContext* context)
{
	InActivate(context);

	// �V�F�[�_�[���\�[�X���N���A
	ID3D11ShaderResourceView* rtv[1] = { NULL };
	ID3D11SamplerState* ss[1] = { NULL };
	context->PSSetShaderResources(1, 1, rtv);

}
