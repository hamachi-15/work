#include "Misc.h"
#include "Audio.h"
// TODO �I�[�f�B�I�쐬����
Audio::Audio()
{
	HRESULT hr = S_OK;
	// COM�̏�����
	hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	
	UINT32 create_flags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	//createFlags |= XAUDIO2_DEBUG_ENGINE;
#endif
	// XAudio������
	hr = XAudio2Create(&xaudio, create_flags);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// �}�X�^�����O�{�C�X����
	hr = xaudio->CreateMasteringVoice(&mastering_voice);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

}

Audio::~Audio()
{
	// �}�X�^�����O�{�C�X�j��
	if (mastering_voice != nullptr)
	{
		mastering_voice->DestroyVoice();
		mastering_voice = nullptr;
	}
	// XAudio�I����
	if (xaudio != nullptr)
	{
		xaudio->Release();
		xaudio = nullptr;
	}
	// COM�I����
	CoUninitialize();
}
//-------------------------------
// �I�[�f�B�I�ǂݍ���
//-------------------------------
std::unique_ptr<AudioSource> Audio::LoadAudioSource(const char* filename, bool loop)
{
	std::shared_ptr<AudioResource> resource = std::make_shared<AudioResource>(filename);

	return std::make_unique<AudioSource>(xaudio, resource, loop);
}
