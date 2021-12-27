#include "2DPrimitive.hlsli"
PSInput main(VSInput input)
{
	PSInput output = (PSInput)0;

	output.position = float4(input.position, 1);
	output.color = input.color;
	output.texcoord = input.texcoord;

	return output;
}