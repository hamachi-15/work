#include "ReflectSea.hlsli"

//--------------------------------------------
//	���_�V�F�[�_�[
//--------------------------------------------
PSInput main(VSInput input)
{
	float3 p = { 0, 0, 0 };
	float3 n = { 0, 0, 0 };
	for (int i = 0; i < 4; i++)
	{
		p += (input.bone_weights[i] * mul(input.Position, bone_transforms[input.bone_indices[i]])).xyz;
		n += (input.bone_weights[i] * mul(float4(input.Normal.xyz, 0), bone_transforms[input.bone_indices[i]])).xyz;
	}
	
	PSInput output = (PSInput)0;

	output.Position = mul(float4(p, 1.0f), view_projection);

	float3 wPos = p;

	float3 wN = n;

	//�ڋ��
	float3 vN = wN;
	float3 vB = { 0, 1, 0.001f };
	float3 vT;
	vB = normalize(vB);
	vT = normalize(cross(vB, vN));
	vB = normalize(cross(vN, vT));

	// �o�͒l�ݒ�.
	output.Color = input.Color;

	//�ڋ�Ԏ�
	output.vT = vT;
	output.vB = vB;
	output.vN = vN;

	//�e�N�X�`���[���W
	output.Tex = input.Tex;
	output.wNormal = wN;
	output.wPosition = wPos;

	return output;
}
