#include "Graphics.h"
#include "CreateShadowMapShader.h"

//-------------------------------
// �R���X�g���N�^
//-------------------------------
CreateShadowMap::CreateShadowMap(ID3D11Device* device)
{
	HRESULT hr = S_OK;
	Create(device, "Shader\\ToShadowMap_vs.cso", "Shader\\ToShadowMap_ps.cso", false);

	// �萔�o�b�t�@
	{
		// �V�[���p�o�b�t�@����
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

		// ���b�V���A�T�u�Z�b�g�o�b�t�@�̐���
		CreateBuffer(device);
	}
	// �T���v���[�X�e�[�g
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

//-------------------------------
// �`��J�n����
//-------------------------------
void CreateShadowMap::Begin(ID3D11DeviceContext* context, RenderContext& render_context)
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

	//�u�����h�X�e�[�g�ݒ�
	context->OMSetBlendState(graphics.Instance().GetBlendState(static_cast<int>(Graphics::BlendState::Alpha)), nullptr, 0xFFFFFFFF);
	//�f�v�X�X�e���V���X�e�[�g�ݒ�
	context->OMSetDepthStencilState(graphics.Instance().GetDepthStencilState(static_cast<int>(Graphics::DepthStencilState::True)), 1);
	//���X�^���C�U�\�ݒ�
	context->RSSetState(graphics.Instance().GetRasterizerState(static_cast<int>(Graphics::RasterizerState::Cull_Back)));
	// �T���v���[�X�e�[�g�ݒ�
	context->PSSetSamplers(0, 1, sampler_state.GetAddressOf());

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// �V�[���p�萔�o�b�t�@�X�V
	CBScene cbscene;
	cbscene.view_projection = render_context.single_light_view_projection;
	context->UpdateSubresource(scene_constant_buffer.Get(), 0, 0, &cbscene, 0, 0);
}

//-------------------------------
// �`��I������
//-------------------------------
void CreateShadowMap::End(ID3D11DeviceContext* context)
{
	InActivate(context);
}
