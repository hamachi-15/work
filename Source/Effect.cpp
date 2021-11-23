#include "Graphics.h"
#include "Effect.h"
#include "EffectManager.h"

//---------------------------------
// コンストラクタ
//---------------------------------
Effect::Effect(const char* filename)
{
	// ミューテクス
	std::lock_guard<std::mutex> lock(Graphics::Instance().GetMutex());

	// Effekseerのリソースを読み込む
	// EffekseerはUTF-16のファイルパス以外は対応していない為文字コード変換が必要
	char16_t utf16_filemane[256];
	Effekseer::ConvertUtf8ToUtf16(utf16_filemane, 256, filename);

	// Effekseer::Managerを取得
	Effekseer::Manager* effekseer_manager = EffectManager::Instance().GetEffekseerManager();

	// Effekseerエフェクトを読み込む
	effekseer_effect = Effekseer::Effect::Create(effekseer_manager, (EFK_CHAR*)utf16_filemane);
}

//---------------------------------
// デストラクタ
//---------------------------------
Effect::~Effect()
{
	if (effekseer_effect != nullptr)
	{
		effekseer_effect->Release();
		effekseer_effect = nullptr;
	}
}

//------------------------------------
// 再生処理
//------------------------------------
Effekseer::Handle Effect::Play(const DirectX::XMFLOAT3& position, float scale)
{
	// Effekseer::Managerを取得
	Effekseer::Manager* effekseer_manager = EffectManager::Instance().GetEffekseerManager();

	// エフェクト再生
	Effekseer::Handle handle = effekseer_manager->Play(effekseer_effect, position.x, position.y, position.z);
	effekseer_manager->SetScale(handle, scale, scale, scale);

	return handle;
}

//---------------------------------------
// 停止処理
//---------------------------------------
void Effect::Stop(Effekseer::Handle handle)
{
	// Effekseer::Managerを取得
	Effekseer::Manager* effekseer_manager = EffectManager::Instance().GetEffekseerManager();

	// エフェクト再生停止
	effekseer_manager->StopEffect(handle);
}

//-----------------------------------------
// 座標設定
//-----------------------------------------
void Effect::SetPosition(Effekseer::Handle handle, const DirectX::XMFLOAT3& position)
{
	// Effekseer::Managerを取得
	Effekseer::Manager* effekseer_manager = EffectManager::Instance().GetEffekseerManager();

	effekseer_manager->SetLocation(handle, position.x, position.y, position.z);
}

//----------------------------------------
// スケール設定
//-----------------------------------------
void Effect::SetScale(Effekseer::Handle handle, const DirectX::XMFLOAT3& scale)
{
	// Effekseer::Managerを取得
	Effekseer::Manager* effekseer_manager = EffectManager::Instance().GetEffekseerManager();

	effekseer_manager->SetScale(handle, scale.x, scale.y, scale.z);
}