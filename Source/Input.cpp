#include "Input.h"

Input* Input::instance = nullptr;

//-------------------------------
// �R���X�g���N�^
//-------------------------------
Input::Input(HWND hwnd) : mouse(hwnd)
{
	// �C���X�^���X�̐ݒ�
	instance = this;
}


//-------------------------------
// �X�V����
//-------------------------------
void Input::Update()
{
	gamepad.Update();
	mouse.Update();
}
