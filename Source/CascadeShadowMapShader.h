#pragma once
#include "Shader.h"

class CascadeShadowMap : public Shader
{
public:
	// コンストラクタ
	CascadeShadowMap(ID3D11Device* device);
	// デストラクタ
	~CascadeShadowMap() override;

	// 名前設定
	const char* GetShaderName() const override { return "CascadeShadowMap"; }

	// 開始処理
	void Begin(ID3D11DeviceContext* context, RenderContext* render_context)override;

	// 終了処理
	void End(ID3D11DeviceContext* context)override;
private:
	struct CBScene
	{
		DirectX::XMFLOAT4X4 light_view_projection[3];
		DirectX::XMFLOAT4X4	view_projection;
		DirectX::XMFLOAT4	light_direction;
		DirectX::XMFLOAT4	eye_position;
		DirectX::XMFLOAT4	shadow_parameter;	//	xyz カラー、w バイアス
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer>			scene_constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>		sampler_state;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>		sampler_shadow;

};