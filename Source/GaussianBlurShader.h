#pragma once
#include <memory>
#include <d3d11.h>
#include <wrl.h>
#include "RenderContext.h"

// 前方宣言
class GaussianXBlur;
class GaussianYBlur;
class Texture;

enum class BlurType
{
	XBlur,
	YBlur
};

class GaussianBlur
{
public:
	// コンストラクタ
	GaussianBlur(ID3D11Device* device, DXGI_FORMAT format = DXGI_FORMAT_R32G32_FLOAT);
	~GaussianBlur();

	// 名前取得
	//const char* GetShaderName() const override { return "GaussianBlur"; }
	
	// 描画開始処理
	void Begin(ID3D11DeviceContext* context, BlurType type);

	// ブラー処理
	Texture* Render(Texture* texture);

	// 描画終了処理
	void End(ID3D11DeviceContext* context, BlurType type);

	void CalcGaussianFilter(float* weight, float blur_power);

	GaussianXBlur* GetGaussianXBlurShader()  { return xblur.get(); }
	GaussianYBlur* GetGaussianYBlurShader()  { return yblur.get(); }
private:
	enum { NumWeights = 8 };				//重みの数。

	// ブラーを掛けるときの定数バッファ
	struct CBForPerGaussainBlur
	{
		float	weight[NumWeights];
		DirectX::XMFLOAT2 texture_size;
		float dummy;
		float dummy1;

	};
	CBForPerGaussainBlur constant_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			blur_constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11BlendState>		blend_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depth_stencil_state;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>		sampler_state;

	float blur_power = 5.0f;
	float weight[NumWeights] = {};

	std::unique_ptr<GaussianXBlur> xblur;
	std::unique_ptr<GaussianYBlur> yblur;
};