#pragma once
#include <DirectXMath.h>
#include <Effekseer.h>


//-------------------------------
// エフェクト
//--------------------------------
class Effect
{
public:
	// コンストラクタ
	Effect(const char* filename, Effekseer::Manager* effect_manager);

	// デストラクタ
	~Effect();

	// 再生処理
	Effekseer::Handle Play(Effekseer::Manager* effect_manager, const DirectX::XMFLOAT3& position, float scalse = 1.0f);

	// 停止処理
	void Stop(Effekseer::Manager* effect_manager, Effekseer::Handle handle);

	// 座標設定
	void SetPosition(Effekseer::Manager* effect_manager, Effekseer::Handle handle, const DirectX::XMFLOAT3& position);

	// スケール設定
	void SetScale(Effekseer::Manager* effect_manager, Effekseer::Handle handle, const DirectX::XMFLOAT3& scale);

	// 座標加算
	void AddPosition(Effekseer::Manager* effect_manager, Effekseer::Handle handle, const DirectX::XMFLOAT3& add);

	// 座標取得
	DirectX::XMFLOAT3 GetPosition(Effekseer::Manager* effect_manager, Effekseer::Handle handle) const;
private:
	Effekseer::Effect* effekseer_effect = nullptr;
};