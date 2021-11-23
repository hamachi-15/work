#include "3DObj.hlsli"

//--------------------------------------------
//	頂点シェーダー
//--------------------------------------------
PSInput main(VSInput input)
{
	PSInput output = (PSInput)0;
	float4 P = float4(input.Position, 1.0);

	output.Position = mul(matWVP, P);

	float3 wPos = mul(World, P).xyz;

	float3 wN = mul((float3x3)World, input.Normal).xyz;
	wN = normalize(wN);

	output.Color = input.Color;
	output.Color = float4(1, 1, 1, 1);

	//テクスチャー座標
	output.Tex = input.Tex;
	output.wNormal = wN;
	output.wPosition = wPos;

	return output;
}
