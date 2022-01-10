#include "CascadeShadowMap.hlsli"

PSInput main(VSInput input)
{
	float3 p = { 0, 0, 0 };
	float3 n = { 0, 0, 0 };
	for (int i = 0; i < 4; i++)
	{
		p += (input.bone_weights[i] * mul(input.position, bone_transforms[input.bone_indices[i]])).xyz;
		n += (input.bone_weights[i] * mul(float4(input.normal.xyz, 0), bone_transforms[input.bone_indices[i]])).xyz;
	}

	float3 wpos = p;

	PSInput output = (PSInput)0;
	output.position = mul(float4(p, 1.0f), view_projection);

	output.wposition = wpos;

	float3 N = normalize(n);
	output.normal = N;
	output.color.rgb = input.color.rgb;
	output.color.a = input.color.a;
	output.texcoord = input.texcoord;

	return output;
}