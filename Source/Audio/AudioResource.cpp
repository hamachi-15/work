#include "Misc.h"
#include "AudioResource.h"

// WAVE�^�O�쐬�}�N��
#define MAKE_WAVE_TAG_VALUE(c1, c2, c3, c4) (c1 | (c2<<8) | (c3<<16) | (c4<<24))

AudioResource::AudioResource(const char* filename)
{
	// wave�t�@�C���ǂݍ���
	{
		FILE* fp = nullptr;
		errno_t error = fopen_s(&fp, filename, "rb");
		_ASSERT_EXPR_A(error == 0, "WAV File not found");

		// �t�@�C���̃T�C�Y�����߂�
		fseek(fp, 0, SEEK_END);
		size_t size = static_cast<size_t>(ftell(fp));
		fseek(fp, 0, SEEK_SET);

		size_t read_bytes = 0;

		// RIFF�w�b�_
		fread(&riff, sizeof(riff), 1, fp);
		read_bytes += sizeof(riff);

		// "RIFF" �Ƃ̈�v���m�F
		_ASSERT_EXPR_A(riff.tag == MAKE_WAVE_TAG_VALUE('R', 'I', 'F', 'F'), "not in RIFF format");

		// "WAVE" �Ƃ̈�v���m�F
		_ASSERT_EXPR_A(riff.type == MAKE_WAVE_TAG_VALUE('W', 'A', 'V', 'E'), "not in WAVE format");
		while (size > read_bytes)
		{
			Chunk chunk;
			fread(&chunk, sizeof(chunk), 1, fp);
			read_bytes += sizeof(chunk);

			// 'fmt '
			if (chunk.tag == MAKE_WAVE_TAG_VALUE('f', 'm', 't', ' '))
			{
				fread(&fmt, sizeof(fmt), 1, fp);
				read_bytes += sizeof(fmt);

				// �g���̈悪����Γǂݎ̂�
				if (chunk.size > sizeof(Fmt))
				{
					UINT16 extSize;
					fread(&extSize, sizeof(extSize), 1, fp);
					read_bytes += sizeof(extSize);

					if (read_bytes + chunk.size == size)
					{
						break;
					}

					fseek(fp, extSize, SEEK_CUR);
					read_bytes += extSize;
				}
			}
			// 'data'
			else if (chunk.tag == MAKE_WAVE_TAG_VALUE('d', 'a', 't', 'a'))
			{
				data.resize(chunk.size);
				fread(data.data(), chunk.size, 1, fp);
				read_bytes += chunk.size;

				// 8-bit wav �t�@�C���̏ꍇ�� unsigned -> signed �̕ϊ����K�v
				if (fmt.quantum_bits == 8)
				{
					for (UINT32 i = 0; i < chunk.size; ++i)
					{
						data[i] -= 128;
					}
				}
			}
			// ����ȊO
			else
			{
				if (read_bytes + chunk.size == size) break;

				// �ǂݔ�΂�
				fseek(fp, chunk.size, SEEK_CUR);
				read_bytes += chunk.size;
			}
		}

		fclose(fp);
	}

	// WAV �t�H�[�}�b�g���Z�b�g�A�b�v
	{
		wfx.wFormatTag = WAVE_FORMAT_PCM;
		wfx.nChannels = 2;
		wfx.nSamplesPerSec = 44100;
		wfx.wBitsPerSample = 16;
		wfx.nBlockAlign = (wfx.wBitsPerSample >> 3) * wfx.nChannels;
		wfx.nAvgBytesPerSec = wfx.nBlockAlign * wfx.nSamplesPerSec;
		wfx.cbSize = sizeof(WAVEFORMATEX);
	}
}

AudioResource::~AudioResource()
{
}
