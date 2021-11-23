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
			//　ビュー・投影変換
			float4 P = mul(view_mat[f], input[i].position);
			element.position = mul(cubemap_projection, P);
			//　ワールド法線
			element.wnormal = normalize(input[i].normal).xyz;

			element.color = input[i].color;

			//テクスチャー座標
			element.texcoord = input[i].texcoord;
			//追加
			output.Append(element);
		}
		output.RestartStrip();
	}
}