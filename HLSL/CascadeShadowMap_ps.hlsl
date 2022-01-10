#include "CascadeShadowMap.hlsli"

float4 main(PSInput input) : SV_TARGET
{
	float4 color = diffuse_texture.Sample(decal_sampler, input.texcoord);
	float3 N = normalize(input.normal);
	float3 E = normalize(eye_pos.xyz - input.wposition);
	float3 L = normalize(light_direction.xyz);
	float3 P = input.wposition;

	// ŠÂ‹«Œõ
	float3 A = float3(1.0f, 1.0f, 1.0f);

	//ŠgŽU”½ŽË
	float3 C = float3(1.0f, 1.0f, 1.0f);
	float3 Kd = float3(1, 1, 1);
	float3 D = Diffuse(N, L, C, Kd);

	//‹¾–Ê”½ŽË
	float3 Ks = float3(1, 1, 1);
	float3 S = BlinnPhongSpecular(N, L, C, E, Ks, 20);
	color *= input.color * float4(A + D + S, 1.0);

	// ƒtƒHƒO
	float3 fog_color = float3(0.0f, 0.0f, 0.5f);
	color.rgb = Fog(color.rgb, eye_pos.xyz, P, fog_color, 500.0f, 700.0f);

	float3 CS = (float3)1;
	CS = GetCascadeShadow(P);
	color.rgb *= CS;
return color;
}