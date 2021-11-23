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
	Effect(const char* filename);
	// デストラクタ
	~Effect();

	// 再生処理
	Effekseer::Handle Play(const DirectX::XMFLOAT3& position, float scalse = 1.0f);

	// 停止処理
	void Stop(Effekseer::Handle handle);

	// 座標設定
	void SetPosition(Effekseer::Handle handle, const DirectX::XMFLOAT3& position);

	// スケール設定
	void SetScale(Effekseer::Handle handle, const DirectX::XMFLOAT3& scale);

private:
	Effekseer::Effect* effekseer_effect = nullptr;
};