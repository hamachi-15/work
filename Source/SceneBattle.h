#pragma once
#include <memory>
#include <d3d11.h>
#include "Scene.h"

struct RenderContext;
struct BlurRenderContext;
struct PrimitiveContext;
class Texture;
class Sprite;
class CameraController;
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

	// �V�[�����擾
	std::string GetName() const override { return "SceneBattle"; }

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

	// �Q�[���I�[�o�[�E�N���A�̏u�Ԃ�`��
	void MomentRender(ID3D11DeviceContext* context, const DirectX::XMFLOAT2& screen_size);

	//�o�b�N�o�b�t�@�`��
	void BuckBufferRender(ID3D11DeviceContext* context, RenderContext* render_context, const DirectX::XMFLOAT2& screen_size);

	// ���b�Z�[�W����
	bool OnMessages(const Telegram& telegram) override;
	
	// GUI�`��
	void OnGui();

	// �Q�[���N���A���菈��
	bool IsGameClearJudgment();
private:
	std::unique_ptr<RenderContext>						render_context;
	std::unique_ptr<BlurRenderContext>					blur_render_context;
	std::unique_ptr<PrimitiveContext>					primitive_context;

	std::unique_ptr<Sprite>				sprite;
	std::shared_ptr<Texture>			sky;
	std::shared_ptr<Texture>			anybutton_texture;
	Texture*							bloom_texture;

	//���C�g
	DirectX::XMFLOAT3					LightDir = { 0,0,0 };

	DirectX::XMFLOAT3					shadow_color = { 0.5f, 0.5f, 0.5f };

	bool								sky_bloom_flag = false; // ��̃u���[���̕`��t���O
	bool								primitive_falg = false;
	bool								isshadowmap = false;
	bool								isgame_clear = false;
	bool								isgame_over = false;
	bool								isbuttle_end = false;
};