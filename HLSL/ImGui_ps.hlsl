#include "ImGui.hlsli"
float4 main(PSInput input) : SV_TARGET
{
	return texture0.Sample(sampler0, input.texcoord) * input.color;
}
