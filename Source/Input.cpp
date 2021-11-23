#include "Input.h"

Input* Input::instance = nullptr;

//-------------------------------
// コンストラクタ
//-------------------------------
Input::Input(HWND hwnd) : mouse(hwnd)
{
	// インスタンスの設定
	instance = this;
}


//-------------------------------
// 更新処理
//-------------------------------
void Input::Update()
{
	gamepad.Update();
	mouse.Update();
}
