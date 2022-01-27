#include "Mask.hlsli"

float4 main(PSInput input) : SV_TARGET0
{
	//テクスチャ画像
	float4 color = DiffuseTexture.Sample(DecalSampler, input.texcoord);
	color *= input.color;

	//マスクの明暗
	float mask = MaskTexture.Sample(DecalSampler, input.texcoord).r;

	float threshold = 1.0f - dissolve_threshold;
	mask = step(mask, threshold);

	//不透明度補正
	color.a *= mask;

	return color;
}