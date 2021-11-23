#include "Font.hlsli"

float4 main(PSInput input) : SV_TARGET
{
	return diffuse_texture.Sample(decal_sampler, input.texcoord).r * input.color;
}