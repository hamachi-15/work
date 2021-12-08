#include "PhongVarianceShadowMap.h"
#include "ActorManager.h"
#include "ModelResource.h"
PhongVarianceShadowMap::PhongVarianceShadowMap(ID3D11Device* device)
{
	HRESULT hr = S_OK;
	Create(device, "Shader\\PhongVarianceShadowMap_vs.cso", "Shader\\PhongVarianceShadowMap_ps.cso", false);

	// 定数バッファ
	{
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

		desc.ByteWidth = sizeof(CBMesh);
		hr = device->CreateBuffer(&desc, 0, mesh_constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		desc.ByteWidth = sizeof(CBSubset);
		hr = device->CreateBuffer(&desc, 0, subset_constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	}
	// ブレンドステート
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

		hr = device->CreateBlendState(&desc, blend_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
	// ラステライザー設定
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

		hr = device->CreateRasterizerState(&desc, rasterizer_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
	// 深度ステンシルステート
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.DepthEnable = true;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		hr = device->CreateDepthStencilState(&desc, depth_stencil_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
	// サンプラーステート
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
	// シャドウ用サンプラーステート
	{
		D3D11_SAMPLER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.MipLODBias = 0.0f;
		desc.MaxAnisotropy = 1;
		desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		desc.MinLOD = -FLT_MAX;
		desc.MaxLOD = FLT_MAX;
		desc.BorderColor[0] = FLT_MAX;
		desc.BorderColor[1] = FLT_MAX;
		desc.BorderColor[2] = FLT_MAX;
		desc.BorderColor[3] = FLT_MAX;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

		hr = device->CreateSamplerState(&desc, sampler_shadow.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
}

void PhongVarianceShadowMap::Begin(ID3D11DeviceContext* context, RenderContext& render_context)
{
	Texture* shadow_texture1 = ActorManager::Instance().GetShadowBulrTexture(0);
	Texture* shadow_texture2 = ActorManager::Instance().GetShadowBulrTexture(1);
	Texture* shadow_texture3 = ActorManager::Instance().GetShadowBulrTexture(2);

	Activate(context);

	ID3D11Buffer* constant_buffers[] =
	{
		scene_constant_buffer.Get(),
		mesh_constant_buffer.Get(),
		subset_constant_buffer.Get()
	};
	context->VSSetConstantBuffers(0, ARRAYSIZE(constant_buffers), constant_buffers);
	context->PSSetConstantBuffers(0, ARRAYSIZE(constant_buffers), constant_buffers);

	//ブレンドステート設定
	context->OMSetBlendState(blend_state.Get(), nullptr, 0xFFFFFFFF);
	//ラスタライザ―設定
	context->RSSetState(rasterizer_state.Get());
	//デプスステンシルステート設定
	context->OMSetDepthStencilState(depth_stencil_state.Get(), 1);

	shadow_texture1->Set(3);
	shadow_texture2->Set(4);
	shadow_texture3->Set(5);

	context->PSSetSamplers(3, 1, sampler_shadow.GetAddressOf());

	// シーン用定数バッファ更新
	CBScene cbscene;
	DirectX::XMMATRIX V = DirectX::XMLoadFloat4x4(&render_context.view);
	DirectX::XMMATRIX P = DirectX::XMLoadFloat4x4(&render_context.projection);
	DirectX::XMStoreFloat4x4(&cbscene.view_projection, V * P);
	V = DirectX::XMMatrixInverse(nullptr, V);
	DirectX::XMFLOAT4 v;
	DirectX::XMStoreFloat4(&v, V.r[3]);
	cbscene.eye_position = v;

	cbscene.light_view_projection[0] = render_context.light_view_projection[0];
	cbscene.light_view_projection[1] = render_context.light_view_projection[1];
	cbscene.light_view_projection[2] = render_context.light_view_projection[2];

	cbscene.light_direction = render_context.light_direction;
	cbscene.shadow_parameter = render_context.ShadowParameter;
	context->UpdateSubresource(scene_constant_buffer.Get(), 0, 0, &cbscene, 0, 0);
}

void PhongVarianceShadowMap::Draw(ID3D11DeviceContext* context, const Model* model)
{
	const ModelResource* resource = model->GetResource();
	const std::vector<Model::Node>& nodes = model->GetNodes();

	for (const ModelResource::Mesh& mesh : resource->GetMeshes())
	{
		// メッシュ用定数バッファ更新
		CBMesh cbmesh;
		::memset(&cbmesh, 0, sizeof(cbmesh));
		if (mesh.nodeIndices.size() > 0)
		{
			for (size_t i = 0; i < mesh.nodeIndices.size(); ++i)
			{
				DirectX::XMMATRIX worldTransform = DirectX::XMLoadFloat4x4(&nodes.at(mesh.nodeIndices.at(i)).world_transform);
				DirectX::XMMATRIX offsetTransform = DirectX::XMLoadFloat4x4(&mesh.offsetTransforms.at(i));
				DirectX::XMMATRIX boneTransform = offsetTransform * worldTransform;
				DirectX::XMStoreFloat4x4(&cbmesh.boneTransforms[i], boneTransform);
			}
		}
		else
		{
			cbmesh.boneTransforms[0] = nodes.at(mesh.nodeIndex).world_transform;
		}
		context->UpdateSubresource(mesh_constant_buffer.Get(), 0, 0, &cbmesh, 0, 0);

		UINT stride = sizeof(ModelResource::Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, mesh.vertexBuffer.GetAddressOf(), &stride, &offset);
		context->IASetIndexBuffer(mesh.indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		for (const ModelResource::Subset& subset : mesh.subsets)
		{
			CBSubset cbsubset;
			cbsubset.materialColor = subset.material->color;
			context->UpdateSubresource(subset_constant_buffer.Get(), 0, 0, &cbsubset, 0, 0);
			context->PSSetShaderResources(0, 1, subset.material->shaderResourceView.GetAddressOf());
			context->PSSetSamplers(0, 1, sampler_state.GetAddressOf());
			context->DrawIndexed(subset.indexCount, subset.startIndex, 0);
		}
	}
}

void PhongVarianceShadowMap::End(ID3D11DeviceContext* context)
{
	InActivate(context);
}
