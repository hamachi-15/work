#include "3DObj.hlsli"


//--------------------------------------------
//	�s�N�Z���V�F�[�_�[
//--------------------------------------------
float4 main(PSInput input) : SV_Target
{
	float4 color = DiffuseTexture.Sample(DecalSampler,input.Tex);
	float3 N = normalize(input.wNormal);
	float3 E = normalize(EyePos.xyz - input.wPosition);
	float3 L = normalize(LightDir.xyz);
	// ����
	float3 A = AmbientColor.rgb;

	//�g�U����
	float3 C = LightColor.rgb;
	float3 Kd = float3(1, 1, 1);
	float3 D = Diffuse(N, L, C);

	//���ʔ���
	float3 Ks = float3(0.5f, 0.5f, 0.5f);
	float3 S = BlinnPhongSpecular(N, L, C, E, Ks, 20);
	color *= input.Color * float4(A + D + S, 1.0);
	return color;
}
