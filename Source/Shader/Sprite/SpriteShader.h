#pragma once
#include <memory>
#include <wrl.h>
#include "Shader.h"

class SpriteShader : public Shader
{
public:
	SpriteShader(ID3D11Device* device);
	~SpriteShader() override {}

	// 名前取得
	const char* GetShaderName() const override { return "SpriteShader"; }

	// 頂点シェーダー取得
	ID3D11VertexShader* GetVertexShader() const { return vertex_shader.Get(); }
	
	// ピクセルシェーダー取得
	ID3D11PixelShader* GetPixelShader() const { return pixel_shader.Get(); }
	
	// インプットレイアウト取得
	ID3D11InputLayout* GetInputLayout() const { return vertex_layout.Get(); }

	// 描画開始処理
	void Begin(ID3D11DeviceContext* context);

	// 描画終了処理
	void End(ID3D11DeviceContext* context);
private:

	//Microsoft::WRL::ComPtr<ID3D11Buffer>		vertex_buffer;

};