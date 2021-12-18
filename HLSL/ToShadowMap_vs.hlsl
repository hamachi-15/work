#include "ToShadowMap.hlsli"

PSInputShadow main(VSInput input)
{
	float3 p = { 0, 0, 0 };
	for (int i = 0; i < 4; ++i)
	{
		p += (input.bone_weights[i] * mul(input.position, boneTransforms[input.bone_indices[i]])).xyz;
	}

	PSInputShadow output = (PSInputShadow)0;

	output.position = mul(float4(p, 1.0f), light_view_projection);
	output.depth = output.position;
	return output;
}