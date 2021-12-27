#pragma once
#include "Shader.h"
// 
enum class PrimitiveType
{
	Encount = 3
};
//***************************************
// 
// 2D�v���~�e�B�u�N���X
//
//***************************************
class Primitive : public Shader
{
public:
	// �R���X�g���N�^
	Primitive(ID3D11Device* device);
	~Primitive() override{}

	// ���O�擾
	const char* GetShaderName()const override { return "2DPrimitive"; }
	
	// �`��J�n����
	void Begin(ID3D11DeviceContext* cotnext, PrimitiveContext& number)override;

	// �`��I������
	void End(ID3D11DeviceContext* context)override;
private:
	// �R���X�^���g�o�b�t�@
	struct ConstantBuffer
	{
		float timer;
		int number;
		int dummy1;
		int dummy2;
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> constant_buffer = nullptr;

};