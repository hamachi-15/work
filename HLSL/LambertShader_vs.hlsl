#include "LambertShader.hlsli"
//PSInput main(VSInput input)
//{
//	PSInput output;
//
//	float3 p = { 0, 0, 0 };
//	float3 n = { 0, 0, 0 };
//	for (int i = 0; i < 4; ++i)
//	{
//		p += (input.bone_weights[i] * mul(input.position, bone_transforms[input.bone_indices[i]])).xyz;
//		n += (input.bone_weights[i] * mul(float4(input.normal.xyz, 0), bone_transforms[input.bone_indices[i]])).xyz;
//	}
//	output.position = mul(float4(p, 1.0f), view_projection);
//
//	float3 N = normalize(n);
//	float3 L = normalize(-light_direction.xyz);
//	float d = dot(L, N);
//	float power = max(0, d) * 0.5f + 0.5f;
//	output.color.rgb = /*input.color.rgb * */ material_color.rgb * power;
//	output.color.a = /*input.color.a * */ material_color.a;
//	output.texcoord = input.texcoord;
//
//	return output;
//}
VS_OUT main(
	float4 position     : POSITION,
	float3 normal : NORMAL,
	float3 tangent : TANGENT,
	float2 texcoord : TEXCOORD,
	float4 color : COLOR,
	float4 boneWeights : WEIGHTS,
	uint4  boneIndices : BONES
)
{
	float3 p = { 0, 0, 0 };
	float3 n = { 0, 0, 0 };
	for (int i = 0; i < 4; i++)
	{
		p += (boneWeights[i] * mul(position, boneTransforms[boneIndices[i]])).xyz;
		n += (boneWeights[i] * mul(float4(normal.xyz, 0), boneTransforms[boneIndices[i]])).xyz;
	}

	VS_OUT vout;
	vout.position = mul(float4(p, 1.0f), viewProjection);

	float3 N = normalize(n);
	float3 L = normalize(-lightDirection.xyz);
	float d = dot(L, N);
	float power = max(0, d) * 0.5f + 0.5f;
	vout.color.rgb = color.rgb * materialColor.rgb * power;
	vout.color.a = color.a * materialColor.a;
	vout.texcoord = texcoord;

	return vout;
}
