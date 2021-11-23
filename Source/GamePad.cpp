#include <windows.h>
#include <math.h>
#include <Xinput.h>
#include "GamePad.h"
// 更新
void GamePad::Update()
{
	axisLx = axisLy = 0.0f;
	axisRx = axisRy = 0.0f;
	triggerL = triggerR = 0.0f;

	GamePadButton new_button_state = 0;

	// ボタン情報取得
	XINPUT_STATE xinput_state;
	if (XInputGetState(slot, &xinput_state) == ERROR_SUCCESS)
	{
		XINPUT_GAMEPAD& pad = xinput_state.Gamepad;

		if (pad.wButtons & XINPUT_GAMEPAD_DPAD_UP)					new_button_state |= BTN_UP;
		if (pad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)				new_button_state |= BTN_RIGHT;
		if (pad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)				new_button_state |= BTN_DOWN;
		if (pad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)				new_button_state |= BTN_LEFT;
		if (pad.wButtons & XINPUT_GAMEPAD_A)						new_button_state |= BTN_A;
		if (pad.wButtons & XINPUT_GAMEPAD_B)						new_button_state |= BTN_B;
		if (pad.wButtons & XINPUT_GAMEPAD_X)						new_button_state |= BTN_X;
		if (pad.wButtons & XINPUT_GAMEPAD_Y)						new_button_state |= BTN_Y;
		if(pad.wButtons & XINPUT_GAMEPAD_START)						new_button_state |= BTN_START;
		if (pad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)				new_button_state |= BTN_LEFT_THUMB;
		if (pad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)				new_button_state |= BTN_RIGHT_THUMB;
		if (pad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)			new_button_state |= BTN_LEFT_SHOULDER;
		if (pad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)			new_button_state |= BTN_RIGHT_SHOULDER;
		if (pad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)	new_button_state |= BTN_LEFT_TRIGGER;
		if (pad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)	new_button_state |= BTN_RIGHT_TRIGGER;
		if ((pad.sThumbLX <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && pad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
			(pad.sThumbLY <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && pad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
		{
			pad.sThumbLX = 0;
			pad.sThumbLY = 0;
		}

		if ((pad.sThumbRX <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
			(pad.sThumbRY <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
		{
			pad.sThumbRX = 0;
			pad.sThumbRY = 0;
		}
		triggerL = static_cast<float>(pad.bLeftTrigger) / 255.0f;
		triggerR = static_cast<float>(pad.bRightTrigger) / 255.0f;
		axisLx = static_cast<float>(pad.sThumbLX) / static_cast<float>(0x8000);
		axisLy = static_cast<float>(pad.sThumbLY) / static_cast<float>(0x8000);
		axisRx = static_cast<float>(pad.sThumbRX) / static_cast<float>(0x8000);
		axisRy = static_cast<float>(pad.sThumbRY) / static_cast<float>(0x8000);
	}
	
	// キーボードでエミュレーション
	{
		float lx = 0.0f;
		float ly = 0.0f;
		float rx = 0.0f;
		float ry = 0.0f;
		if (GetAsyncKeyState('W') & 0x8000)			ly = 1.0f;
		if (GetAsyncKeyState('A') & 0x8000)			lx = -1.0f;
		if (GetAsyncKeyState('S') & 0x8000)			ly = -1.0f;
		if (GetAsyncKeyState('D') & 0x8000)			lx = 1.0f;
		if (GetAsyncKeyState('I') & 0x8000)			ry = 1.0f;
		if (GetAsyncKeyState('J') & 0x8000)			rx = -1.0f;
		if (GetAsyncKeyState('K') & 0x8000)			ry = -1.0f;
		if (GetAsyncKeyState('L') & 0x8000)			rx = 1.0f;
		if (GetAsyncKeyState('Z') & 0x8000)			new_button_state |= BTN_A;
		if (GetAsyncKeyState('X') & 0x8000)			new_button_state |= BTN_B;
		if (GetAsyncKeyState('C') & 0x8000)			new_button_state |= BTN_X;
		if (GetAsyncKeyState('V') & 0x8000)			new_button_state |= BTN_Y;
		if (GetAsyncKeyState(VK_UP) & 0x8000)		new_button_state |= BTN_UP;
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)	new_button_state |= BTN_RIGHT;
		if (GetAsyncKeyState(VK_DOWN) & 0x8000)		new_button_state |= BTN_DOWN;
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)		new_button_state |= BTN_LEFT;

#if 1
		if (new_button_state & BTN_UP)    ly = 1.0f;
		if (new_button_state & BTN_RIGHT) lx = 1.0f;
		if (new_button_state & BTN_DOWN)  ly = -1.0f;
		if (new_button_state & BTN_LEFT)  lx = -1.0f;
#endif
		if (lx >= 1.0f || lx <= -1.0f || ly >= 1.0f || ly <= -1.0f)
		{
			float power = ::sqrtf(lx * lx + ly * ly);
			axisLx = lx / power;
			axisLy = ly / power;
		}

		if (rx >= 1.0f || rx <= -1.0f || ry >= 1.0f || ry <= -1.0f)
		{
			float power = ::sqrtf(rx * rx + ry * ry);
			axisRx = rx / power;
			axisRy = ry / power;
		}
 	}
	
	// ボタン情報の更新
	{
		button_state[1] = button_state[0]; // スイッチ履歴
		button_state[0] = new_button_state;

		button_down = ~button_state[1] & new_button_state; // 押した瞬間
		button_up = ~new_button_state & button_state[1];   // 離した瞬間
	}
}
