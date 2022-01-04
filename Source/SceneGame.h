#pragma once
#include <memory>
#include <DirectXMath.h>
#include "Shader.h"
#include "Scene.h"
#include "RenderContext.h"
#include "ScriptDataHeadder.h"

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

	// 初期化処理
	void Initialize() override;

	// 終了処理
	void Finalize() override;

	// 更新処理
	void Update(float elapsed_time) override;

	// 描画処理
	void Render() override;

	// スクリーンテクスチャに描画
	void ScreenRender(ID3D11DeviceContext* context, RenderContext& render_context, const DirectX::XMFLOAT2& screen_size);

	// ポストエフェクトに使うテクスチャ描画
	void PostRender(ID3D11DeviceContext* context, RenderContext& render_context, const DirectX::XMFLOAT2& screen_size);

	// バックバッファに描画
	void BuckBufferRender(ID3D11DeviceContext* context, RenderContext& render_context, const DirectX::XMFLOAT2& screen_size);

	bool OnMessages(const Telegram& telegram) override;

	// ImGui描画
	void OnGui();

	// 一定範囲内にいる敵を探してIDを配列に追加する
	void DistanceBetweenEnemyAndPoint(BattleSceneDataHeadder& data_headder, const DirectX::XMVECTOR& origin, const float& range);

private:
	RenderContext						render_context;
	BlurRenderContext					blur_render_context;
	PrimitiveContext					primitive_context;

	//std::unique_ptr<CameraController>	camera_controller;

	std::unique_ptr<Shader>				sprite_cube;
	std::unique_ptr<Bloom>				bloom;

	std::unique_ptr<Sprite>				sprite;
	std::unique_ptr<Texture>			sky;
	std::unique_ptr<Texture>			bulr_texture;
	std::unique_ptr<Texture>			depth_texture;
	Texture*							bloom_texture;

	// バトルシーンに遷移するときに使うメンバ
	std::vector<int>					search_enemy_id;			// バトルに参加する敵ID配列
	float								enemy_search_range = 30.0f; // エンカウント時の索敵範囲

	//ライト
	DirectX::XMFLOAT3					LightDir = { 0,0,0 };
	DirectX::XMFLOAT3					shadow_color = { 0.5f, 0.5f, 0.5f };
	bool								battle_flag = false;
	bool								primitive_falg = false;
	bool								isshadowmap = false;

	DirectX::XMFLOAT3 radius = DirectX::XMFLOAT3(10, 10, 10);
	DirectX::XMFLOAT3 center = DirectX::XMFLOAT3(-100, 8, -100);
	DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1, 1, 1, 1);
	bool iscull = false;
};