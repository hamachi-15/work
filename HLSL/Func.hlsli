//--------------------------------------------
//ランバートシェーディング
//--------------------------------------------
// N:法線(正規化済み)
// L:入射ベクトル(正規化済み)
// C:入射光(色・強さ)
// K:反射率(0～1.0)
float3 Diffuse(float3 N, float3 L, float3 C, float3 K)
{
	float D = dot(N, -L);
	D = max(0, D);			// 負の値を０にする
	return K * C * D;
}
float3 Diffuse(float3 N, float3 L, float3 C)
{
	float D = dot(N, -L);
	D = max(0, D);			// 負の値を０にする
	return C * D;
}

//--------------------------------------------
//	鏡面反射関数(ブリン・フォン)
//--------------------------------------------
// N:法線(正規化済み)
// L:入射ベクトル(正規化済み)
// C:入射光(色・強さ)
// E:視点方向ベクトル(正規化済み)
// K:反射率(0～1.0)
// Power:ハイライトの強さ(輝き度)
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
//フォンシェーディング
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
// ワールド座標をライト空間座標系に変換
//--------------------------------------------
float3 GetShadowTex(float4x4 view_projection, float3 wpos)
{
	float4 wvp_pos = float4(wpos, 1.0f);
	wvp_pos = mul(view_projection, wvp_pos);
	wvp_pos /= wvp_pos.w;
	// テクスチャ座標系
	wvp_pos.y = -wvp_pos.y;
	wvp_pos.xy = 0.5f * wvp_pos.xy + 0.5f;
	return wvp_pos.xyz;
}

//--------------------------------------------
// シャドウマップからライト空間座標系とZ値比較
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
//{ // シャドウマップから深度を取り出す
//	float d = st.Sample(ss, Tex.xy).r;
//	// シャドウマップの深度値と現実の深度の比較
//	Scolor = (Tex.z - d > Bias) ? Scolor : float3(1, 1, 1);
//	return Scolor;
//}

//--------------------------------------------
// フォグ
//--------------------------------------------
// color	 : 現在の色
// view_pos	 : 視点
// pos		 : ピクセルの位置
// fog_color : フォグの色
// near		 : フォグの近距離
// far		 : フォグの遠距離
float3 Fog(float3 color, float3 view_pos, float3 pos, float3 fog_color, float near, float far)
{
	float len = length(view_pos - pos);
	float fogalpha = saturate((len - near) / (far - near));
	return color.rgb * (1.0f - fogalpha) + fog_color * fogalpha;
}