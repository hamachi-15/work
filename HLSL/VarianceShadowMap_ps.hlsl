#include "VarianceShadowMap.hlsli"
float4 main(PSInputShadow input) : SV_TARGET0
{
	float4 color = (float4)0;
	float d = input.depth.z / input.depth.w;
	color.r = d;
	color.g = d * d;
	color.b = 1.0f;
	color.a = 1.0f;
	return color;
}