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
	// �C���X�^���X�擾
	static MenuSystem& Instance()
	{
		static MenuSystem instance;
		return instance;
	}
	// �X�V����
	void Update(float elapsed_time);
	// �`�揈��
	void Render(ID3D11DeviceContext* context);

	// 
	bool IsOpened()
	{
		return !menu_stack.empty();
	}
private:
	// ���j���[���J������
	void MenuOpen(void* data);
	// ���j���[����鏈��
	void MenuClose(void* data);
private:
	std::stack<MenuBase*> menu_stack;
	bool menu_close_flag = false;
	uint64_t menu_open_index = -1;
	uint64_t menu_close_index = -1;
};