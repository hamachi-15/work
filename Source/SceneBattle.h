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
// バトルシーンクラス
// 
//-------------------------------------
class SceneBattle : public Scene
{
public:
	// コンストラクタ
	SceneBattle();

	// デストラクタ
	~SceneBattle() override;

	// シーン名取得
	std::string GetName() const override { return "SceneBattle"; }

	// 初期化処理
	void Initialize() override;

	// 終了化処理
	void Finalize() override;

	// 更新処理
	void Update(float elapsed_time) override;

	// 描画処理
	void Render() override;

	// スクリーンテクスチャ描画
	void ScreenRender(ID3D11DeviceContext* context, RenderContext* render_context, const DirectX::XMFLOAT2& screen_size);

	// ポストテクスチャ描画
	void PostRender(ID3D11DeviceContext* context, RenderContext* render_context, const DirectX::XMFLOAT2& screen_size);

	// ゲームオーバー・クリアの瞬間を描画
	void MomentRender(ID3D11DeviceContext* context, const DirectX::XMFLOAT2& screen_size);

	//バックバッファ描画
	void BuckBufferRender(ID3D11DeviceContext* context, RenderContext* render_context, const DirectX::XMFLOAT2& screen_size);

	// メッセージ処理
	bool OnMessages(const Telegram& telegram) override;
	
	// GUI描画
	void OnGui();

	// ゲームクリア判定処理
	bool IsGameClearJudgment();
private:
	std::unique_ptr<RenderContext>						render_context;
	std::unique_ptr<BlurRenderContext>					blur_render_context;
	std::unique_ptr<PrimitiveContext>					primitive_context;

	std::unique_ptr<Sprite>				sprite;
	std::shared_ptr<Texture>			sky;
	std::shared_ptr<Texture>			anybutton_texture;
	Texture*							bloom_texture;

	//ライト
	DirectX::XMFLOAT3					LightDir = { 0,0,0 };

	DirectX::XMFLOAT3					shadow_color = { 0.5f, 0.5f, 0.5f };

	bool								sky_bloom_flag = false; // 空のブルームの描画フラグ
	bool								primitive_falg = false;
	bool								isshadowmap = false;
	bool								isgame_clear = false;
	bool								isgame_over = false;
	bool								isbuttle_end = false;
};