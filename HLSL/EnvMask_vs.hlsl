#include "EnvMask.hlsli"
//--------------------------------------------
//	�s�N�Z���V�F�[�_�[
//--------------------------------------------
PSInput main(VSInput input)
{
	PSInput output;
	output.position = float4(input.position, 1);
	output.color = input.color;
	output.texcoord = input.texcoord;

	return output;
}