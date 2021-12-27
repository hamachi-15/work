#include "2DPrimitive.hlsli"

float4 main(PSInput input) : SV_TARGET0
{
	float2 texcoord = input.texcoord;
	//float4 color = (float4)0;
	
	switch (number)
	{
	case	0:	//	��ɉ�
	{
		float4 color = (float4)1;
		color.xyz = step(0.25f, texcoord.x);
		return color;
		break;
	}
	case	1:	//	��������
	{
		float4 color = (float4)1;
		color.xyz = step(0.25f, texcoord.x) * step(0.5f, 1 - texcoord.x)
			+ step(0.25f, texcoord.y) * step(0.5f, 1 - texcoord.y);
		return color;
		break;
	}
	case	2:	//	�~��̃t�F�[�h�C���G�t�F�N�g
	{
		float4 color = float4(0, 0, 0.5f, 1);
		float time = frac(timer / 50);
		float d1 = (time - 0.05f) *1.5f;
		if (Circle(texcoord, d1))
		{
			color = float4(0, 0, 0, 0);
		}

		return color;
		break;
	}
	case	3:	//	�~��̃G���J�E���g�G�t�F�N�g(�t�F�[�h�A�E�g)
	{
		float4 color = (float4)0;
		float time = frac(timer / 50);
		float d1 = (time + 0.05f);// *1.5f;
		if (Circle(texcoord, d1))
		{
			color = float4( 0, 0, 0.5f, 1 );
		}
		//color = Circle(texcoord, d1);
		return color;
		break;
	}
	case	4:	//	��
	{
		float4 color = (float4)1;
		color.xyz = Ring(texcoord, 0.4f, 0.05f);
		return color;
		break;
	}
	case	5:	//	�ւ𓮂���
	{
		float4 color = (float4)1;
		float r = 10.00f;
		float d = frac(timer / 100) * -r;
		color.xyz = Ring(texcoord, d, r);
		return color;
		break;
	}
	case	6:	//	���ː�
	{
		float4 color = (float4)1;
		color.xyz = Radiation(texcoord, 8, 0.75f);
		return color;
		break;
	}
	case	7:	//	���ː��A�j���[�V����
	{
		float4 color = (float4)1;
		float d = 0.75f + 0.5f * (1 - frac(timer / 100));
		color.xyz = Radiation(texcoord, 8, d);
		return color;
		break;
	}
	case	8:	//	���ː��{�ւ��g�����G�t�F�N�g
	{
		float4 color = (float4)1;
		float time = frac(timer / 60);
		float r = 0.05f;
		float d1 = (time + r) * 2.0f;
		float d2 = 0.75f + 0.3f * (sin(time * 2.0f) * 2.0f);
		color.a = Ring(texcoord, d1, r) + Circle(texcoord, d1) * Radiation(texcoord, 4, d2, time * 3.14f);
		clip(1 - length(texcoord - (float2)0.5f) - 0.5f);
		clip(color.a);
		return color;
		break;
	}
	}
	return 1;
}