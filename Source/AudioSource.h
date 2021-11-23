#pragma once
#include <memory>
#include <xaudio2.h>
#include "AudioResource.h"

class AudioSource
{
public:
	AudioSource(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource, bool loop);
	~AudioSource();

	// Ä¶ˆ—
	void Play();

	// ’â~ˆ—
	void Stop();

	// ƒ{ƒŠƒ…[ƒ€İ’è
	void SetVolume(float volume);

private:
	IXAudio2SourceVoice* source_voice = nullptr;
	std::shared_ptr<AudioResource> resource;
	bool loop;
};