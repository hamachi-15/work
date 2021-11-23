#pragma once
#include <xaudio2.h>
#include "AudioSource.h"

class Audio
{
public:
	Audio();
	~Audio();

public:
	// �C���X�^���X�擾
	static Audio& Instance()
	{
		static Audio instance;
		return instance;
	}

	// �I�[�f�B�I�ǂݍ���
	std::unique_ptr<AudioSource> LoadAudioSource(const char* filename, bool loop);
private:
	IXAudio2* xaudio = nullptr;
	IXAudio2MasteringVoice* mastering_voice = nullptr;
};