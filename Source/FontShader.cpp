#include "FontShader.h"

//----------------------------------
// コンストラクタ
//----------------------------------
FontShader::FontShader(ID3D11Device* device)
{
	Create(device, "Shader\\Font_vs.cso", "Shader\\Font_ps.cso");
}

//----------------------------------
// 描画開始処理
//----------------------------------
void FontShader::Begin(ID3D11DeviceContext* context)
{
	Activate(context);
}

//----------------------------------
// 描画終了処理
//----------------------------------
void FontShader::End(ID3D11DeviceContext* context)
{
	InActivate(context);
}
