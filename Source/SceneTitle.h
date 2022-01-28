#pragma once
#include <memory>
#include "Scene.h"
#include "Sprite.h"
#include "Texture.h"
#include "CameraController.h"

class SceneTitle : public Scene
{
public:
	// �R���X�g���N�^�E�f�X�g���N�^
	SceneTitle() {};
	~SceneTitle()override{}

	// ����������
	void Initialize() override;

	// �I������
	void Finalize() override;

	// �X�V����
	void Update(float elapsed_time) override;

	// �X�N���[���e�N�X�`���`��
	void ScreenRender(ID3D11DeviceContext* context, RenderContext& render_context, const DirectX::XMFLOAT2& screen_size);

	// �o�b�N�o�b�t�@�`��
	void BuckBufferRender(ID3D11DeviceContext* context, RenderContext& render_context, const DirectX::XMFLOAT2& screen_size);
	
	// �`�揈��
	void Render() override;

	// �{�^���`��
	void RenderButton(ID3D11DeviceContext* context, Texture* texture, int index);

	// ImGui�`��
	void OnGui();
private:
	RenderContext render_context;
	BlurRenderContext blur_render_context;

	std::unique_ptr<Sprite> sprite;
	std::unique_ptr<Texture> sky;
	std::unique_ptr<Texture> mask_texture;

	//���C�g
	DirectX::XMFLOAT3 LightDir = { 0,0,0 };

	float timer = 0.0f; // �f�B�\�[�v�Ɏg��
};