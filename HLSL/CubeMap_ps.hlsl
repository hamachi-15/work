#include "CubeMap.hlsli"


//--------------------------------------------
//	ピクセルシェーダー
//--------------------------------------------

float4 main(PSInput input) : SV_TARGET0
{
	float4 color = (float4)0;
	//color = DiffuseTexture.Sample(DecalSampler, input.Tex);
	//color *= input.Color;

	float3 Tex = float3(input.Tex,1);

	color.rgb = CubeTexture.Sample(DecalSampler,input.Position.xyz).rgb;
	return color;
}
