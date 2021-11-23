//--------------------------------------------
//�����o�[�g�V�F�[�f�B���O
//--------------------------------------------
// N:�@��(���K���ς�)
// L:���˃x�N�g��(���K���ς�)
// C:���ˌ�(�F�E����)
// K:���˗�(0�`1.0)
float3 Diffuse(float3 N, float3 L, float3 C, float3 K)
{
	float D = dot(N, -L);
	D = max(0, D);			// ���̒l���O�ɂ���
	return K * C * D;
}
float3 Diffuse(float3 N, float3 L, float3 C)
{
	float D = dot(N, -L);
	D = max(0, D);			// ���̒l���O�ɂ���
	return C * D;
}

//--------------------------------------------
//	���ʔ��ˊ֐�(�u�����E�t�H��)
//--------------------------------------------
// N:�@��(���K���ς�)
// L:���˃x�N�g��(���K���ς�)
// C:���ˌ�(�F�E����)
// E:���_�����x�N�g��(���K���ς�)
// K:���˗�(0�`1.0)
// Power:�n�C���C�g�̋���(�P���x)
float3 BlinnPhongSpecular(float3 N, float3 L, float3 C, float3 E,
	float3 K, float Power)
{
	float3 H = normalize(-L + E);

	float3 S = dot(H, N);
	S = max(0, S);
	S = pow(S, Power);
	S = S * K * C;

	return S;
}

//--------------------------------------------
//�t�H���V�F�[�f�B���O
//--------------------------------------------
float3 PhongSpecular(float3 N, float3 L, float3 C, float3 E,
	float3 K, float Power)
{
	float3 R = reflect(L, N);
	R = normalize(R);
	float3 S = dot(R, E);
	S = max(0, S);
	S = pow(S, Power);
	S = S * K * C;
	return S;
}

//--------------------------------------------
// ���[���h���W�����C�g��ԍ��W�n�ɕϊ�
//--------------------------------------------
float3 GetShadowTex(float4x4 view_projection, float3 wpos)
{
	float4 wvp_pos = float4(wpos, 1.0f);
	wvp_pos = mul(view_projection, wvp_pos);
	wvp_pos /= wvp_pos.w;
	// �e�N�X�`�����W�n
	wvp_pos.y = -wvp_pos.y;
	wvp_pos.xy = 0.5f * wvp_pos.xy + 0.5f;
	return wvp_pos.xyz;
}

//--------------------------------------------
// �V���h�E�}�b�v���烉�C�g��ԍ��W�n��Z�l��r
//--------------------------------------------
float3 GetShadow(Texture2D st, SamplerState ss, float3 tex, float4 parameter)
{
	float2	d = st.Sample(ss, tex.xy).rg;

	float v = max(0.0f, d.y - d.x * d.x);
	float e = tex.z - d.x;
	static float light_bleeding_factor = 0.15f;
	float s = saturate(v / (v + e * e));
	s = saturate((s - light_bleeding_factor) / (1.0f - light_bleeding_factor));
	return	lerp(1.0f, parameter.rgb + (1.0f - parameter.rgb) * s, tex.z - parameter.w >= d.x);
}
//float3 GetShadow(Texture2D st, SamplerState ss, float3 Tex, float3 Scolor,
//	float Bias)
//{ // �V���h�E�}�b�v����[�x�����o��
//	float d = st.Sample(ss, Tex.xy).r;
//	// �V���h�E�}�b�v�̐[�x�l�ƌ����̐[�x�̔�r
//	Scolor = (Tex.z - d > Bias) ? Scolor : float3(1, 1, 1);
//	return Scolor;
//}

//--------------------------------------------
// �t�H�O
//--------------------------------------------
// color	 : ���݂̐F
// view_pos	 : ���_
// pos		 : �s�N�Z���̈ʒu
// fog_color : �t�H�O�̐F
// near		 : �t�H�O�̋ߋ���
// far		 : �t�H�O�̉�����
float3 Fog(float3 color, float3 view_pos, float3 pos, float3 fog_color, float near, float far)
{
	float len = length(view_pos - pos);
	float fogalpha = saturate((len - near) / (far - near));
	return color.rgb * (1.0f - fogalpha) + fog_color * fogalpha;
}