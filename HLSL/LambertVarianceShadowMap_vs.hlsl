#include "LambertVarianceShadowMap.hlsli"

PSInput main(VSInput input) : SV_POSITION
{
	PSInput output = (PSInput)0;

	float3 p = { 0, 0, 0 };
	float3 n = { 0, 0, 0 };
	for (int i = 0; i < 4; ++i)
	{
		p += (BoneWeights[i] * mul(input.Position, BoneTransforms[BoneIndices])).xyz;
		n += (BoneWeights[i] * mul(float4(input.Normal.xyz, 0), BoneTransforms[BoneIndices])).xyz;
	}

	output.Position = mul(float4(p, 1.0f), ViewProjection);

	float3 N = normalize(n);
	float3 L = normalize(-LightDirection.xyz);
	float d = dot(L, N);
	float power = max(0, d) * 0.5f + 0.5f;
	output.wNormal = N;
	output.Color.rgb = input.Color.rgb * MaterialColor.rgb * power;
	output.Color.a = input.Color.a * MaterialColor.a;
	output.Texcoord = input.Texcoord;

	output.vShadow = GetShadowTex(LightViewProjection, output.Position);
	return output;
}