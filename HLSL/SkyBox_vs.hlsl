#include "SkyBox.hlsli"

PSInput main(VSInput input)
{
	PSInput output = (PSInput)0;

	output.position = float4(input.position.xyz, 1.0f);

	float4 p = mul(inverse_projection, float4(input.position.xyz, 1.0f));
	output.world_pos = mul(inverse_view, p / p.w).xyz;
	output.texcoord = input.texcoord;
	output.color = input.color;

	return output;
}