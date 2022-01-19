#include "Bright.hlsli"

float4 main(PSInput input) : SV_TARGET0
{
	float4 color = (float4)0;
	color = diffuse_texture.Sample(decal_sampler, input.texcoord) * input.color;
	//float3(0.2125f, 0.7154f, 0.0721f)
	//float t = dot(color.xyz, float3(0.2125f, 0.7154f, 0.0721f));

	// clip()関数は引数の値がマイナスになると、以降の処理をスキップする
	// なので、マイナスになるとピクセルカラーは出力されない
	// 今回の実装はカラーの明るさが1以下ならピクセルキルする
	//clip(t - 1.0f);
	
	float3 tgray = color.r * 0.299f
		+ color.g * 0.587f
		+ color.b * 0.114f;
	if (dot(tgray, tgray) > threshold) return color;

	color = float4(0, 0, 0, 0);
	return color;
}