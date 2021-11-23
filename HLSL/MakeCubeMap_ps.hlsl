#include "MakeCubeMap.hlsli"
//--------------------------------------------
//	ピクセルシェーダー
//--------------------------------------------
float4 main(PSInput input) :SV_TARGET
{
	float4 color = diffuse_texture.Sample(decal_sampler,input.texcoord);
	float3 N = normalize(input.wnormal);
	float3 E = normalize(eye_pos.xyz - input.wposition);
	float3 L = normalize(light_direction.xyz);
	// 環境光
	//float3 A = ambient_color.rgb;
	float3 A = float3(1.0f, 1.0f, 1.0f);

	//拡散反射
	//float3 C = light_color.rgb;
	float3 C = float3(1.0f, 1.0f, 1.0f);
	float3 Kd = {1,1,1};
	float3 D = Diffuse(N, L, C, Kd);

	//鏡面反射
	float3 Ks = float3(0.5f, 0.5f, 0.5f);
	float3 S = BlinnPhongSpecular(N, L, C, E, Ks, 20);
	color *= input.color * float4(A + D + S, 1.0);
	return color;
}

