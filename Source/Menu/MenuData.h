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
	// メニューオープン情報
	static const char* MENUOPENEVENT = "MenuOpen";
	struct MenuOpenData
	{
		MenuId	menuId;
	};
	// メニュークローズ情報
	static	const	char* MENUCLOSEEVENT = "MENU CLOSE";
	struct MenuCloseata
	{
		int	dummy;
	};

};