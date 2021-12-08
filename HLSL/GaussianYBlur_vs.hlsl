#include "GaussianBlur.hlsli"

PSInput main(VSInput input)
{
	PSInput output = (PSInput)0;

	// 出力値設定.
	output.position = float4(input.position, 1.0f);
	float2 tex = input.tex;

	// テクスチャサイズを取得
    float2 tex_size;
    float level;
    diffuse_texture.GetDimensions(tex_size.x, tex_size.y);
    float height = 360;

    tex_size.x = 0.0f;
    tex_size.y = height;

    // 基準テクセルからV座標を+1テクセルずらすためのオフセットを計算する
    output.tex0.xy = float2(0.0f, 1.0f / tex_size.y);

    // 基準テクセルからV座標を+3テクセルずらすためのオフセットを計算する
    output.tex1.xy = float2(0.0f, 3.0f / tex_size.y);

    // 基準テクセルからV座標を+5テクセルずらすためのオフセットを計算する
    output.tex2.xy = float2(0.0f, 5.0f / tex_size.y);

    // 基準テクセルからV座標を+7テクセルずらすためのオフセットを計算する
    output.tex3.xy = float2(0.0f, 7.0f / tex_size.y);

    // 基準テクセルからV座標を+9テクセルずらすためのオフセットを計算する
    output.tex4.xy = float2(0.0f, 9.0f / tex_size.y);

    // 基準テクセルからV座標を+11テクセルずらすためのオフセットを計算する
    output.tex5.xy = float2(0.0f, 11.0f / tex_size.y);

    // 基準テクセルからV座標を+13テクセルずらすためのオフセットを計算する
    output.tex6.xy = float2(0.0f, 13.0f / tex_size.y);

    // 基準テクセルからV座標を+15テクセルずらすためのオフセットを計算する
    output.tex7.xy = float2(0.0f, 15.0f / tex_size.y);
    
    // オフセットに-1を掛けてマイナス方向のオフセットも計算する
    output.tex0.zw = output.tex0.xy * -1.0f;
    output.tex1.zw = output.tex1.xy * -1.0f;
    output.tex2.zw = output.tex2.xy * -1.0f;
    output.tex3.zw = output.tex3.xy * -1.0f;
    output.tex4.zw = output.tex4.xy * -1.0f;
    output.tex5.zw = output.tex5.xy * -1.0f;
    output.tex6.zw = output.tex6.xy * -1.0f;
    output.tex7.zw = output.tex7.xy * -1.0f;

    // オフセットに基準テクセルのUV座標を足し算して、実際にサンプリングするUV座標に変換する
    output.tex0 += float4(tex, tex);
    output.tex1 += float4(tex, tex);
    output.tex2 += float4(tex, tex);
    output.tex3 += float4(tex, tex);
    output.tex4 += float4(tex, tex);
    output.tex5 += float4(tex, tex);
    output.tex6 += float4(tex, tex);
    output.tex7 += float4(tex, tex);


	return output;
}