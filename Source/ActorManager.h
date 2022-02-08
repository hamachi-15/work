#pragma once
#include <string>
#include <memory>
#include <set>
#include <vector>
#include "RenderContext.h"
#include "ShaderManager.h"

// 前方宣言
class GaussianBlur;
class Texture;
class Sprite;
class Actor;

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
	
	// 描画処理
	void Render(RenderContext& render_context);
	void BrightRender(RenderContext& render_context);

	// シャドウマップ取得
	Texture* GetShadowTexture(int index) const { return shadow_texture[index].get(); }
	Texture* GetShadowBulrTexture(int index) const { return shadow_vsm_texture[index].get(); }
	Texture* GetDepthTexture(int index) const { return depth_texture[index].get(); }

	// アクター取得処理
	std::shared_ptr<Actor> GetActor(const char* name);
private:
	// ImGuiの描画
	// リスター描画
	void DrawLister();

	// 詳細描画
	void DrawDetail();

private:
	//シャドーマップサイズ
	const DirectX::XMFLOAT2				 shadow_size[3]{ { 1024,1024 },{ 512,512 },{ 256,256 } };
	//シャドークリップエリア
	const float							 shadow_area[3] = { 80, 150, 300 };
	//ライトビュープロジェクションクロップ行列の配列を定義する
	DirectX::XMFLOAT4X4					 lvpc_matrix[3];

	// アクター
	std::vector<std::shared_ptr<Actor>>	 start_actors;
	std::vector<std::shared_ptr<Actor>>	 update_actors;
	std::vector<std::shared_ptr<Actor>>	 remove_actors;
	std::set<std::shared_ptr<Actor>>	 selection_actors;
	// GUIの描画フラグ
	bool								 hidden_lister = false;
	bool								 hidden_detail = false;
	// シェーダー
	ShaderManager::ShaderType			 shader_type = (ShaderManager::ShaderType)-1;
	std::unique_ptr<GaussianBlur>		 bulr;

	// テクスチャ
	std::unique_ptr<Texture>			 shadow_texture[3];
	std::unique_ptr<Texture>			 shadow_vsm_texture[3];
	std::unique_ptr<Texture>			 depth_texture[3];
};