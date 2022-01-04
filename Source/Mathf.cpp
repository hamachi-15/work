#include <stdlib.h>
#include "Mathf.h"
#include <DirectXMath.h>

//----------------------------------
// 線形補完
//----------------------------------
float Mathf::Lerp(float a, float b, float t)
{
	return a * (1.0f - t) + (b * t);
}

//----------------------------------
// 指定範囲のランダム値を算出
//----------------------------------
float Mathf::RandomRange(float min, float max)
{
	float random = min + static_cast<float>(rand() * (max - min) / (1.0f + RAND_MAX));
	return random;
}

//----------------------------------
// ラジアン値へ変換
//----------------------------------
float Mathf::ConvartToRadian(float degrees)
{
	return DirectX::XMConvertToRadians(degrees);
}

//----------------------------------
// 平面からの最近値算出
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
// 平面からの最遠値算出
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
