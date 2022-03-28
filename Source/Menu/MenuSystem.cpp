#include "MenuSystem.h"

#include "MenuData.h"
#include "Messenger.h"
#include "TopMenu.h"
#include "OperateDictionaryMenu.h"

// �R���X�g���N�^
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
// �X�V����
void MenuSystem::Update(float elapsed_time)
{
	if (menu_close_flag)
	{
		menu_close_flag = false;
		// ���j���[�����
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
// �`�揈��
void MenuSystem::Render(ID3D11DeviceContext* context)
{
	if (menu_stack.empty())
		return;

	menu_stack.top()->Render(context);
}
// ���j���[���J������
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
// ���j���[����鏈��
void MenuSystem::MenuClose(void* data)
{
	menu_close_flag = true;
}
