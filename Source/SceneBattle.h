#pragma once
#include <memory>
#include <d3d11.h>
#include "Scene.h"
#include "RenderContext.h"

class Texture;
class Sprite;
class CameraController;
class GaussianBlur;
class Bloom;
class Shader;

//-------------------------------------
// 
// �o�g���V�[���N���X
// 
//-------------------------------------
class SceneBattle : public Scene
{
public:
	// �R���X�g���N�^
	SceneBattle();

	// �f�X�g���N�^
	~SceneBattle() override;

	// ����������
	void Initialize() override;

	// �I��������
	void Finalize() override;

	// �X�V����
	void Update(float elapsed_time) override;

	// �`�揈��
	void Render() override;

	// �X�N���[���e�N�X�`���`��
	void ScreenRender(ID3D11DeviceContext* context, RenderContext& render_context, const DirectX::XMFLOAT2& screen_size);

	// �|�X�g�e�N�X�`���`��
	void PostRender(ID3D11DeviceContext* context, RenderContext& render_context, const DirectX::XMFLOAT2& screen_size);

	//�o�b�N�o�b�t�@�`��
	void BuckBufferRender(ID3D11DeviceContext* context, RenderContext& render_context, const DirectX::XMFLOAT2& screen_size);

	// GUI�`��
	void OnGui();
private:
	RenderContext render_context;
	BlurRenderContext blur_render_context;
	PrimitiveContext					primitive_context;
	std::unique_ptr<Shader> sprite_cube;
	std::unique_ptr<Bloom> bloom;
	std::unique_ptr<Shader>				primitive;

	std::unique_ptr<Sprite>		sprite;
	std::unique_ptr<Texture>	sky;
	Texture*					bloom_texture;


	std::unique_ptr<CameraController>camera_controller;
	//���C�g
	DirectX::XMFLOAT3 LightDir = { 0,0,0 };

	DirectX::XMFLOAT3 shadow_color = { 0.5f, 0.5f, 0.5f };
	std::unique_ptr<GaussianBlur>		 bulr;

	bool								primitive_falg = false;
	bool								isshadowmap = false;
};