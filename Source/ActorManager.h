#pragma once
#include <string>
#include <memory>
#include <set>
#include <vector>
#include "RenderContext.h"

// 前方宣言
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
	// コンストラクタ・デストラクタ
	ActorManager();
	~ActorManager();
public:
	// インスタンス作成
	static ActorManager& Instance()
	{
		static ActorManager instance;
		return instance;
	}

	// 作成処理
	std::shared_ptr<Actor>Create();

	// 破棄配列に追加
	void Remove(std::shared_ptr<Actor> actor);

	// 破棄処理
	void Destroy(std::shared_ptr<Actor> actor);

	// 全破棄処理
	void AllDestroy();

	// 更新処理
	void Update(float elapsed_time);

	// 行列更新処理
	void UpdateTransform();

	// シャドウマップ描画処理
	void ShadowRender(RenderContext& render_context, BlurRenderContext& bulr_render_context);

	// キューブマップ描画
	void RenderCubeMap(RenderContext& render_context);
	
	// 描画処理
	void Render(RenderContext& render_context);

	// シャドウマップ取得
	Texture* GetShadowTexture() const { return shadow_texture.get(); }
	Texture* GetShadowBulrTexture() const { return shadow_vsm_texture.get(); }
	Texture* GetDepthTexture() const { return depth_texture.get(); }
	// キューブマップ取得
	Texture* GetCubeMapTexture() const { return cubemap_texture.get(); }

	// アクター取得処理
	std::shared_ptr<Actor> GetActor(const char* name);
private:
	// ImGuiの描画
	// リスター描画
	void DrawLister();

	// 詳細描画
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