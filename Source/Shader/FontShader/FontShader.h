#pragma once
#include "Shader.h"

class FontShader : public Shader
{
public:
	// コンストラクタ・デストラクタ
	FontShader(ID3D11Device* device);
	~FontShader() override {}

	// 名前取得
	const char* GetShaderName() const override { return "FontShader"; }

	// 描画開始処理
	void Begin(ID3D11DeviceContext* context);

	// 描画終了処理
	void End(ID3D11DeviceContext* context);

};