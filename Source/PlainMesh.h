#pragma once
#include <DirectXMath.h>
#include "Shader.h"

class Texture;

//--------------------------------------------------------------------
//	PlainMeshクラス
//--------------------------------------------------------------------
class PlainMesh
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer;
	int iNumVertices;	// 頂点数　
	Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer;
	int iNumIndices;	// インデックス数　

	//テクスチャ利用
	std::shared_ptr<Texture> texture;

	Microsoft::WRL::ComPtr<ID3D11Buffer> ConstantBuffer;
	struct ConstantBufferForPerMesh
	{
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4X4 matWVP;
	};
	// 情報
	DirectX::XMFLOAT3 pos = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT3 angle = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT4X4 WorldMatrix;

public:
	PlainMesh(int row, int col);
	PlainMesh(const char* filename, int row, int col);
	PlainMesh(std::shared_ptr<Texture> texture, int row, int col);

	~PlainMesh();
	void Update();
	void Render(Shader* shader, DirectX::XMMATRIX* view, DirectX::XMMATRIX* projection, D3D_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	void Render(Shader* shader, Texture* texture, RenderContext* render_context, D3D_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	void SetPos(const DirectX::XMFLOAT3& pos) { this->pos = pos; }
	void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }
	void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }



};
//----------------------------------------------
//
//	頂点データ構造体定義
//
//----------------------------------------------

struct VERTEX
{
	DirectX::XMFLOAT3 Pos;	//位置
	DirectX::XMFLOAT4 Color;	//頂点色
	DirectX::XMFLOAT2 Tex;	//UV座標
	DirectX::XMFLOAT3 Normal;//法線
};