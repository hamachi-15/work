#pragma once
#include	"Sprite.h"
#include	"Font.h"
#include	<vector>
#include	<string>

namespace SpriteLayout
{
	// �����R�[�h�̕ϊ������n�֐�
	int	ShiftJisToUnicode(const char* shift_jis, wchar_t* unicode, uint64_t unicode_buffer_size);
	int	UnicodeToUTF8(const wchar_t* unicode, char* utf8, uint64_t utf8_buffer_size);
	int	ShiftJisToUTF8(const char* shift_jis, char* utf8, uint64_t unicode_buffer_size);
	int	UTF8ToUnicode(const char* utf8, wchar_t* unicode, uint64_t unicode_buffer_size);
	int	UnicodeToShiftJis(const wchar_t* unicode, char* shift_jis, uint64_t shift_jis_buffer_size);
	int	UTF8ToShiftJis(const char* utf8, char* shift_jis, uint64_t shift_jis_buffer_size);
	
	// �e�N�X�`���̏c����3�������ĕ\������
	// ��������������
	// ���P���Q���R��
	// ��������������
	// ���S���T���U��
	// ��������������
	// ���V���W���X��
	// ��������������
	// �y�P�E�R�E�V�E�X�z�̃T�C�Y���e�N�X�`���Ŏw�肵�����̒l�ɋK�肵�A
	// �y�Q�E�S�E�T�E�U�E�W�z�̃T�C�Y��L�k�����ĕ\��������
	struct	DisplayInfo
	{
		Sprite* sprite;
		DirectX::XMFLOAT2	center;
		DirectX::XMFLOAT2	size;
		DirectX::XMFLOAT2	corner_size_rate;
		float				uv_split_size;
	};
	// ���C�A�E�g�������ĕ\���B���̃u���b�N�̒�`�ǂ���
	void Draw(ID3D11DeviceContext* context, DisplayInfo info, DirectX::XMFLOAT4 color = { 1, 1, 1, 1 });

	// ���C�A�E�g������̕\����`���Z�o�B���́y�T�z�̋�`���Z�o����
	DirectX::XMFLOAT4 CalcDisplayRect(DisplayInfo info);

}