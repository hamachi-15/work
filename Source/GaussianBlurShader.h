#pragma once
#include <memory>
#include <d3d11.h>
#include <wrl.h>
#include "RenderContext.h"

// �O���錾
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
	// �R���X�g���N�^
	GaussianBlur(ID3D11Device* device, DXGI_FORMAT format = DXGI_FORMAT_R32G32_FLOAT);
	~GaussianBlur();

	// ���O�擾
	//const char* GetShaderName() const override { return "GaussianBlur"; }
	
	// �`��J�n����
	void Begin(ID3D11DeviceContext* context, BlurType type);

	// �u���[����
	Texture* Render(Texture* texture);

	// �`��I������
	void End(ID3D11DeviceContext* context, BlurType type);

	void CalcGaussianFilter(float* weight, float blur_power);

	GaussianXBlur* GetGaussianXBlurShader()  { return xblur.get(); }
	GaussianYBlur* GetGaussianYBlurShader()  { return yblur.get(); }
private:
	enum { NumWeights = 8 };				//�d�݂̐��B

	// �u���[���|����Ƃ��̒萔�o�b�t�@
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