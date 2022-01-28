#pragma once
#include <memory>
#include <DirectXMath.h>
#include "Shader.h"
#include "Scene.h"
#include "RenderContext.h"

class Texture;
class Sprite;
class CameraController;
class Bloom;

class SceneGame : public Scene
{
public:
	SceneGame();
	~SceneGame() override;
private:

	// ����������
	void Initialize() override;

	// �I������
	void Finalize() override;

	// �X�V����
	void Update(float elapsed_time) override;

	// �`�揈��
	void Render() override;

	// �X�N���[���e�N�X�`���ɕ`��
	void ScreenRender(ID3D11DeviceContext* context, RenderContext& render_context, const DirectX::XMFLOAT2& screen_size);

	// �|�X�g�G�t�F�N�g�Ɏg���e�N�X�`���`��
	void PostRender(ID3D11DeviceContext* context, RenderContext& render_context, const DirectX::XMFLOAT2& screen_size);

	// �o�b�N�o�b�t�@�ɕ`��
	void BuckBufferRender(ID3D11DeviceContext* context, RenderContext& render_context, const DirectX::XMFLOAT2& screen_size);

	// ���b�Z�[�W����
	bool OnMessages(const Telegram& telegram) override;

	// ImGui�`��
	void OnGui();
private:
	const int Primitive_Max_Time = 40;

private:
	RenderContext						render_context;
	BlurRenderContext					blur_render_context;
	PrimitiveContext					primitive_context;

	std::unique_ptr<Shader>				sprite_cube;
	std::unique_ptr<Bloom>				bloom;
	std::unique_ptr<Shader>				primitive;

	std::unique_ptr<Sprite>				sprite;
	std::unique_ptr<Texture>			sky;
	std::unique_ptr<Texture>			bulr_texture;
	std::unique_ptr<Texture>			sky_texture;
	std::unique_ptr<Texture>			depth_texture;
	Texture*							bloom_texture;

	// �o�g���V�[���ɑJ�ڂ���Ƃ��Ɏg�������o
	std::vector<int>					search_enemy_id;			// �o�g���ɎQ������GID�z��
	float								enemy_search_range = 30.0f; // �G���J�E���g���̍��G�͈�

	//���C�g
	DirectX::XMFLOAT3					LightDir = { 0,0,0 };
	DirectX::XMFLOAT3					shadow_color = { 0.5f, 0.5f, 0.5f };
	bool								battle_flag = false;
	bool								primitive_falg = false;
	bool								isshadowmap = false;

	DirectX::XMFLOAT4					color = DirectX::XMFLOAT4(1, 1, 1, 1);
	bool								iscull = false;
};