#pragma once
#include <memory>
#include <wrl.h>
#include "Shader.h"

class SkyBoxShader : public Shader
{
public:
	// コンストラクタ
	SkyBoxShader(ID3D11Device* device);
	// デストラクタ
	~SkyBoxShader() override {}

	// 名前取得
	const char* GetShaderName()const override { return "SkyBox"; }

	// 描画開始
	void Begin(ID3D11DeviceContext* context, RenderContext& render);

	// 描画終了
	void End(ID3D11DeviceContext* context);

private:
	struct CBBuffers
	{
		DirectX::XMFLOAT4X4 inverse_view;
		DirectX::XMFLOAT4X4 inverse_projection;
		DirectX::XMFLOAT4	eye_position;
	};

	Microsoft::WRL::ComPtr<ID3D11VertexShader>	vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	pixel_shader;

	Microsoft::WRL::ComPtr<ID3D11InputLayout>	input_layout;

	Microsoft::WRL::ComPtr<ID3D11Buffer>		skybox_buffer;

};