#include "EnvMask.hlsli"

//--------------------------------------------
//	ピクセルシェーダー
//--------------------------------------------
float4 main(PSInput input) : SV_Target
{
	float4 color = diffuse_texture.Sample(decal_sampler,input.texcoord);

	// マスク値
	float mask = mask_texture.Sample(decal_sampler, input.texcoord).r;

	//閾値設定
	if (dissolve_threshold - 0.15f > 0.0f)
	{
		// 閾値が0より小さくなればクリッピング
		float threshold = step(mask, dissolve_threshold - 0.15f);
		clip(0.0f - threshold);
	}
	// 淵の色をつつける
	float threshold2 = (float)0;
	threshold2 = step(mask, dissolve_threshold);

	//環境マップとの合成
	float4 environment_color = float4(1, 0, 0, 1);
	color = color + environment_color * threshold2;

	return color;
}