#include "Mask.hlsli"


PSInput main(VSInput input)
{
	PSInput output = (PSInput)0;

	float4 P = float4(input.position, 1.0);

	// 射影空間に変換.
	//P = mul(matWVP, P);

	// ワールド法線算出
	//float3 N = mul((float3x3)World, input.normal);
	//N = normalize(N);//正規化

	// 出力値設定.
	output.position = P;
	output.color = float4(1, 1, 1, 1);
	output.texcoord = input.texcoord;
	return output;
}