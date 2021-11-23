#include "Misc.h"
#include "AudioSource.h"

//-------------------------------
// コンストラクタ
//-------------------------------
AudioSource::AudioSource(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource, bool loop)
	: resource(resource), loop(loop)
{
	HRESULT hr;

	// ソースボイスを生成
	hr = xaudio->CreateSourceVoice(&source_voice, &resource->GetWaveFormat());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// ソースボイスにデータを送信
	XAUDIO2_BUFFER buffer = { 0 };
	buffer.AudioBytes = resource->GetAudioBytes();
	buffer.pAudioData = resource->GetAudioData();
	buffer.LoopCount = loop ? XAUDIO2_LOOP_INFINITE : 0;
	buffer.Flags = XAUDIO2_END_OF_STREAM;

	source_voice->SubmitSourceBuffer(&buffer);
}

//-------------------------------
// デストラクタ
//-------------------------------
AudioSource::~AudioSource()
{
	if (source_voice != nullptr)
	{
		source_voice->DestroyVoice();
		source_voice = nullptr;
	}
}

//-------------------------------
// 再生処理
//-------------------------------
void AudioSource::Play()
{
	// 再生中なら停止して
	source_voice->Stop();
	// 今までの再生バッファを削除
	source_voice->FlushSourceBuffers();

	// ソースボイスにデータを送信
	XAUDIO2_BUFFER buffer = { 0 };
	buffer.PlayBegin = 0;
	buffer.AudioBytes = resource->GetAudioBytes();
	buffer.pAudioData = resource->GetAudioData();
	buffer.LoopCount = loop ? XAUDIO2_LOOP_INFINITE : 0;
	buffer.Flags = XAUDIO2_END_OF_STREAM;

	source_voice->SubmitSourceBuffer(&buffer);
	HRESULT hr = source_voice->Start();
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	source_voice->SetVolume(1.0f);
}

//-------------------------------
// 停止処理
//-------------------------------
void AudioSource::Stop()
{
	source_voice->Stop();
}

//-------------------------------
// ボリューム設定
//-------------------------------
void AudioSource::SetVolume(float volume)
{
	source_voice->SetVolume(volume);
}
