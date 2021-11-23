#include "MakeCubeMap.hlsli"
[maxvertexcount(18)]
void main(
	triangle GSInput input[3],
	inout TriangleStream< PSInput > output
)
{
	for (int f = 0; f < 6; f++)
	{
		PSInput element = (PSInput)0;
		element.RTindex = f;
		for (int i = 0; i < 3; i++)
		{
			//�@�r���[�E���e�ϊ�
			float4 P = mul(view_mat[f], input[i].position);
			element.position = mul(cubemap_projection, P);
			//�@���[���h�@��
			element.wnormal = normalize(input[i].normal).xyz;

			element.color = input[i].color;

			//�e�N�X�`���[���W
			element.texcoord = input[i].texcoord;
			//�ǉ�
			output.Append(element);
		}
		output.RestartStrip();
	}
}