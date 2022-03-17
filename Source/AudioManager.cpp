#include "AudioManager.h"
#include "Audio.h"
//***************************************
// 
// �T�E���h�}�l�[�W���[
// 
//***************************************
//----------------------
// SE�ǉ�
//----------------------
void AudioManager::RegisterSoundEffect(const char* filename, bool roop)
{
	// �z���SE�ǉ�
	sound_effect.emplace_back(Audio::Instance().LoadAudioSource(filename, roop));
}

//----------------------
// BGM�ǉ�
//----------------------
void AudioManager::RegisterBGM(const char* filename, bool roop)
{
	// �z���BGM�ǉ�
	bgm.emplace_back(Audio::Instance().LoadAudioSource(filename, roop));
}

//----------------------
// SE�Đ�
//----------------------
void AudioManager::PlaySoundEffect(SEType type)
{
	// �Đ�
	sound_effect[static_cast<int>(type)]->Play();
}

//----------------------
// BGM�Đ�
//----------------------
void AudioManager::PlayBGM(BGMType type)
{
	// �Đ�
	bgm[static_cast<int>(type)]->Play();
}

//----------------------
// SE�Đ���~
//----------------------
void AudioManager::StopSoundEffect(SEType type)
{
	// �Đ���~
	sound_effect[static_cast<int>(type)]->Stop();
}

//----------------------
// BGM�Đ���~
//----------------------
void AudioManager::StopBGM(BGMType type)
{
	// �Đ���~
	bgm[static_cast<int>(type)]->Stop();
}

//----------------------
// SE�̃{�����[���ݒ�
//----------------------
void AudioManager::SetSoundEffectVolume(SEType type, float volume)
{
	// �{�����[���ݒ�
	sound_effect[static_cast<int>(type)]->SetVolume(volume);
}

//----------------------
// �{�����[���ݒ�
//----------------------
void AudioManager::SetBGMVolume(BGMType type, float volume)
{
	// �{�����[���ݒ�
	bgm[static_cast<int>(type)]->SetVolume(volume);
}

//----------------------
// �j������
//----------------------
void AudioManager::Destroy()
{
	// SE�z��̔j������
	std::vector <std::shared_ptr<AudioSource>>::iterator sound_effect_iterater = sound_effect.begin();
	for (; sound_effect_iterater != sound_effect.end(); sound_effect_iterater = sound_effect.begin())
	{
		sound_effect.erase(sound_effect_iterater);
	}

	// BGM�z��̔j������
	std::vector <std::shared_ptr<AudioSource>>::iterator bgm_iterater = bgm.begin();
	for (; bgm_iterater != bgm.end(); bgm_iterater = bgm.begin())
	{
		bgm.erase(bgm_iterater);
	}
}
