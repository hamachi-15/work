#include <stdio.h>
#include <WICTextureLoader.h>
#include "Sprite.h"
#include "Misc.h"
#include "Graphics.h"
#include "SpriteShader.h"

// �R���X�g���N�^
Sprite::Sprite() : texture(nullptr)
{
	ID3D11Device* device = Graphics::Instance().GetDevice();

	HRESULT hr = S_OK;

	Vertex vertices[] = {
		{DirectX::XMFLOAT3(-0.5f, +0.5f, 0.0f), DirectX::XMFLOAT4(1, 1, 1, 1)},
		{DirectX::XMFLOAT3(+0.5f, +0.5f, 0.0f), DirectX::XMFLOAT4(1, 0, 0, 1)},
		{DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f), DirectX::XMFLOAT4(0, 1, 0, 1)},
		{DirectX::XMFLOAT3(+0.5f, -0.5f, 0.0f), DirectX::XMFLOAT4(0, 0, 1, 1)},
	};

	// ���_�o�b�t�@�̐���
	{
		D3D11_BUFFER_DESC desc;
		desc.ByteWidth = sizeof(vertices);
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// UNIT.03
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		// ���_�o�b�t�@�ɒ��_�f�[�^�����邽�߂̐ݒ�
		D3D11_SUBRESOURCE_DATA subresource_data = {};
		subresource_data.pSysMem = vertices;	// �����Ɋi�[���������_�f�[�^�̃A�h���X��n�����Ƃ�CreateBuffer()���Ƀf�[�^�����邱�Ƃ��ł���B
		subresource_data.SysMemPitch = 0; //Not use for vertex buffers.
		subresource_data.SysMemSlicePitch = 0; //Not use for vertex buffers.
		// ���_�o�b�t�@�I�u�W�F�N�g�̐���
		hr = device->CreateBuffer(&desc, &subresource_data, &vertex_buffer);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// �u�����h�X�e�[�g
	{
		D3D11_BLEND_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.AlphaToCoverageEnable = false;
		desc.IndependentBlendEnable = false;
		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		HRESULT hr = device->CreateBlendState(&desc, blend_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		desc.IndependentBlendEnable = false;
		desc.AlphaToCoverageEnable = false;
		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		hr = device->CreateBlendState(&desc, add_blend_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	}

	// �[�x�X�e���V���X�e�[�g
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.DepthEnable = true;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_ALWAYS;

		HRESULT hr = device->CreateDepthStencilState(&desc, depth_stencil_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// ���X�^���C�U�[�X�e�[�g
	{
		D3D11_RASTERIZER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.FrontCounterClockwise = false;
		desc.DepthBias = 0;
		desc.DepthBiasClamp = 0;
		desc.SlopeScaledDepthBias = 0;
		desc.DepthClipEnable = true;
		desc.ScissorEnable = false;
		desc.MultisampleEnable = true;
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_NONE;
		desc.AntialiasedLineEnable = false;

		HRESULT hr = device->CreateRasterizerState(&desc, rasterizer_state.GetAddressOf());

		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
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

		HRESULT hr = device->CreateSamplerState(&desc, sampler_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
	{
		const int width = 8;
		const int height = 8;
		UINT pixels[width * height];
		::memset(pixels, 0xFF, sizeof(pixels));

		D3D11_TEXTURE2D_DESC desc = { 0 };
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA data;
		::memset(&data, 0, sizeof(data));

		texture_width = desc.Width;
		texture_height = desc.Height;
	}
}

Sprite::Sprite(const char* filename)
{
	ID3D11Device* device = Graphics::Instance().GetDevice();

	HRESULT hr = S_OK;

	Vertex vertices[] = {
		{DirectX::XMFLOAT3(-0.5f, +0.5f, 0.0f), DirectX::XMFLOAT4(1, 1, 1, 1)},
		{DirectX::XMFLOAT3(+0.5f, +0.5f, 0.0f), DirectX::XMFLOAT4(1, 0, 0, 1)},
		{DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f), DirectX::XMFLOAT4(0, 1, 0, 1)},
		{DirectX::XMFLOAT3(+0.5f, -0.5f, 0.0f), DirectX::XMFLOAT4(0, 0, 1, 1)},
	};

	// ���_�o�b�t�@�̐���
	{
		D3D11_BUFFER_DESC desc;
		desc.ByteWidth = sizeof(vertices);
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// UNIT.03
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		// ���_�o�b�t�@�ɒ��_�f�[�^�����邽�߂̐ݒ�
		D3D11_SUBRESOURCE_DATA subresource_data = {};
		subresource_data.pSysMem = vertices;	// �����Ɋi�[���������_�f�[�^�̃A�h���X��n�����Ƃ�CreateBuffer()���Ƀf�[�^�����邱�Ƃ��ł���B
		subresource_data.SysMemPitch = 0; //Not use for vertex buffers.
		subresource_data.SysMemSlicePitch = 0; //Not use for vertex buffers.
		// ���_�o�b�t�@�I�u�W�F�N�g�̐���
		hr = device->CreateBuffer(&desc, &subresource_data, &vertex_buffer);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// �u�����h�X�e�[�g
	{
		D3D11_BLEND_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.AlphaToCoverageEnable = false;
		desc.IndependentBlendEnable = false;
		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		HRESULT hr = device->CreateBlendState(&desc, blend_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// �[�x�X�e���V���X�e�[�g
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.DepthEnable = true;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_ALWAYS;

		HRESULT hr = device->CreateDepthStencilState(&desc, depth_stencil_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// ���X�^���C�U�[�X�e�[�g
	{
		D3D11_RASTERIZER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.FrontCounterClockwise = false;
		desc.DepthBias = 0;
		desc.DepthBiasClamp = 0;
		desc.SlopeScaledDepthBias = 0;
		desc.DepthClipEnable = true;
		desc.ScissorEnable = false;
		desc.MultisampleEnable = true;
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_NONE;
		desc.AntialiasedLineEnable = false;

		HRESULT hr = device->CreateRasterizerState(&desc, rasterizer_state.GetAddressOf());

		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
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

		HRESULT hr = device->CreateSamplerState(&desc, sampler_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// �e�N�X�`���̐���
	if (filename != nullptr)
	{
		texture = std::make_unique<Texture>();
		texture->Load(filename);

		texture_width = texture->GetWidth();
		texture_height = texture->GetHeight();
	}
	else
	{
		const int width = 8;
		const int height = 8;
		UINT pixels[width * height];
		::memset(pixels, 0xFF, sizeof(pixels));

		D3D11_TEXTURE2D_DESC desc = { 0 };
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA data;
		::memset(&data, 0, sizeof(data));

		texture_width = desc.Width;
		texture_height = desc.Height;
	}
}

Sprite::Sprite(Texture* texture) : texture(texture)
{
	ID3D11Device* device = Graphics::Instance().GetDevice();

	HRESULT hr = S_OK;

	Vertex vertices[] = {
		{DirectX::XMFLOAT3(-0.5f, +0.5f, 0.0f), DirectX::XMFLOAT4(1, 1, 1, 1)},
		{DirectX::XMFLOAT3(+0.5f, +0.5f, 0.0f), DirectX::XMFLOAT4(1, 0, 0, 1)},
		{DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f), DirectX::XMFLOAT4(0, 1, 0, 1)},
		{DirectX::XMFLOAT3(+0.5f, -0.5f, 0.0f), DirectX::XMFLOAT4(0, 0, 1, 1)},
	};

	// ���_�o�b�t�@�̐���
	{
		D3D11_BUFFER_DESC desc;
		desc.ByteWidth = sizeof(vertices);
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// UNIT.03
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		// ���_�o�b�t�@�ɒ��_�f�[�^�����邽�߂̐ݒ�
		D3D11_SUBRESOURCE_DATA subresource_data = {};
		subresource_data.pSysMem = vertices;	// �����Ɋi�[���������_�f�[�^�̃A�h���X��n�����Ƃ�CreateBuffer()���Ƀf�[�^�����邱�Ƃ��ł���B
		subresource_data.SysMemPitch = 0; //Not use for vertex buffers.
		subresource_data.SysMemSlicePitch = 0; //Not use for vertex buffers.
		// ���_�o�b�t�@�I�u�W�F�N�g�̐���
		hr = device->CreateBuffer(&desc, &subresource_data, &vertex_buffer);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// �u�����h�X�e�[�g
	{
		D3D11_BLEND_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.AlphaToCoverageEnable = false;
		desc.IndependentBlendEnable = false;
		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		HRESULT hr = device->CreateBlendState(&desc, blend_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// �[�x�X�e���V���X�e�[�g
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.DepthEnable = FALSE;
		//desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		//desc.DepthFunc = D3D11_COMPARISON_ALWAYS;

		HRESULT hr = device->CreateDepthStencilState(&desc, depth_stencil_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// ���X�^���C�U�[�X�e�[�g
	{
		D3D11_RASTERIZER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.FrontCounterClockwise = false;
		desc.DepthBias = 0;
		desc.DepthBiasClamp = 0;
		desc.SlopeScaledDepthBias = 0;
		desc.DepthClipEnable = true;
		desc.ScissorEnable = false;
		desc.MultisampleEnable = true;
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_NONE;
		desc.AntialiasedLineEnable = false;

		HRESULT hr = device->CreateRasterizerState(&desc, rasterizer_state.GetAddressOf());

		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
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

		HRESULT hr = device->CreateSamplerState(&desc, sampler_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

}

// �`�揈��
void Sprite::Render(
	ID3D11DeviceContext* context,
	float dx, float dy,
	float dw, float dh,
	float sx, float sy,
	float sw, float sh,
	float angle,
	float r, float g, float b, float a) const
{
	{
		D3D11_VIEWPORT viewport;
		UINT num_viewport = 1;
		context->RSGetViewports(&num_viewport, &viewport);
		float screen_width = viewport.Width;
		float screen_height = viewport.Height;

		//
		DirectX::XMFLOAT2 positions[] = {
			DirectX::XMFLOAT2(dx,		dy),
			DirectX::XMFLOAT2(dx + dw,	dy),
			DirectX::XMFLOAT2(dx,		dy + dh),
			DirectX::XMFLOAT2(dx + dw,	dy + dh)
		};

		DirectX::XMFLOAT2 texcoords[] = {
			DirectX::XMFLOAT2(sx,		sy),
			DirectX::XMFLOAT2(sx + sw,	sy),
			DirectX::XMFLOAT2(sx,		sy + sh),
			DirectX::XMFLOAT2(sx + sw,	sy + sh)
		};

		float mx = dx + dw * 0.5f;
		float my = dy + dh * 0.5f;
		for (auto& p : positions)
		{
			p.x -= mx;
			p.y -= my;
		}

		// ���_����]
		const float PI = 3.141592653589793f;
		float theta = angle * (PI / 180.0f);
		float c = cosf(theta);
		float s = sinf(theta);
		for (auto& p : positions)
		{
			DirectX::XMFLOAT2 r = p;
			p.x = c * r.x + -s * r.y;
			p.y = s * r.x + c * r.y;
		}

		// ��]�ׂ̈Ɉړ����������_�����̈ʒu��
		for (auto& p : positions)
		{
			p.x += mx;
			p.y += my;
		}

		// NDC���W�n�֕ϊ�
		for (auto& p : positions)
		{
			p.x = 2.0f * p.x / screen_width - 1.0f;
			p.y = 1.0f - 2.0f * p.y / screen_height;
		}

		// ���_�o�b�t�@�̓��e�̕ҏW���J�n����B
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		HRESULT hr = context->Map(vertex_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// pData��ҏW���邱�ƂŒ��_�f�[�^�̓��e�����������邱�Ƃ��ł���B
		Vertex* v = static_cast<Vertex*>(mappedBuffer.pData);
		for (int i = 0; i < 4; ++i)
		{
			v[i].position.x = positions[i].x;
			v[i].position.y = positions[i].y;
			v[i].position.z = 0.0f;

			v[i].color.x = r;
			v[i].color.y = g;
			v[i].color.z = b;
			v[i].color.w = a;

			v[i].texcoord.x = texcoords[i].x / texture_width;
			v[i].texcoord.y = texcoords[i].y / texture_height;
		}

		// ���_�o�b�t�@�̓��e�̕ҏW���I������B
		context->Unmap(vertex_buffer.Get(), 0);
	}

	// �p�C�v���C���ݒ�
	{
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		context->RSSetState(rasterizer_state.Get());
		const float blendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		context->OMSetBlendState(blend_state.Get(), blendFactor, 0xFFFFFFFF);
		context->OMSetDepthStencilState(depth_stencil_state.Get(), 0);

		// �V�F�[�_�[���\�[�X�r���[�̐ݒ�
		texture->Set(0);

		//context->OMSetDepthStencilState(depth_stencil_state.Get(), 1); 
		context->PSSetSamplers(0, 1, sampler_state.GetAddressOf());

		// �`��
		context->Draw(4, 0);
	}
}

void Sprite::Render(ID3D11DeviceContext* context, float dx, float dy, float dw, float dh, float sx, float sy, float sw, float sh, float angle, Pivot pivot, float r, float g, float b, float a) const
{
	D3D11_VIEWPORT viewport;
	UINT num_viewport = 1;
	context->RSGetViewports(&num_viewport, &viewport);
	float screen_width = viewport.Width;
	float screen_height = viewport.Height;
	DirectX::XMFLOAT2 positions[] = {
		DirectX::XMFLOAT2(dx,		dy),
		DirectX::XMFLOAT2(dx + dw,	dy),
		DirectX::XMFLOAT2(dx,		dy + dh),
		DirectX::XMFLOAT2(dx + dw,	dy + dh)
	};

	DirectX::XMFLOAT2 texcoords[] = {
		DirectX::XMFLOAT2(sx,		sy),
		DirectX::XMFLOAT2(sx + sw,	sy),
		DirectX::XMFLOAT2(sx,		sy + sh),
		DirectX::XMFLOAT2(sx + sw,	sy + sh)
	};
	// �X�v���C�g�̒��S�ŉ�]�����邽�߂ɂS���_�̒��S�ʒu��
	// ���_(0, 0)�ɂȂ�悤�Ɉ�U���_���ړ�������B
	float mx = dx + dw * 0.5f;
	float my = dy + dh * 0.5f;
	switch (pivot)
	{
	case Pivot::LeftTop:		mx -= dw * 0.5f;	my -= dh * 0.5f;	break;
	case Pivot::LeftCenter:		mx -= dw * 0.5f;	break;
	case Pivot::LeftBottom:		mx -= dw * 0.5f;	my += dh * 0.5f;	break;
	case Pivot::CenterTop:		my -= dh * 0.5f;	break;
	case Pivot::CenterCenter:	break;
	case Pivot::CenterBottom:	my += dh * 0.5f;	break;
	case Pivot::RightTop:		mx -= dw * 0.5f;	my -= dh * 0.5f;	break;
	case Pivot::RightCenter:	mx += dw * 0.5f;	break;
	case Pivot::RightBottom:	mx += dw * 0.5f;	my += dh * 0.5f;	break;
	}
	for (auto& p : positions)
	{
		p.x -= mx;
		p.y -= my;
	}
	// ���_����]
	const float PI = 3.141592653589793f;
	float theta = angle * (PI / 180.0f);
	float c = cosf(theta);
	float s = sinf(theta);
	for (auto& p : positions)
	{
		DirectX::XMFLOAT2 r = p;
		p.x = c * r.x + -s * r.y;
		p.y = s * r.x + c * r.y;
	}

	// ��]�ׂ̈Ɉړ����������_�����̈ʒu��
	for (auto& p : positions)
	{
		p.x += mx;
		p.y += my;
	}

	// NDC���W�n�֕ϊ�
	for (auto& p : positions)
	{
		p.x = 2.0f * p.x / screen_width - 1.0f;
		p.y = 1.0f - 2.0f * p.y / screen_height;
	}
	// ���_�o�b�t�@�̓��e�̕ҏW���J�n����B
	D3D11_MAPPED_SUBRESOURCE mappedBuffer;
	HRESULT hr = context->Map(vertex_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// pData��ҏW���邱�ƂŒ��_�f�[�^�̓��e�����������邱�Ƃ��ł���B
	Vertex* v = static_cast<Vertex*>(mappedBuffer.pData);
	for (int i = 0; i < 4; ++i)
	{
		v[i].position.x = positions[i].x;
		v[i].position.y = positions[i].y;
		v[i].position.z = 0.0f;

		v[i].color.x = r;
		v[i].color.y = g;
		v[i].color.z = b;
		v[i].color.w = a;

		v[i].texcoord.x = texcoords[i].x / texture->GetWidth();
		v[i].texcoord.y = texcoords[i].y / texture->GetHeight();
	}

	// ���_�o�b�t�@�̓��e�̕ҏW���I������B
	context->Unmap(vertex_buffer.Get(), 0);

	// �p�C�v���C���ݒ�
	{
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		context->RSSetState(rasterizer_state.Get());

		// �V�F�[�_�[���\�[�X�r���[�̐ݒ�
		texture->Set(0);
		context->OMSetDepthStencilState(depth_stencil_state.Get(), 0);

		//context->OMSetDepthStencilState(depth_stencil_state.Get(), 1); 
		context->PSSetSamplers(0, 1, sampler_state.GetAddressOf());

		// �`��
		context->Draw(4, 0);
	}

}

void Sprite::Render(ID3D11DeviceContext* context,
	Texture* texture,
	float dx, float dy,
	float dw, float dh,
	float sx, float sy,
	float sw, float sh,
	float angle,
	float r, float g, float b, float a) const
{
	{
#if 0
		Graphics& graphics = Graphics::Instance();
		float screen_width = graphics.GetScreenWidth();
		float screen_height = graphics.GetScreenHeight();

#endif
		D3D11_VIEWPORT viewport;
		UINT num_viewport = 1;
		context->RSGetViewports(&num_viewport, &viewport);
		float screen_width = viewport.Width;
		float screen_height = viewport.Height;
		//
		DirectX::XMFLOAT2 positions[] = {
			DirectX::XMFLOAT2(dx,		dy),
			DirectX::XMFLOAT2(dx + dw,	dy),
			DirectX::XMFLOAT2(dx,		dy + dh),
			DirectX::XMFLOAT2(dx + dw,	dy + dh)
		};

		DirectX::XMFLOAT2 texcoords[] = {
			DirectX::XMFLOAT2(sx,		sy),
			DirectX::XMFLOAT2(sx + sw,	sy),
			DirectX::XMFLOAT2(sx,		sy + sh),
			DirectX::XMFLOAT2(sx + sw,	sy + sh)
		};

		// �X�v���C�g�̒��S�ŉ�]�����邽�߂ɂS���_�̒��S�ʒu��
		// ���_(0, 0)�ɂȂ�悤�Ɉ�U���_���ړ�������B
		float mx = dx + dw * 0.5f;
		float my = dy + dh * 0.5f;
		for (auto& p : positions)
		{
			p.x -= mx;
			p.y -= my;
		}

		// ���_����]
		const float PI = 3.141592653589793f;
		float theta = angle * (PI / 180.0f);
		float c = cosf(theta);
		float s = sinf(theta);
		for (auto& p : positions)
		{
			DirectX::XMFLOAT2 r = p;
			p.x = c * r.x + -s * r.y;
			p.y = s * r.x + c * r.y;
		}

		// ��]�ׂ̈Ɉړ����������_�����̈ʒu��
		for (auto& p : positions)
		{
			p.x += mx;
			p.y += my;
		}

		// NDC���W�n�֕ϊ�
		for (auto& p : positions)
		{
			p.x = 2.0f * p.x / screen_width - 1.0f;
			p.y = 1.0f - 2.0f * p.y / screen_height;
		}

		// ���_�o�b�t�@�̓��e�̕ҏW���J�n����B
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		HRESULT hr = context->Map(vertex_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// pData��ҏW���邱�ƂŒ��_�f�[�^�̓��e�����������邱�Ƃ��ł���B
		Vertex* v = static_cast<Vertex*>(mappedBuffer.pData);
		for (int i = 0; i < 4; ++i)
		{
			v[i].position.x = positions[i].x;
			v[i].position.y = positions[i].y;
			v[i].position.z = 0.0f;

			v[i].color.x = r;
			v[i].color.y = g;
			v[i].color.z = b;
			v[i].color.w = a;

			v[i].texcoord.x = texcoords[i].x / texture->GetWidth();
			v[i].texcoord.y = texcoords[i].y / texture->GetHeight();
		}

		// ���_�o�b�t�@�̓��e�̕ҏW���I������B
		context->Unmap(vertex_buffer.Get(), 0);
	}

	// �p�C�v���C���ݒ�
	{
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		context->RSSetState(rasterizer_state.Get());

		const float blend_factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		context->OMSetBlendState(blend_state.Get(), blend_factor, 0xFFFFFFFF);
		texture->Set(0);
		//context->OMSetDepthStencilState(depth_stencil_state.Get(), 1);
		context->PSSetSamplers(0, 1, sampler_state.GetAddressOf());

		// �`��
		context->Draw(4, 0);
	}
}

void Sprite::AddRender(ID3D11DeviceContext* context, Texture* texture, float dx, float dy, float dw, float dh, float sx, float sy, float sw, float sh, float angle, float r, float g, float b, float a) const
{
	{
#if 0
		Graphics& graphics = Graphics::Instance();
		float screen_width = graphics.GetScreenWidth();
		float screen_height = graphics.GetScreenHeight();

#endif
		D3D11_VIEWPORT viewport;
		UINT num_viewport = 1;
		context->RSGetViewports(&num_viewport, &viewport);
		float screen_width = viewport.Width;
		float screen_height = viewport.Height;
		//
		DirectX::XMFLOAT2 positions[] = {
			DirectX::XMFLOAT2(dx,		dy),
			DirectX::XMFLOAT2(dx + dw,	dy),
			DirectX::XMFLOAT2(dx,		dy + dh),
			DirectX::XMFLOAT2(dx + dw,	dy + dh)
		};

		DirectX::XMFLOAT2 texcoords[] = {
			DirectX::XMFLOAT2(sx,		sy),
			DirectX::XMFLOAT2(sx + sw,	sy),
			DirectX::XMFLOAT2(sx,		sy + sh),
			DirectX::XMFLOAT2(sx + sw,	sy + sh)
		};

		// �X�v���C�g�̒��S�ŉ�]�����邽�߂ɂS���_�̒��S�ʒu��
		// ���_(0, 0)�ɂȂ�悤�Ɉ�U���_���ړ�������B
		float mx = dx + dw * 0.5f;
		float my = dy + dh * 0.5f;
		for (auto& p : positions)
		{
			p.x -= mx;
			p.y -= my;
		}

		// ���_����]
		const float PI = 3.141592653589793f;
		float theta = angle * (PI / 180.0f);
		float c = cosf(theta);
		float s = sinf(theta);
		for (auto& p : positions)
		{
			DirectX::XMFLOAT2 r = p;
			p.x = c * r.x + -s * r.y;
			p.y = s * r.x + c * r.y;
		}

		// ��]�ׂ̈Ɉړ����������_�����̈ʒu��
		for (auto& p : positions)
		{
			p.x += mx;
			p.y += my;
		}

		// NDC���W�n�֕ϊ�
		for (auto& p : positions)
		{
			p.x = 2.0f * p.x / screen_width - 1.0f;
			p.y = 1.0f - 2.0f * p.y / screen_height;
		}

		// ���_�o�b�t�@�̓��e�̕ҏW���J�n����B
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		HRESULT hr = context->Map(vertex_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// pData��ҏW���邱�ƂŒ��_�f�[�^�̓��e�����������邱�Ƃ��ł���B
		Vertex* v = static_cast<Vertex*>(mappedBuffer.pData);
		for (int i = 0; i < 4; ++i)
		{
			v[i].position.x = positions[i].x;
			v[i].position.y = positions[i].y;
			v[i].position.z = 0.0f;

			v[i].color.x = r;
			v[i].color.y = g;
			v[i].color.z = b;
			v[i].color.w = a;

			v[i].texcoord.x = texcoords[i].x / texture->GetWidth();
			v[i].texcoord.y = texcoords[i].y / texture->GetHeight();
		}

		// ���_�o�b�t�@�̓��e�̕ҏW���I������B
		context->Unmap(vertex_buffer.Get(), 0);
	}

	// �p�C�v���C���ݒ�
	{
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		context->RSSetState(rasterizer_state.Get());

		const float blend_factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		context->OMSetBlendState(add_blend_state.Get(), blend_factor, 0xFFFFFFFF);
		texture->Set(0);
		//context->OMSetDepthStencilState(depth_stencil_state.Get(), 1);
		context->PSSetSamplers(0, 1, sampler_state.GetAddressOf());

		// �`��
		context->Draw(4, 0);
	}
}




