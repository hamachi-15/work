#include "Sprite.hlsli"

float4 main(PSInput input) : SV_TARGET
{
	return diffusetexture.Sample(decalsampler, input.texcoord) * input.color;
}