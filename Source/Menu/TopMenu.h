#pragma once
#include <memory>
#include "MenuBase.h"
#include "Button.h"

class TopMenu : public MenuBase
{
public:
	TopMenu();
	~TopMenu()override{}

	void Update(float elapsed_time)override;
	void Render(ID3D11DeviceContext* context)override;
private:
	std::unique_ptr<ButtonList> button_list;
};