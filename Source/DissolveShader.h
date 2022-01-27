#pragma once
#include <memory>
#include "Shader.h"


//**********************************
// 
// ディソープシェーダー
// 
//**********************************
class DissolveShader : public Shader
{
public:
	// コンストラクタ
	DissolveShader(ID3D11Device* device);
	
	// デストラクタ
	~DissolveShader() override {}

	// 名前取得
	const char* GetShaderName() const override { return "Dissolve"; }

	// 描画開始処理
	void Begin(ID3D11DeviceContext* context, float timer)override;

	// 描画処理
	void Render(ID3D11DeviceContext* context, Texture* texture, Texture* mask_texture);

	// 描画終了処理
	void End(ID3D11DeviceContext* context)override;
private:
	struct ConstantBufferForDissolve
	{
		float threshold;
		float dummy1;
		float dummy2;
		float dummy3;
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer>	constant_buffer;

	//std::unique_ptr<Texture>				mask_texture;
};