#pragma once
#include "Shader.h"

class CascadeShadowMap : public Shader
{
public:
	// �R���X�g���N�^
	CascadeShadowMap(ID3D11Device* device);
	// �f�X�g���N�^
	~CascadeShadowMap() override;

	// ���O�ݒ�
	const char* GetShaderName() const override { return "CascadeShadowMap"; }

	// �J�n����
	void Begin(ID3D11DeviceContext* context, RenderContext& render_context)override;

	// �`�揈��
	void Draw(ID3D11DeviceContext* context, const Model* model) override;

	// �I������
	void End(ID3D11DeviceContext* context)override;
private:
	static const int		MaxBones = 128;
	struct CBScene
	{
		DirectX::XMFLOAT4X4 light_view_projection[3];
		DirectX::XMFLOAT4X4	view_projection;
		DirectX::XMFLOAT4	light_direction;
		DirectX::XMFLOAT4	eye_position;
		DirectX::XMFLOAT4	shadow_parameter;	//	xyz �J���[�Aw �o�C�A�X
	};

	struct CBMesh
	{
		DirectX::XMFLOAT4X4	boneTransforms[MaxBones];
	};
	struct CBSubset
	{
		DirectX::XMFLOAT4	materialColor;
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer>			scene_constant_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			mesh_constant_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			subset_constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>		sampler_state;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>		sampler_shadow;

};