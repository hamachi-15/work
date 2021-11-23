#pragma once
#include <string>
#include <memory>
#include <set>
#include <vector>
#include "RenderContext.h"

// �O���錾
class VarianceShadowMap;
class PhongVarianceShadowMap;
class GaussianBlur;
class Texture;
class Sprite;
class Actor;
class Shader;

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

	// �L���[�u�}�b�v�`��
	void RenderCubeMap(RenderContext& render_context);
	
	// �`�揈��
	void Render(RenderContext& render_context);

	// �V���h�E�}�b�v�擾
	Texture* GetShadowTexture() const { return shadow_texture.get(); }
	Texture* GetShadowBulrTexture() const { return shadow_vsm_texture.get(); }
	Texture* GetDepthTexture() const { return depth_texture.get(); }
	// �L���[�u�}�b�v�擾
	Texture* GetCubeMapTexture() const { return cubemap_texture.get(); }

	// �A�N�^�[�擾����
	std::shared_ptr<Actor> GetActor(const char* name);
private:
	// ImGui�̕`��
	// ���X�^�[�`��
	void DrawLister();

	// �ڍו`��
	void DrawDetail();

private:
	enum TextureFile
	{
		BUFFER_WIDTH = 1024,
		BUFFER_HEIGHT = 1024,
		CUBE_MAP_WIDTH = 1024,
		CUBE_MAP_HEIGHT = 1024,
	};
	std::vector<std::shared_ptr<Actor>>	 start_actors;
	std::vector<std::shared_ptr<Actor>>	 update_actors;
	std::vector<std::shared_ptr<Actor>>	 remove_actors;
	std::set<std::shared_ptr<Actor>>	 selection_actors;
	std::vector<std::shared_ptr<Shader>> phong_shadewmap;
	bool								 hidden_lister = false;
	bool								 hidden_detail = false;
	std::string							 shader_name;
	//Shader*								 shader = nullptr;
	std::unique_ptr<PhongVarianceShadowMap>				 shader;
	std::unique_ptr<VarianceShadowMap>   shadowmap;
	std::unique_ptr<GaussianBlur>		 bulr;

	std::unique_ptr<Texture>			 shadow_texture;
	std::unique_ptr<Texture>			 shadow_vsm_texture;
	std::unique_ptr<Texture>			 depth_texture;

	std::unique_ptr<Texture>			 cubemap_texture;
	std::unique_ptr<Texture>			 depth_cubemap_texture;

};