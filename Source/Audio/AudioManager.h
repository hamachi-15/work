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

// �O���錾
class AudioSource;

//***************************************
// 
// �T�E���h�}�l�[�W���[
// 
//***************************************
class AudioManager
{
private:
	AudioManager() {}
	~AudioManager() {}
public:
	// �C���X�^���X�擾
	static AudioManager& Instance()
	{
		static AudioManager instance;
		return instance;
	}

	// SE�ǉ�
	void RegisterSoundEffect(const char* filename, bool roop);
	
	// BGM�ǉ�
	void RegisterBGM(const char* filename, bool roop);

	// SE�Đ�
	void PlaySoundEffect(SEType type);
	
	// BGM�Đ�
	void PlayBGM(BGMType type);

	// SE�Đ���~
	void StopSoundEffect(SEType type);

	// BGM�Đ���~
	void StopBGM(BGMType type);

	// SE�̃{�����[���ݒ�
	void SetSoundEffectVolume(SEType type, float volume);

	// BGM�̃{�����[���ݒ�
	void SetBGMVolume(BGMType type, float volume);

	// �j������
	void Destroy();
private:
	std::vector<std::shared_ptr<AudioSource>> sound_effect;// SE
	std::vector<std::shared_ptr<AudioSource>> bgm;// BGM
};