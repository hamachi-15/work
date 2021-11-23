#include "GaussianBlur.hlsli"
//--------------------------------------------
//	頂点シェーダー
//--------------------------------------------
PSInput main(VSInput input)
{
	PSInput output = (PSInput)0;

	// 出力値設定.
	//output.position = float4(input.position, 1.0f);
	//output.color = input.color;
	//output.tex0 = input.tex;

	return output;
}

