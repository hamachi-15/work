#include "Misc.h"
#include "Audio.h"
// TODO オーディオ作成する
Audio::Audio()
{
	HRESULT hr = S_OK;
	// COMの初期化
	hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	
	UINT32 create_flags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	//createFlags |= XAUDIO2_DEBUG_ENGINE;
#endif
	// XAudio初期化
	hr = XAudio2Create(&xaudio, create_flags);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// マスタリングボイス生成
	hr = xaudio->CreateMasteringVoice(&mastering_voice);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

}

Audio::~Audio()
{
	// マスタリングボイス破棄
	if (mastering_voice != nullptr)
	{
		mastering_voice->DestroyVoice();
		mastering_voice = nullptr;
	}
	// XAudio終了化
	if (xaudio != nullptr)
	{
		xaudio->Release();
		xaudio = nullptr;
	}
	// COM終了化
	CoUninitialize();
}
//-------------------------------
// オーディオ読み込み
//-------------------------------
std::unique_ptr<AudioSource> Audio::LoadAudioSource(const char* filename, bool loop)
{
	std::shared_ptr<AudioResource> resource = std::make_shared<AudioResource>(filename);

	return std::make_unique<AudioSource>(xaudio, resource, loop);
}
