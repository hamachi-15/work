#include "Mask.hlsli"


PSInput main(VSInput input)
{
	PSInput output = (PSInput)0;

	float4 P = float4(input.position, 1.0);

	// �ˉe��Ԃɕϊ�.
	//P = mul(matWVP, P);

	// ���[���h�@���Z�o
	//float3 N = mul((float3x3)World, input.normal);
	//N = normalize(N);//���K��

	// �o�͒l�ݒ�.
	output.position = P;
	output.color = float4(1, 1, 1, 1);
	output.texcoord = input.texcoord;
	return output;
}