#include "SkyBox.hlsli"

float4 main(PSInput input) : SV_TARGET
{
	float3	E = normalize(input.world_pos.xyz - eye_pos.xyz);
	float4 color = diffuse_texture.Sample(decal_sampler, EquirectangularProjection(E)); 
	return color;
}