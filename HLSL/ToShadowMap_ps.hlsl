#include "ToShadowMap.hlsli"

float4 main(PSInputShadow input) : SV_Target
{
	float4 color = input.depth.z / input.depth.w;
	color.a = 1.0f;
	
	return color;
}