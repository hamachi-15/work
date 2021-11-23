#include "Mouse.h"

static const int keyMap[] = {
	VK_LBUTTON,		// ���{�^��
	VK_MBUTTON,		// ���{�^��
	VK_RBUTTON,		// �E�{�^��
};

//------------------------------
// �R���X�g���N�^
//------------------------------
Mouse::Mouse(HWND hwnd)
{
	RECT rect;
	GetClientRect(hwnd, &rect);
	screen_width = rect.right - rect.left;
	screen_height = rect.bottom - rect.top;
}

//------------------------------
// �X�V����
//------------------------------
void Mouse::Update()
{
	// �X�C�b�`���
	MouseButton new_button_state = 0;

	for (int i = 0; i < ARRAYSIZE(keyMap); ++i)
	{
		if (::GetAsyncKeyState(keyMap[i]) & 0x8000)
		{
			new_button_state |= (1 << i);
		}
	}

	// �z�C�[��
	wheel[1] = wheel[0];
	wheel[0] = 0;

	// �{�^�����X�V
	button_state[1] = button_state[0];	// �X�C�b�`����
	button_state[0] = new_button_state;

	button_down = ~button_state[1] & new_button_state;	// �������u��
	button_up = ~new_button_state & button_state[1];	// �������u��

	// �J�[�\���ʒu�̎擾
	POINT cursor;
	::GetCursorPos(&cursor);
	::ScreenToClient(hwnd, &cursor);

	// ��ʂ̃T�C�Y���擾����
	RECT rect;
	GetClientRect(hwnd, &rect);
	UINT screenW = rect.right - rect.left;
	UINT screenH = rect.bottom - rect.top;
	UINT viewportW = screen_width;
	UINT viewportH = screen_height;

	// ��ʕ␳
	positionX[1] = positionX[0];
	positionY[1] = positionY[1];
	positionX[0] = (LONG)(cursor.x / static_cast<float>(viewportW) * static_cast<float>(screenW));
	positionY[0] = (LONG)(cursor.y / static_cast<float>(viewportH) * static_cast<float>(screenH));
}
