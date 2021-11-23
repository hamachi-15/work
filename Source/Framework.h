#pragma once
#include <Windows.h>
#include "Graphics.h"
#include "HighResolutionTimer.h"
#include "Input.h"

class Framework
{
public:
	// �R���X�g���N�^
	Framework(HWND hwnd);
	
	// �f�X�g���N�^
	~Framework();

private:
	// �X�V����
	void Update(float elapsed_time);

	// �`�揈��
	void Render(float elapsed_time);

	void CalculateFrameStats();
public:
	// ���b�Z�[�W���[�v
	int Run();
	LRESULT CALLBACK HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
private:
	const HWND				hwnd;
	HighResolutionTimer		timer;
	Graphics				graphics;
	Input					input;
};