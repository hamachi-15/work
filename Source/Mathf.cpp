#include <stdlib.h>
#include "Mathf.h"
#include <DirectXMath.h>

//----------------------------------
// ���`�⊮
//----------------------------------
float Mathf::Lerp(float a, float b, float t)
{
	return a * (1.0f - t) + (b * t);
}

//----------------------------------
// �w��͈͂̃����_���l���Z�o
//----------------------------------
float Mathf::RandomRange(float min, float max)
{
	float random = min + static_cast<float>(rand() * (max - min) / (1.0f + RAND_MAX));
	return random;
}

//----------------------------------
// ���W�A���l�֕ϊ�
//----------------------------------
float Mathf::ConvartToRadian(float degrees)
{
	return DirectX::XMConvertToRadians(degrees);
}

//----------------------------------
// ���ʂ���̍ŋߒl�Z�o
//----------------------------------
void Mathf::NegaCalculate(DirectX::XMFLOAT3& anser, DirectX::XMFLOAT3& normal, DirectX::XMFLOAT3& radius)
{
    if (normal.x > 0)
    {
        anser.x += radius.x;
    }
    else
    {
        anser.x -= radius.x;
    }
    if (normal.y > 0)
    {
        anser.y += radius.y;
    }
    else
    {
        anser.y -= radius.y;
    }
    if (normal.z > 0)
    {
        anser.z += radius.z;
    }
    else
    {
        anser.z -= radius.z;
    }
}

//----------------------------------
// ���ʂ���̍ŉ��l�Z�o
//----------------------------------
void Mathf::PosiCalculate(DirectX::XMFLOAT3& anser, DirectX::XMFLOAT3& normal, DirectX::XMFLOAT3& radius)
{
    if (normal.x < 0)
    {
        anser.x += radius.x;
    }
    else
    {
        anser.x -= radius.x;
    }
    if (normal.y < 0)
    {
        anser.y += radius.y;
    }
    else
    {
        anser.y -= radius.y;
    }
    if (normal.z < 0)
    {
        anser.z += radius.z;
    }
    else
    {
        anser.z -= radius.z;
    }
}
