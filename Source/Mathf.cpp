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
