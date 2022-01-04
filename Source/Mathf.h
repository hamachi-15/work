#pragma once
#include <DirectXMath.h>

/// <summary>
/// �v�Z�֐��N���X
/// </summary>
class Mathf
{
public:
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
};