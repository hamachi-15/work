#include "AudioManager.h"
#include "Audio.h"
//***************************************
// 
// サウンドマネージャー
// 
//***************************************
//----------------------
// SE追加
//----------------------
void AudioManager::RegisterSoundEffect(const char* filename, bool roop)
{
	// 配列にSE追加
	sound_effect.emplace_back(Audio::Instance().LoadAudioSource(filename, roop));
}

//----------------------
// BGM追加
//----------------------
void AudioManager::RegisterBGM(const char* filename, bool roop)
{
	// 配列にBGM追加
	bgm.emplace_back(Audio::Instance().LoadAudioSource(filename, roop));
}

//----------------------
// SE再生
//----------------------
void AudioManager::PlaySoundEffect(SEType type)
{
	// 再生
	sound_effect[static_cast<int>(type)]->Play();
}

//----------------------
// BGM再生
//----------------------
void AudioManager::PlayBGM(BGMType type)
{
	// 再生
	bgm[static_cast<int>(type)]->Play();
}

//----------------------
// SE再生停止
//----------------------
void AudioManager::StopSoundEffect(SEType type)
{
	// 再生停止
	sound_effect[static_cast<int>(type)]->Stop();
}

//----------------------
// BGM再生停止
//----------------------
void AudioManager::StopBGM(BGMType type)
{
	// 再生停止
	bgm[static_cast<int>(type)]->Stop();
}

//----------------------
// SEのボリューム設定
//----------------------
void AudioManager::SetSoundEffectVolume(SEType type, float volume)
{
	// ボリューム設定
	sound_effect[static_cast<int>(type)]->SetVolume(volume);
}

//----------------------
// ボリューム設定
//----------------------
void AudioManager::SetBGMVolume(BGMType type, float volume)
{
	// ボリューム設定
	bgm[static_cast<int>(type)]->SetVolume(volume);
}

//----------------------
// 破棄処理
//----------------------
void AudioManager::Destroy()
{
	// SE配列の破棄処理
	std::vector <std::shared_ptr<AudioSource>>::iterator sound_effect_iterater = sound_effect.begin();
	for (; sound_effect_iterater != sound_effect.end(); sound_effect_iterater = sound_effect.begin())
	{
		sound_effect.erase(sound_effect_iterater);
	}

	// BGM配列の破棄処理
	std::vector <std::shared_ptr<AudioSource>>::iterator bgm_iterater = bgm.begin();
	for (; bgm_iterater != bgm.end(); bgm_iterater = bgm.begin())
	{
		bgm.erase(bgm_iterater);
	}
}
