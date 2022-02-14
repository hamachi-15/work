#pragma once

#include <memory>
#include <wrl.h>
#include "Shader.h"

class LambertShader: public Shader
{
public:
	LambertShader(ID3D11Device* device);
	~LambertShader() override {}

	// 名前取得
	const char* GetShaderName() const override { return "Lambert"; }

	// 描画開始処理
	void Begin(ID3D11DeviceContext* context, RenderContext* render_context);

	// 描画終了処理
	void End(ID3D11DeviceContext* context) ;

private:
	struct CbScene
	{
		DirectX::XMFLOAT4X4	view_projection;
		DirectX::XMFLOAT4	light_direction;
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer>			scene_constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>		sampler_state;

};
