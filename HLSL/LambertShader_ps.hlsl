#include "LambertShader.hlsli"
//float4 main(PSInput input) : SV_TARGET
//{
//	return diffusetexture.Sample(decalsampler, input.texcoord) * input.color;
//}

Texture2D diffuseMap : register(t0);
SamplerState diffuseMapSamplerState : register(s0);

float4 main(VS_OUT pin) : SV_TARGET
{
	return diffuseMap.Sample(diffuseMapSamplerState, pin.texcoord) * pin.color;
}
