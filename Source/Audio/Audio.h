#pragma once
#include <xaudio2.h>
#include "AudioSource.h"

class Audio
{
public:
	Audio();
	~Audio();

public:
	// インスタンス取得
	static Audio& Instance()
	{
		static Audio instance;
		return instance;
	}

	// オーディオ読み込み
	std::shared_ptr<AudioSource> LoadAudioSource(const char* filename, bool loop);
private:
	IXAudio2* xaudio = nullptr;
	IXAudio2MasteringVoice* mastering_voice = nullptr;
};