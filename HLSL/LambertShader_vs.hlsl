#include "LambertShader.hlsli"
PSInput main(VSInput input)
{
	float3 p = { 0, 0, 0 };
	float3 n = { 0, 0, 0 };
	for (int i = 0; i < 4; i++)
	{
		p += (input.bone_weights[i] * mul(input.position, bone_transforms[input.bone_indices[i]])).xyz;
		n += (input.bone_weights[i] * mul(float4(input.normal.xyz, 0), bone_transforms[input.bone_indices[i]])).xyz;
	}

	PSInput vout = (PSInput)0;
	vout.position = mul(float4(p, 1.0f), view_projection);

	float3 N = normalize(n);
	float3 L = normalize(-light_direction.xyz);
	float d = dot(L, N);
	float power = max(0, d) * 0.5f + 0.5f;
	vout.color.rgb = input.color.rgb * power;
	vout.color.a = input.color.a;
	vout.texcoord = input.texcoord;

	return vout;
}
