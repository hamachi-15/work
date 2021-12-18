#include "CascadeShadowMapShader.h"
#include "Graphics.h"
#include "ModelResource.h"
#include "ActorManager.h"

//--------------------------------
// コンストラクタ
//--------------------------------
CascadeShadowMap::CascadeShadowMap(ID3D11Device* device)
{
	HRESULT hr = S_OK;
	Create(device, "Shader\\CascadeShadowMap_vs.cso", "Shader\\CascadeShadowMap_ps.cso", false);

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
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;

		hr = device->CreateSamplerState(&desc, sampler_shadow.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

}

//--------------------------------
// デストラクタ
//--------------------------------
CascadeShadowMap::~CascadeShadowMap()
{
}

//--------------------------------
// 描画開始処理
//--------------------------------
void CascadeShadowMap::Begin(ID3D11DeviceContext* context, RenderContext& render_context)
{
	Graphics& graphics = Graphics::Instance();
	Texture* shadow_texture1 = ActorManager::Instance().GetShadowTexture(0);
	Texture* shadow_texture2 = ActorManager::Instance().GetShadowTexture(1);
	Texture* shadow_texture3 = ActorManager::Instance().GetShadowTexture(2);

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
	//context->OMSetBlendState(graphics.GetBlendState((int)Graphics::BlendState::Alpha), nullptr, 0xFFFFFFFF);
	//ラスタライザ―設定
	context->RSSetState(graphics.GetRasterizerState((int)Graphics::RasterizerState::Cull_Back));
	//デプスステンシルステート設定
	context->OMSetDepthStencilState(graphics.GetDepthStencilState((int)Graphics::DepthStencilState::True), 1);

	shadow_texture1->Set(3); 
	shadow_texture2->Set(4);
	shadow_texture3->Set(5);

	context->PSSetSamplers(0, 1, sampler_state.GetAddressOf());
	context->PSSetSamplers(3, 1, sampler_shadow.GetAddressOf());

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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

//--------------------------------
// 描画処理
//--------------------------------
void CascadeShadowMap::Draw(ID3D11DeviceContext* context, const Model* model)
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
		u_int count = 0;
		u_int index = 0;
		for (const ModelResource::Subset& subset : mesh.subsets)
		{
			CBSubset cbsubset;
			cbsubset.materialColor = subset.material->color;
			context->UpdateSubresource(subset_constant_buffer.Get(), 0, 0, &cbsubset, 0, 0);
			context->PSSetShaderResources(0, 1, subset.material->shaderResourceView.GetAddressOf());
			count += subset.indexCount;
			index += subset.startIndex;
		}
		context->DrawIndexed(count, index, 0);
	}
}

//--------------------------------
// 描画終了処理
//--------------------------------
void CascadeShadowMap::End(ID3D11DeviceContext* context)
{
	InActivate(context);
}
