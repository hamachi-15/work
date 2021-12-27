#include "Graphics.h"

#include "2DPrimitive.h"

//------------------------------
// �R���X�g���N�^
//------------------------------
Primitive::Primitive(ID3D11Device* device)
{
	Graphics& graphics = Graphics::Instance();
	HRESULT hr = S_OK;

	Create(device, "Shader/2DPrimitive_vs.cso", "Shader/2DPrimitive_ps.cso", true);

	// �萔�o�b�t�@�ݒ�
	{
		D3D11_BUFFER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.ByteWidth = sizeof(ConstantBuffer);
		desc.StructureByteStride = 0;
		hr = device->CreateBuffer(&desc, 0, constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
}

//------------------------------
// �`��J�n����
//------------------------------
void Primitive::Begin(ID3D11DeviceContext* context, PrimitiveContext& cprimitive_context)
{
	Graphics& graphics = Graphics::Instance();
	Activate(context);

	//	�u�����h�X�e�[�g�ݒ�
	context->OMSetBlendState(graphics.GetBlendState(static_cast<int>(Graphics::BlendState::Alpha)), nullptr, 0xFFFFFFFF);

	//���X�^���C�U�\�ݒ�
	context->RSSetState(graphics.GetRasterizerState(static_cast<int>(Graphics::RasterizerState::Cull_Back)));

	//�f�v�X�X�e���V���X�e�[�g�ݒ�
	context->OMSetDepthStencilState(graphics.GetDepthStencilState(static_cast<int>(Graphics::DepthStencilState::True)), 1);
	
	// �R���X�^���g�o�b�t�@�X�V
	ConstantBuffer cbscene;
	cbscene.timer = cprimitive_context.timer;
	cbscene.number = cprimitive_context.number;

	context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
	context->PSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
	context->UpdateSubresource(constant_buffer.Get(), 0, 0, &cbscene, 0, 0);
}

//------------------------------
// �`��I������
//------------------------------
void Primitive::End(ID3D11DeviceContext* context)
{
	InActivate(context);
}
