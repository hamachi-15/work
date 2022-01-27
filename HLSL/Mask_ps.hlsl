#include "Mask.hlsli"

float4 main(PSInput input) : SV_TARGET0
{
	//�e�N�X�`���摜
	float4 color = DiffuseTexture.Sample(DecalSampler, input.texcoord);
	color *= input.color;

	//�}�X�N�̖���
	float mask = MaskTexture.Sample(DecalSampler, input.texcoord).r;

	float threshold = 1.0f - dissolve_threshold;
	mask = step(mask, threshold);

	//�s�����x�␳
	color.a *= mask;

	return color;
}