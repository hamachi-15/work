#pragma once

#include "GamePad.h"
#include "Mouse.h"

class Input
{
public:
	// �R���X�g���N�^
	Input(HWND hwnd);
	
	// �f�X�g���N�^
	~Input() {}

public:
	// �C���X�^���X�擾
	static Input& Instance() { return *instance; }

	// �X�V����
	void Update();

	// �Q�[���p�b�h�擾
	GamePad& GetGamePad() { return gamepad; }

	// �}�E�X�擾
	Mouse& GetMouse() { return mouse; }

private:
	static Input*		instance;
	GamePad				gamepad;
	Mouse				mouse;
};