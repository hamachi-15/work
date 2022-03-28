#pragma once

#pragma once
#include <memory>
#include"Shader.h"
#include "RenderContext.h"
#include "Texture.h"
#include "GaussianBlurShader.h"

class GaussianYBlur : public Shader
{
public:
	// コンストラクタ
	GaussianYBlur(ID3D11Device* device, u_int buffer_width, u_int buffer_height, DXGI_FORMAT format = DXGI_FORMAT_R32G32_FLOAT);
	~GaussianYBlur()override {}

	// 名前取得
	const char* GetShaderName() const override { return "GaussianYBlur"; }

	// 描画開始処理
	void Begin(ID3D11DeviceContext* context, Texture* texture)override;

	// 描画終了処理
	void End(ID3D11DeviceContext* context) override;

	Texture* GetBlurYTexture()  { return yblur_texture.get(); }

private:
	std::unique_ptr<Texture> yblur_texture;
	std::unique_ptr<Texture> depth_texture;
};