#include "MenuSystem.h"

#include "MenuData.h"
#include "Messenger.h"
#include "TopMenu.h"
#include "OperateDictionaryMenu.h"

// コンストラクタ
MenuSystem::MenuSystem()
{
	menu_open_index = Messenger::Instance().AddReceiver(MessageData::MENUOPENEVENT, [&](void* data) { MenuOpen(data); });
	menu_close_index = Messenger::Instance().AddReceiver(MessageData::MENUCLOSEEVENT, [&](void* data) { MenuClose(data); });
}

MenuSystem::~MenuSystem()
{
	Messenger::Instance().RemoveReceiver(menu_open_index);
	Messenger::Instance().RemoveReceiver(menu_close_index);
	while (!menu_stack.empty())
	{
		delete menu_stack.top();
		menu_stack.pop();
	}
}
// 更新処理
void MenuSystem::Update(float elapsed_time)
{
	if (menu_close_flag)
	{
		menu_close_flag = false;
		// メニューを閉じる
		if (!menu_stack.empty())
		{
			delete menu_stack.top();
			menu_stack.pop();
		}
	}

	if (menu_stack.empty())
		return;

	menu_stack.top()->Update(elapsed_time);
}
// 描画処理
void MenuSystem::Render(ID3D11DeviceContext* context)
{
	if (menu_stack.empty())
		return;

	menu_stack.top()->Render(context);
}
// メニューを開く処理
void MenuSystem::MenuOpen(void* data)
{
	MessageData::MenuOpenData* open_data = static_cast<MessageData::MenuOpenData*>(data);
	switch (open_data->menuId)
	{
	case MenuId::TopMenu:
	{
		menu_stack.push(new TopMenu());
		break;
	}
	case MenuId::OperateMenu:
	{
		menu_stack.push(new OperateDictionaryMenu());
		break;
	}
	}
}
// メニューを閉じる処理
void MenuSystem::MenuClose(void* data)
{
	menu_close_flag = true;
}
