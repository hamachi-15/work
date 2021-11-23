#include "Sprite.hlsli"

PSInput main(VSInput input)
{
	PSInput output;
	output.position = float4(input.position, 1);
	output.color = input.color;
	output.texcoord = input.texcoord;

	return output;
}