#include "Bright.hlsli"

// ���_�V�F�[�_�[
PSInput main(VSInput input)
{
	PSInput output = (PSInput)0;

	// �o�͒l�ݒ�
	output.position = float4(input.position, 1.0f);
	output.color = input.color;
	output.texcoord = input.texcoord;

	return output;
}