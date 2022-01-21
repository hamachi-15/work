#include <stdlib.h>
#include <DirectXMath.h>
#include "Mathf.h"
#include "Model.h"
//----------------------------------
// �x�N�g���Z�o,XMVECTOR�^��Ԃ�
//----------------------------------
DirectX::XMVECTOR Mathf::ReturnVectorSubtract(const DirectX::XMFLOAT3& subtract1, const DirectX::XMFLOAT3& subtract2)
{
    return DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&subtract1), DirectX::XMLoadFloat3(&subtract2));
}

//----------------------------------
// �x�N�g���Z�o,XMFLOAT3�^��Ԃ�
//----------------------------------
DirectX::XMFLOAT3 Mathf::ReturnFloatSubtract(const DirectX::XMFLOAT3& subtract1, const DirectX::XMFLOAT3& subtract2)
{
    DirectX::XMFLOAT3 subtract;
    DirectX::XMStoreFloat3(&subtract, DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&subtract1), DirectX::XMLoadFloat3(&subtract2)));
    return subtract;
}

//----------------------------------
// �x�N�g���̒����Z�o
//----------------------------------
float Mathf::VectorLength(const DirectX::XMVECTOR& vector)
{
    float anser;
    DirectX::XMStoreFloat(&anser, DirectX::XMVector3Length(vector));
    return anser;
}

//----------------------------------
// �p�x����O�������Z�o,XMVECTOR�^��Ԃ�
//----------------------------------
DirectX::XMVECTOR Mathf::ReturnVectorFront(const DirectX::XMFLOAT3& angle)
{
    DirectX::XMFLOAT3 front = DirectX::XMFLOAT3(sinf(angle.y), 0, cosf(angle.y));
    return DirectX::XMLoadFloat3(&front);
}

//----------------------------------
// �p�x����O�������Z�o,XMFLOAT3�^��Ԃ�
//----------------------------------
DirectX::XMFLOAT3 Mathf::ReturnFloatFront(const DirectX::XMFLOAT3& angle)
{
    return DirectX::XMFLOAT3(sinf(angle.x), 0, cosf(angle.z));
}

//----------------------------------
// ���ώZ�o,XMVECTOR�^��Ԃ�
//----------------------------------
DirectX::XMVECTOR Mathf::ReturnVectorDot(const DirectX::XMVECTOR& vector1, const DirectX::XMVECTOR& vector2)
{
    return DirectX::XMVector3Dot(vector1, vector2);
}

//----------------------------------
// ���ώZ�o,XMFLOAT3�^��Ԃ�
//----------------------------------
float Mathf::ReturnFloatDot(const DirectX::XMVECTOR& vector1, const DirectX::XMVECTOR& vector2)
{
    float dot;
    DirectX::XMStoreFloat(&dot, DirectX::XMVector3Dot(vector1, vector2));
    return dot;
}

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
// ���f���̃m�[�h���W�擾
void Mathf::GetNodePosition(const char* node_name, DirectX::XMFLOAT3& node_position, Model* model)
{
    Model::Node* node = model->FindNode(node_name);
    if (node != nullptr)
    {
        node_position.x = node->world_transform._41;
        node_position.y = node->world_transform._42;
        node_position.z = node->world_transform._43;
    }
}
