#pragma once
#include <DirectXMath.h>

// �O���錾
class Model;

/// <summary>
/// �v�Z�֐��N���X
/// </summary>
class Mathf
{
public:
	// �x�N�g���Z�o
	static DirectX::XMVECTOR ReturnVectorSubtract(const DirectX::XMFLOAT3& subtract1, const DirectX::XMFLOAT3& subtract2);
	static DirectX::XMFLOAT3 ReturnFloatSubtract(const DirectX::XMFLOAT3& subtract1, const DirectX::XMFLOAT3& subtract2);
	// ���K�����ꂽ�x�N�g���Z�o
	static DirectX::XMFLOAT3 ReturnNormalizeFloatSubtract(const DirectX::XMFLOAT3& subtract1, const DirectX::XMFLOAT3& subtract2);

	// �x�N�g���̒����Z�o
	static float VectorLength(const DirectX::XMVECTOR& vector);
	static float VectorLength(const DirectX::XMFLOAT3& vector);

	// �p�x����O�������Z�o
	static DirectX::XMVECTOR ReturnVectorFront(const DirectX::XMFLOAT3& angle);
	static DirectX::XMFLOAT3 ReturnFloatFront(const DirectX::XMFLOAT3& angle);

	// ���ώZ�o
	static DirectX::XMVECTOR ReturnVectorDot(const DirectX::XMVECTOR& vector1, const DirectX::XMVECTOR& vector2);
	static float ReturnFloatDot(const DirectX::XMVECTOR& vector1, const DirectX::XMVECTOR& vector2);

	// ���`�⊮
	static float Lerp(float a, float b, float t);
	
	// �w��͈͂̃����_���l���Z�o
	static float RandomRange(float min, float max);
	
	// ���W�A���l�֕ϊ�
	static float ConvartToRadian(float dogree);

	// ���ʂ���̍ŋߒl�Z�o
	static void NegaCalculate(DirectX::XMFLOAT3& anser, DirectX::XMFLOAT3& normal, DirectX::XMFLOAT3& radius);
	
	// ���ʂ���̍ŉ��l�Z�o
	static void PosiCalculate(DirectX::XMFLOAT3& anser, DirectX::XMFLOAT3& normal, DirectX::XMFLOAT3& radius);

	// ���f���̃m�[�h���W�擾
	static void GetNodePosition(const char* node_name, DirectX::XMFLOAT3& node_position, Model* model);
};