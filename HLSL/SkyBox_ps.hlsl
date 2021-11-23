#include "SkyBox.hlsli"
//--------------------------------------------
//	視線ベクトルを正距円筒図法に則ったUV座標へ変換する
//--------------------------------------------
float2 EquirectangularProjection(float3 v)
{
	static const float PI = 3.141592654f;
	float2 tex;
	tex.x = 1.0f - ((1.0f / (2 * PI)) * atan2(v.z, v.x) + 0.5f);
	tex.y = -(1.0f / PI) * atan2(v.y, length(v.xz)) + 0.5f;

	return tex;
}
float4 main(PSInput input) : SV_TARGET
{
	float3	E = normalize(input.world_pos.xyz - eye_pos.xyz);
	float4 color = diffuse_texture.Sample(decal_sampler, EquirectangularProjection(E)); 
	float3 fog_color = float3(0.0f, 0.0f, 0.5f);
	color.rgb = Fog(color.rgb, eye_pos, input.world_pos, fog_color, 20.0f, 40.0f);
	return color;
}