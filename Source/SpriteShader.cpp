#include "SpriteShader.h"
//#include "Misc.h"

//----------------------------------
// コンストラクタ
//----------------------------------
SpriteShader::SpriteShader(ID3D11Device* device)
{
	Create(device, "Shader\\Sprite_vs.cso", "Shader\\Sprite_ps.cso", true);
}

//----------------------------------
// 描画開始処理
//----------------------------------
void SpriteShader::Begin(ID3D11DeviceContext* context)
{
	Activate(context);
}

//----------------------------------
// 描画終了処理
//----------------------------------
void SpriteShader::End(ID3D11DeviceContext* context)
{
	InActivate(context);
}
