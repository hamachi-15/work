#pragma once
#include <DirectXMath.h>
#include <vector>
#include <string>

enum class MenuId
{
	None,
	TopMenu,
	OperateMenu
};

namespace MessageData
{
	// ���j���[�I�[�v�����
	static const char* MENUOPENEVENT = "MenuOpen";
	struct MenuOpenData
	{
		MenuId	menuId;
	};
	// ���j���[�N���[�Y���
	static	const	char* MENUCLOSEEVENT = "MENU CLOSE";
	struct MenuCloseata
	{
		int	dummy;
	};

};