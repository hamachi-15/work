#include "EnvMask.hlsli"

//--------------------------------------------
//	�s�N�Z���V�F�[�_�[
//--------------------------------------------
float4 main(PSInput input) : SV_Target
{
	float4 color = diffuse_texture.Sample(decal_sampler,input.texcoord);

	// �}�X�N�l
	float mask = mask_texture.Sample(decal_sampler, input.texcoord).r;

	//臒l�ݒ�
	if (dissolve_threshold - 0.15f > 0.0f)
	{
		// 臒l��0��菬�����Ȃ�΃N���b�s���O
		float threshold = step(mask, dissolve_threshold - 0.15f);
		clip(0.0f - threshold);
	}
	// ���̐F������
	float threshold2 = (float)0;
	threshold2 = step(mask, dissolve_threshold);

	//���}�b�v�Ƃ̍���
	float4 environment_color = float4(1, 0, 0, 1);
	color = color + environment_color * threshold2;

	return color;
}