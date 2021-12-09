#pragma once
#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include "Shader.h"
#include "Texture.h"

class Sprite
{
public:
	enum class Pivot
	{
		LeftTop,
		LeftCenter,
		LeftBottom,
		CenterTop,
		CenterCenter,
		CenterBottom,
		RightTop,
		RightCenter,
		RightBottom,
	};
public:
	// コンストラクタ
	Sprite();
	Sprite(const char* filename);
	Sprite(Texture* texture);
	// デストラクタ
	~Sprite() {}

	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT2 texcoord;
	};

	// 描画処理
	void Render(ID3D11DeviceContext* context, 
		float dx, float dy,
		float dw, float dh,
		float sx, float sy,
		float sw, float sh,
		float angle = 0.0f,
		float r = 1, float g = 1, float b = 1, float a = 1) const;

	void Render(ID3D11DeviceContext* context,
		float dx, float dy,
		float dw, float dh,
		float sx, float sy,
		float sw, float sh,
		float angle, Pivot pivot = Pivot::LeftTop,
		float r = 1, float g = 1, float b = 1, float a = 1) const;

	void Render(ID3D11DeviceContext* context,
		Texture* texture,
		float dx, float dy,
		float dw, float dh,
		float sx, float sy,
		float sw, float sh,
		float angle = 0.0f,
		float r = 1, float g = 1, float b = 1, float a = 1) const;
	void AddRender(ID3D11DeviceContext* context,
		Texture* texture,
		float dx, float dy,
		float dw, float dh,
		float sx, float sy,
		float sw, float sh,
		float angle = 0.0f,
		float r = 1, float g = 1, float b = 1, float a = 1) const;
	// テクスチャ幅取得
	int GetTextureWidth() const { return texture_width; }

	// テクスチャ高さ取得
	int GetTextureHeight() const { return texture_height; }

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer>		vertex_buffer;

	Microsoft::WRL::ComPtr<ID3D11BlendState>	blend_state;
	Microsoft::WRL::ComPtr<ID3D11BlendState>	add_blend_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depth_stencil_state;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>			sampler_state;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	shader_resource_view;

	std::unique_ptr<Texture> texture;

	int texture_width = 0;
	int texture_height = 0;

};



