#pragma once
#include "Shader.h"

class Obj : public Shader
{
public:
	Obj(ID3D11Device* device);
	~Obj(){}
	// ���O�擾
	const char* GetShaderName() const override { return "CubeMap"; }

	void Begin(ID3D11DeviceContext* context, RenderContext& render_context);

	void Draw(ID3D11DeviceContext* context, const Model* model) {}

	void End(ID3D11DeviceContext* context);

	struct CBScene
	{
		DirectX::XMFLOAT4	LightColor;		//���C�g�̐F
		DirectX::XMFLOAT4	LightDir;		//���C�g�̕���
		DirectX::XMFLOAT4	AmbientColor;	//����
		DirectX::XMFLOAT4	EyePos;			//�J�����ʒu
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer>			scene_constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11BlendState>		blend_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depth_stencil_state;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>		sampler_state;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>		sampler_shadow;
};