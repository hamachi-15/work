#include "ImGui.hlsli"
PSInput main(VSInput input)
{
	PSInput output;
	output.position = mul(input.position, world_view_projection);
	output.texcoord = input.texcoord;
	output.color = input.color;

	return output;
}