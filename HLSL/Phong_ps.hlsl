#include "Phong.hlsli"

float4 main(PSInput input) : SV_TARGET
{
	float4 color = diffuse_texture.Sample(decal_sampler, input.texcoord);
	float3 N = normalize(input.normal);
	float3 E = normalize(eye_pos.xyz - input.wposition);
	float3 L = normalize(light_direction.xyz);
	float3 P = input.wposition;

	// ŠÂ‹«Œõ
	float3 A = float3(0.5f, 0.5f, 0.5f);

	//ŠgŽU”½ŽË
	float3 C = light_color.rgb;
	float3 Kd = float3(1, 1, 1);
	float3 D = Diffuse(N, L, C, Kd);

	//‹¾–Ê”½ŽË
	float3 Ks = float3(1, 1, 1);
	float3 S = BlinnPhongSpecular(N, L, C, E, Ks, 100);
	color *= input.color * float4(A + D + S, 1.0);

	return color;
}