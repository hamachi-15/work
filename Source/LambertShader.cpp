#include "Misc.h"
#include "LambertShader.h"
#include "ModelResource.h"
LambertShader::LambertShader(ID3D11Device* device)
{
	HRESULT hr = S_OK;
	Create(device, "Shader\\LambertShader_vs.cso", "Shader\\LambertShader_ps.cso", false);

	// �萔�o�b�t�@
	{
		// �V�[���p�o�b�t�@
		D3D11_BUFFER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.ByteWidth = sizeof(CbScene);
		desc.StructureByteStride = 0;

		hr = device->CreateBuffer(&desc, 0, sceneConstantBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// ���b�V���p�o�b�t�@
		desc.ByteWidth = sizeof(CbMesh);

		hr = device->CreateBuffer(&desc, 0, meshConstantBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// �T�u�Z�b�g�p�o�b�t�@
		desc.ByteWidth = sizeof(CbSubset);

		hr = device->CreateBuffer(&desc, 0, subsetConstantBuffer.GetAddressOf());
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

		hr = device->CreateBlendState(&desc, blendState.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// �[�x�X�e���V���X�e�[�g
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.DepthEnable = true;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		hr = device->CreateDepthStencilState(&desc, depthStencilState.GetAddressOf());
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
		desc.CullMode = D3D11_CULL_BACK;
		desc.AntialiasedLineEnable = false;

		hr = device->CreateRasterizerState(&desc, rasterizerState.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// �T���v���X�e�[�g
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

		hr = device->CreateSamplerState(&desc, samplerState.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
}

// �`��J�n
void LambertShader::Begin(ID3D11DeviceContext* context, RenderContext& rc)
{
	Activate(context);
	
	ID3D11Buffer* constantBuffers[] =
	{
		sceneConstantBuffer.Get(),
		meshConstantBuffer.Get(),
		subsetConstantBuffer.Get()
	};
	context->VSSetConstantBuffers(0, ARRAYSIZE(constantBuffers), constantBuffers);
	context->PSSetConstantBuffers(0, ARRAYSIZE(constantBuffers), constantBuffers);

	const float blend_factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	context->OMSetBlendState(blendState.Get(), blend_factor, 0xFFFFFFFF);
	context->OMSetDepthStencilState(depthStencilState.Get(), 0);
	context->RSSetState(rasterizerState.Get());

	// �V�[���p�萔�o�b�t�@�X�V
	CbScene cbScene;
	DirectX::XMMATRIX V = DirectX::XMLoadFloat4x4(&rc.view);
	DirectX::XMMATRIX P = DirectX::XMLoadFloat4x4(&rc.projection);
	DirectX::XMStoreFloat4x4(&cbScene.viewProjection, V * P);

	cbScene.lightDirection = rc.light_direction;
	context->UpdateSubresource(sceneConstantBuffer.Get(), 0, 0, &cbScene, 0, 0);
}

// �`��
void LambertShader::Draw(ID3D11DeviceContext* context, const Model* model)
{
	const ModelResource* resource = model->GetResource();
	const std::vector<Model::Node>& nodes = model->GetNodes();

	for (const ModelResource::Mesh& mesh : resource->GetMeshes())
	{
		// ���b�V���p�萔�o�b�t�@�X�V
		CbMesh cbMesh;
		::memset(&cbMesh, 0, sizeof(cbMesh));
		if (mesh.nodeIndices.size() > 0)
		{
			for (size_t i = 0; i < mesh.nodeIndices.size(); ++i)
			{
				DirectX::XMMATRIX worldTransform = DirectX::XMLoadFloat4x4(&nodes.at(mesh.nodeIndices.at(i)).world_transform);
				DirectX::XMMATRIX offsetTransform = DirectX::XMLoadFloat4x4(&mesh.offsetTransforms.at(i));
				DirectX::XMMATRIX boneTransform = offsetTransform * worldTransform;
				DirectX::XMStoreFloat4x4(&cbMesh.boneTransforms[i], boneTransform);
			}
		}
		else
		{
			cbMesh.boneTransforms[0] = nodes.at(mesh.nodeIndex).world_transform;
		}
		context->UpdateSubresource(meshConstantBuffer.Get(), 0, 0, &cbMesh, 0, 0);

		UINT stride = sizeof(ModelResource::Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, mesh.vertexBuffer.GetAddressOf(), &stride, &offset);
		context->IASetIndexBuffer(mesh.indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		for (const ModelResource::Subset& subset : mesh.subsets)
		{
			CbSubset cbSubset;
			cbSubset.materialColor = subset.material->color;
			context->UpdateSubresource(subsetConstantBuffer.Get(), 0, 0, &cbSubset, 0, 0);
			context->PSSetShaderResources(0, 1, subset.material->shaderResourceView.GetAddressOf());
			context->PSSetSamplers(0, 1, samplerState.GetAddressOf());
			context->DrawIndexed(subset.indexCount, subset.startIndex, 0);
		}
	}

}

// �`��I��
void LambertShader::End(ID3D11DeviceContext* context)
{
	InActivate(context);
}
