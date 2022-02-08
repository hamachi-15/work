#pragma once
#include <string>
#include <memory>
#include <set>
#include <vector>
#include "RenderContext.h"
#include "ShaderManager.h"

// �O���錾
class GaussianBlur;
class Texture;
class Sprite;
class Actor;

class ActorManager
{
private:
	// �R���X�g���N�^�E�f�X�g���N�^
	ActorManager();
	~ActorManager();
public:
	// �C���X�^���X�쐬
	static ActorManager& Instance()
	{
		static ActorManager instance;
		return instance;
	}

	// �쐬����
	std::shared_ptr<Actor>Create();

	// �j���z��ɒǉ�
	void Remove(std::shared_ptr<Actor> actor);

	// �j������
	void Destroy(std::shared_ptr<Actor> actor);

	// �S�j������
	void AllDestroy();

	// �X�V����
	void Update(float elapsed_time);

	// �s��X�V����
	void UpdateTransform();

	// �V���h�E�}�b�v�`�揈��
	void ShadowRender(RenderContext& render_context, BlurRenderContext& bulr_render_context);
	
	// �`�揈��
	void Render(RenderContext& render_context);
	void BrightRender(RenderContext& render_context);

	// �V���h�E�}�b�v�擾
	Texture* GetShadowTexture(int index) const { return shadow_texture[index].get(); }
	Texture* GetShadowBulrTexture(int index) const { return shadow_vsm_texture[index].get(); }
	Texture* GetDepthTexture(int index) const { return depth_texture[index].get(); }

	// �A�N�^�[�擾����
	std::shared_ptr<Actor> GetActor(const char* name);
private:
	// ImGui�̕`��
	// ���X�^�[�`��
	void DrawLister();

	// �ڍו`��
	void DrawDetail();

private:
	//�V���h�[�}�b�v�T�C�Y
	const DirectX::XMFLOAT2				 shadow_size[3]{ { 1024,1024 },{ 512,512 },{ 256,256 } };
	//�V���h�[�N���b�v�G���A
	const float							 shadow_area[3] = { 80, 150, 300 };
	//���C�g�r���[�v���W�F�N�V�����N���b�v�s��̔z����`����
	DirectX::XMFLOAT4X4					 lvpc_matrix[3];

	// �A�N�^�[
	std::vector<std::shared_ptr<Actor>>	 start_actors;
	std::vector<std::shared_ptr<Actor>>	 update_actors;
	std::vector<std::shared_ptr<Actor>>	 remove_actors;
	std::set<std::shared_ptr<Actor>>	 selection_actors;
	// GUI�̕`��t���O
	bool								 hidden_lister = false;
	bool								 hidden_detail = false;
	// �V�F�[�_�[
	ShaderManager::ShaderType			 shader_type = (ShaderManager::ShaderType)-1;
	std::unique_ptr<GaussianBlur>		 bulr;

	// �e�N�X�`��
	std::unique_ptr<Texture>			 shadow_texture[3];
	std::unique_ptr<Texture>			 shadow_vsm_texture[3];
	std::unique_ptr<Texture>			 depth_texture[3];
};