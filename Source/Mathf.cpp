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
