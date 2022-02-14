#pragma once
#include <DirectXMath.h>
#include "Shader.h"

class Texture;

//--------------------------------------------------------------------
//	PlainMesh�N���X
//--------------------------------------------------------------------
class PlainMesh
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer;
	int iNumVertices;	// ���_���@
	Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer;
	int iNumIndices;	// �C���f�b�N�X���@

	//�e�N�X�`�����p
	std::shared_ptr<Texture> texture;

	Microsoft::WRL::ComPtr<ID3D11Buffer> ConstantBuffer;
	struct ConstantBufferForPerMesh
	{
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4X4 matWVP;
	};
	// ���
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
//	���_�f�[�^�\���̒�`
//
//----------------------------------------------

struct VERTEX
{
	DirectX::XMFLOAT3 Pos;	//�ʒu
	DirectX::XMFLOAT4 Color;	//���_�F
	DirectX::XMFLOAT2 Tex;	//UV���W
	DirectX::XMFLOAT3 Normal;//�@��
};