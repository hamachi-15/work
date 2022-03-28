#pragma once

#include "GamePad.h"
#include "Mouse.h"

class Input
{
public:
	// コンストラクタ
	Input(HWND hwnd);
	
	// デストラクタ
	~Input() {}

public:
	// インスタンス取得
	static Input& Instance() { return *instance; }

	// 更新処理
	void Update();

	// ゲームパッド取得
	GamePad& GetGamePad() { return gamepad; }

	// マウス取得
	Mouse& GetMouse() { return mouse; }

private:
	static Input*		instance;
	GamePad				gamepad;
	Mouse				mouse;
};