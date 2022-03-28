#include "Mouse.h"

static const int keyMap[] = {
	VK_LBUTTON,		// 左ボタン
	VK_MBUTTON,		// 中ボタン
	VK_RBUTTON,		// 右ボタン
};

//------------------------------
// コンストラクタ
//------------------------------
Mouse::Mouse(HWND hwnd)
{
	RECT rect;
	GetClientRect(hwnd, &rect);
	screen_width = rect.right - rect.left;
	screen_height = rect.bottom - rect.top;
}

//------------------------------
// 更新処理
//------------------------------
void Mouse::Update()
{
	// スイッチ情報
	MouseButton new_button_state = 0;

	for (int i = 0; i < ARRAYSIZE(keyMap); ++i)
	{
		if (::GetAsyncKeyState(keyMap[i]) & 0x8000)
		{
			new_button_state |= (1 << i);
		}
	}

	// ホイール
	wheel[1] = wheel[0];
	wheel[0] = 0;

	// ボタン情報更新
	button_state[1] = button_state[0];	// スイッチ履歴
	button_state[0] = new_button_state;

	button_down = ~button_state[1] & new_button_state;	// 押した瞬間
	button_up = ~new_button_state & button_state[1];	// 離した瞬間

	// カーソル位置の取得
	POINT cursor;
	::GetCursorPos(&cursor);
	::ScreenToClient(hwnd, &cursor);

	// 画面のサイズを取得する
	RECT rect;
	GetClientRect(hwnd, &rect);
	UINT screenW = rect.right - rect.left;
	UINT screenH = rect.bottom - rect.top;
	UINT viewportW = screen_width;
	UINT viewportH = screen_height;

	// 画面補正
	positionX[1] = positionX[0];
	positionY[1] = positionY[1];
	positionX[0] = (LONG)(cursor.x / static_cast<float>(viewportW) * static_cast<float>(screenW));
	positionY[0] = (LONG)(cursor.y / static_cast<float>(viewportH) * static_cast<float>(screenH));
}
