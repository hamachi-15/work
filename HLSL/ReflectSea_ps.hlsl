#include "ReflectSea.hlsli"

//--------------------------------------------
//	�s�N�Z���V�F�[�_�[
//--------------------------------------------
float4 main(PSInput input) : SV_Target
{
	float4 color = diffuse_texture.Sample(decal_sampler,input.Tex);
	float3 E = normalize(input.wPosition - eye_pos);
	float3 N = normalize(input.wNormal);
	float3 L = normalize(light_direction.xyz);
	float3 C = light_color.rgb;

	float3 Tex = float3(input.Tex * wave_size, 0);

	// �@���擾
	float3 N1 = normal_texture.Sample(wrap_sampler, Tex + wave1 * wave_time).xyz;
	N1 = N1 * 2.0f - 1.0f;
	// �@���擾
	float3 N2 = normal_texture.Sample(wrap_sampler, Tex + wave2 * wave_time).xyz;
	N2 = N2 * 2.0f - 1.0f;

	// �ڋ�Ԏ�
	float3 vx = normalize(input.vT);
	float3 vy = normalize(input.vB);
	float3 vz = normalize(input.vN);
	// �@������
	float3 wave_normal = N1 + N2;

	// �@�����[���h�ϊ�
	float3x3 mat = { vx,vy,vz };

	// ���[���h�ϊ�
	wave_normal = normalize(mul(wave_normal, mat));
	// 
	float3 KS = {1,1,1};
	float3 S = PhongSpecular(wave_normal, L, C, -E, KS, 20);

	//����
	float3 R1 = (float3)0;
	R1 = reflect(E, N);

	//�t���l���\��
	float ratio = saturate(dot(-E, N) + 0.2f);
	R1 = R1 + wave_normal * fluctuation;
	color.rgb = color.rgb * ratio + cube_texture.Sample(decal_sampler, R1).rgb * (1.0 - ratio);
	color.rgb += S;

	return color;
}
