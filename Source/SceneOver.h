#pragma once
#include <memory>
#include "Scene.h"
#include "CameraController.h"

struct RenderContext;
class Sprite;
class Texture;

class SceneOver : public Scene
{
public:
	// �R���X�g���N�^
	SceneOver(){}

	// �f�X�g���N�^
	~SceneOver() override {}

	// �V�[�����擾
	std::string GetName() const override { return "SceneOver"; }

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

	//�o�b�N�o�b�t�@�`��
	void BuckBufferRender(ID3D11DeviceContext* context, RenderContext* render_context, const DirectX::XMFLOAT2& screen_size);

	// ���b�Z�[�W����
	bool OnMessages(const Telegram& telegram) override { return false; }

	// GUI�`��
	void OnGui() {};
private:
	std::unique_ptr<Sprite>				sprite;
	std::shared_ptr<Texture>			anybutton_texture;
	std::unique_ptr<Texture>			over_texture;
	std::unique_ptr<Texture>			over_timing_texture;
};

