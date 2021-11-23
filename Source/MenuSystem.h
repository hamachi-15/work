#pragma once

#include <stack>
#include "MenuBase.h"
#include "Button.h"
class MenuSystem
{
private:
	MenuSystem();
	~MenuSystem();

public:
	// インスタンス取得
	static MenuSystem& Instance()
	{
		static MenuSystem instance;
		return instance;
	}
	// 更新処理
	void Update(float elapsed_time);
	// 描画処理
	void Render(ID3D11DeviceContext* context);

	// 
	bool IsOpened()
	{
		return !menu_stack.empty();
	}
private:
	// メニューを開く処理
	void MenuOpen(void* data);
	// メニューを閉じる処理
	void MenuClose(void* data);
private:
	std::stack<MenuBase*> menu_stack;
	bool menu_close_flag = false;
	uint64_t menu_open_index = -1;
	uint64_t menu_close_index = -1;
};