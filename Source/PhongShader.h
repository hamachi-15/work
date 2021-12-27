#pragma once
#include "Shader.h"

class Phong : public Shader
{
public:
	Phong(ID3D11Device* device);
	~Phong(){}

	// –¼‘OŽæ“¾
	const char* GetShaderName() const override { return "Phong"; }
	
	void Begin(ID3D11DeviceContext* context, RenderContext& render_context, DirectX::XMFLOAT4 light_color);

	void End(ID3D11DeviceContext* context)override;

private:
	struct CbScene
	{
		DirectX::XMFLOAT4X4	view_projection;
		DirectX::XMFLOAT4	light_direction;
		DirectX::XMFLOAT4	light_color;
		DirectX::XMFLOAT4	eye_position;
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer>			scene_constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>		sampler_state;

};