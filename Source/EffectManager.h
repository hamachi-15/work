#pragma once
#include <DirectXMath.h>
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>

//------------------------------------
// エフェクトマネージャー
//------------------------------------
class EffectManager
{
private:
	// コンストラクタ
	EffectManager() {}
	// デストラクタ
	~EffectManager() {}
public:
	// インスタンス取得
	static EffectManager& Instance()
	{
		static EffectManager instance;
		return instance;
	}

	// 初期化処理
	void Initialize();

	// 終了化処理
	void Finalize();

	// 更新処理
	void Update(float elapsed_time);

	// 描画処理
	void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

	// Effekseerマネージャーの取得
	Effekseer::Manager* GetEffekseerManager() { return effekseer_manager; }

private:
	Effekseer::Manager*				effekseer_manager = nullptr;
	EffekseerRenderer::Renderer*	effekseer_renderer = nullptr;
};