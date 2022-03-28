#pragma once
#include <memory>
#include "Scene.h"
#include "CameraController.h"

class Sprite;
class Texture;
struct RenderContext;

class SceneClear : public Scene
{
public:
	// �R���X�g���N�^
	SceneClear(){}

	// �f�X�g���N�^
	~SceneClear() override {}

	// �V�[�����擾
	std::string GetName() const override { return "SceneClear"; }

	// ����������
	void Initialize() override;

	// �I��������
	void Finalize() override;

	// �X�V����
	void Update(float elapsed_time) override;

	// �`�揈��
	void Render() override;

	// �X�N���[���e�N�X�`���`��
	void ScreenRender(ID3D11DeviceContext* context, RenderContext* render_context, const DirectX::XMFLOAT2& screen_size);

	// �|�X�g�e�N�X�`���`��
	void PostRender(ID3D11DeviceContext* context, RenderContext* render_context, const DirectX::XMFLOAT2& screen_size);

	//�o�b�N�o�b�t�@�`��
	void BuckBufferRender(ID3D11DeviceContext* context, RenderContext* render_context, const DirectX::XMFLOAT2& screen_size);

	// ���b�Z�[�W����
	bool OnMessages(const Telegram& telegram) override { return true; }

	// GUI�`��
	void OnGui() {}
private:
	std::unique_ptr<Sprite>				sprite;
	std::shared_ptr<Texture>			anybutton_texture;
	std::unique_ptr<Texture>			clear_texture;
	std::unique_ptr<Texture>			clear_timing_texture;
};
