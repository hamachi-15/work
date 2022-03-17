#pragma once
#include <vector>
#include <map>
#include "AudioSource.h"

enum class SEType
{
	SwordHit,
	PlayerGetHit,
	PlayerWalk,
	DragonScream,
};

enum class BGMType
{
	Title,
	WorldMap,
	ButtleMap,
	Clear,
	Over
};

// 前方宣言
class AudioSource;

//***************************************
// 
// サウンドマネージャー
// 
//***************************************
class AudioManager
{
private:
	AudioManager() {}
	~AudioManager() {}
public:
	// インスタンス取得
	static AudioManager& Instance()
	{
		static AudioManager instance;
		return instance;
	}

	// SE追加
	void RegisterSoundEffect(const char* filename, bool roop);
	
	// BGM追加
	void RegisterBGM(const char* filename, bool roop);

	// SE再生
	void PlaySoundEffect(SEType type);
	
	// BGM再生
	void PlayBGM(BGMType type);

	// SE再生停止
	void StopSoundEffect(SEType type);

	// BGM再生停止
	void StopBGM(BGMType type);

	// SEのボリューム設定
	void SetSoundEffectVolume(SEType type, float volume);

	// BGMのボリューム設定
	void SetBGMVolume(BGMType type, float volume);

	// 破棄処理
	void Destroy();
private:
	std::vector<std::shared_ptr<AudioSource>> sound_effect;// SE
	std::vector<std::shared_ptr<AudioSource>> bgm;// BGM
};