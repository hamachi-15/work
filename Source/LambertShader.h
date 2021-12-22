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
	void Begin(ID3D11DeviceContext* dc, RenderContext& rc);
	void Draw(ID3D11DeviceContext* dc, const Model* model);
	// 描画終了処理
	void End(ID3D11DeviceContext* dc) ;

private:
	static const int MaxBones = 128;

	struct CbScene
	{
		DirectX::XMFLOAT4X4	viewProjection;
		DirectX::XMFLOAT4	lightDirection;
	};

	struct CbMesh
	{
		DirectX::XMFLOAT4X4	boneTransforms[MaxBones];
	};

	struct CbSubset
	{
		DirectX::XMFLOAT4	materialColor;
	};


	Microsoft::WRL::ComPtr<ID3D11Buffer>			scene_constant_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			mesh_constant_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			subset_constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>		sampler_state;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>		vertex_shader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader>	geometry_shader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		pixel_shader = nullptr;

	Microsoft::WRL::ComPtr<ID3D11InputLayout>		input_layout = nullptr;

};
