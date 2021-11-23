#include "GaussianBlur.hlsli"

//--------------------------------------------
//	�s�N�Z���V�F�[�_�[
//--------------------------------------------
float4 main(PSInput input) : SV_TARGET0
{
	float4 color = input.color;

	color  = weight[0].x * diffuse_texture.Sample(decal_sampler, input.tex0.xy);
	color += weight[0].y * diffuse_texture.Sample(decal_sampler, input.tex1.xy);
	color += weight[0].z * diffuse_texture.Sample(decal_sampler, input.tex2.xy);
	color += weight[0].w * diffuse_texture.Sample(decal_sampler, input.tex3.xy);
	color += weight[1].x * diffuse_texture.Sample(decal_sampler, input.tex4.xy);
	color += weight[1].y * diffuse_texture.Sample(decal_sampler, input.tex5.xy);
	color += weight[1].z * diffuse_texture.Sample(decal_sampler, input.tex6.xy);
	color += weight[1].w * diffuse_texture.Sample(decal_sampler, input.tex7.xy);

	// ��e�N�Z���Ƀ}�C�i�X������8�e�N�Z���A�d�ݕt���ŃT���v�����O
	color += weight[0].x * diffuse_texture.Sample(decal_sampler, input.tex0.zw);
	color += weight[0].y * diffuse_texture.Sample(decal_sampler, input.tex1.zw);
	color += weight[0].z * diffuse_texture.Sample(decal_sampler, input.tex2.zw);
	color += weight[0].w * diffuse_texture.Sample(decal_sampler, input.tex3.zw);
	color += weight[1].x * diffuse_texture.Sample(decal_sampler, input.tex4.zw);
	color += weight[1].y * diffuse_texture.Sample(decal_sampler, input.tex5.zw);
	color += weight[1].z * diffuse_texture.Sample(decal_sampler, input.tex6.zw);
	color += weight[1].w * diffuse_texture.Sample(decal_sampler, input.tex7.zw);

	return color;
}