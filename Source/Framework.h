#pragma once
#include <Windows.h>
#include "Graphics.h"
#include "HighResolutionTimer.h"
#include "Input.h"

class Framework
{
public:
	// コンストラクタ
	Framework(HWND hwnd);
	
	// デストラクタ
	~Framework();

private:
	// 更新処理
	void Update(float elapsed_time);

	// 描画処理
	void Render(float elapsed_time);

	void CalculateFrameStats();
public:
	// メッセージループ
	int Run();
	LRESULT CALLBACK HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
private:
	const HWND				hwnd;
	HighResolutionTimer		timer;
	Graphics				graphics;
	Input					input;
};