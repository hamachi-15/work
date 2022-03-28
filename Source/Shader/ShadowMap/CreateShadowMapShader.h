#pragma once
#include "Shader.h"

class CreateShadowMap : public Shader
{
public:
	// コンストラクタ
	CreateShadowMap(ID3D11Device* device);
	// デストラクタ
	~CreateShadowMap() override{}

	// 名前取得
	const char* GetShaderName() const override { return "CreateShadowMap"; }

	// 描画開始処理
	void Begin(ID3D11DeviceContext* context, RenderContext* render_context)override;

	// 描画終了処理
	void End(ID3D11DeviceContext* context)override;

private:
	struct CBScene
	{
		DirectX::XMFLOAT4X4	view_projection;
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer>			scene_constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>		sampler_state;

};