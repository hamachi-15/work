#include "Graphics.h"
#include "Effect.h"
#include "EffectManager.h"

//---------------------------------
// コンストラクタ
//---------------------------------
Effect::Effect(const char* filename, Effekseer::Manager* effekseer_manager)
{
	// ミューテクス
	std::lock_guard<std::mutex> lock(Graphics::Instance().GetMutex());

	// Effekseerのリソースを読み込む
	// EffekseerはUTF-16のファイルパス以外は対応していない為文字コード変換が必要
	char16_t utf16_filemane[256];
	Effekseer::ConvertUtf8ToUtf16(utf16_filemane, 256, filename);

	// Effekseerエフェクトを読み込む
	effekseer_effect = Effekseer::Effect::Create(effekseer_manager, (EFK_CHAR*)utf16_filemane);
}

//---------------------------------
// デストラクタ
//---------------------------------
Effect::~Effect()
{
	// エフェクシアーの破棄処理
	if (effekseer_effect != nullptr)
	{
		effekseer_effect->Release();
		effekseer_effect = nullptr;
	}
}

//------------------------------------
// 再生処理
//------------------------------------
Effekseer::Handle Effect::Play(Effekseer::Manager* effekseer_manager, const DirectX::XMFLOAT3& position, float scale)
{
	// エフェクト再生
	Effekseer::Vector3D vector = {position.x, position.y, position.z};
	Effekseer::Handle handle = effekseer_manager->Play(effekseer_effect, vector);
	effekseer_manager->SetScale(handle, scale, scale, scale);

	return handle;
}

//---------------------------------------
// 停止処理
//---------------------------------------
void Effect::Stop(Effekseer::Manager* effekseer_manager, Effekseer::Handle handle)
{
	// エフェクト再生停止
	effekseer_manager->StopEffect(handle);
}

//-----------------------------------------
// 座標設定
//-----------------------------------------
void Effect::SetPosition(Effekseer::Manager* effekseer_manager, Effekseer::Handle handle, const DirectX::XMFLOAT3& position)
{
	effekseer_manager->SetLocation(handle, position.x, position.y, position.z);
}

//----------------------------------------
// スケール設定
//-----------------------------------------
void Effect::SetScale(Effekseer::Manager* effekseer_manager, Effekseer::Handle handle, const DirectX::XMFLOAT3& scale)
{
	effekseer_manager->SetScale(handle, scale.x, scale.y, scale.z);
}

//-----------------------------------------
// 座標加算
//-----------------------------------------
void Effect::AddPosition(Effekseer::Manager* effekseer_manager, Effekseer::Handle handle, const DirectX::XMFLOAT3& add)
{
	Effekseer::Vector3D vec(add.x, add.y, add.z);
	effekseer_manager->AddLocation(handle, vec);
}

//-----------------------------------------
// 座標取得
//-----------------------------------------
DirectX::XMFLOAT3 Effect::GetPosition(Effekseer::Manager* effekseer_manager, Effekseer::Handle handle) const
{
	Effekseer::Vector3D vector = effekseer_manager->GetLocation(handle);
	
	return DirectX::XMFLOAT3(vector.X, vector.Y, vector.Z);
}
