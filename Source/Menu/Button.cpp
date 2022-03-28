#include "Button.h"
#include "Input.h"
#include "SpriteLayout.h"

Button::Button(const std::string& name)
{
	sprite = std::make_unique<Sprite>("Data/Sprite/Button.png");
	this->name = name;
}

void Button::Update(float elapsed_time)
{
}

void Button::Render(ID3D11DeviceContext* context, Font* font)
{
	DirectX::XMFLOAT2 half_size = { size.x * 0.5f, size.y * 0.5f };
	DirectX::XMFLOAT2 center = { position.x + half_size.x, position.y + half_size.y };
	SpriteLayout::DisplayInfo display_info = { sprite.get(), center, half_size, {0.25f, 0.25f}, 0.3333f };
	SpriteLayout::Draw(context, display_info, { 1, 1, 1, 1 });

	//	文字列表示
	font->Draw(position.x + 10, position.y + 10, name);
}

ButtonCursol::ButtonCursol()
{
	sprite = std::make_unique<Sprite>("Data/Sprite/ButtonCursol.png");
}

ButtonCursol::~ButtonCursol()
{
}

void ButtonCursol::Update(float elapsed_time)
{
	alpha_timer += elapsed_time;
	alpha_timer = fmodf(alpha_timer, alpha_timer_max * 2);

	position.x += (target_pos.x - position.x) * 0.5f;
	position.y += (target_pos.y - position.y) * 0.5f;
	size.x += (target_size.x - size.x) * 0.5f;
	size.y += (target_size.y - size.y) * 0.5f;
}

void ButtonCursol::Render(ID3D11DeviceContext* dc, Font* font)
{
	DirectX::XMFLOAT2 half_size = { size.x * 0.5f, size.y * 0.5f };
	DirectX::XMFLOAT2 center = { position.x + half_size.x, position.y + half_size.y };
	SpriteLayout::DisplayInfo display_info = { sprite.get(), center, half_size, {0.25f, 0.25f}, 0.3333f };
	SpriteLayout::Draw(dc, display_info, { 1, 1, 1, fabsf((alpha_timer - alpha_timer_max) / alpha_timer_max) });
}

void ButtonCursol::SetTarget(DirectX::XMFLOAT2 target_pos, DirectX::XMFLOAT2 target_size)
{
	this->target_pos = target_pos;
	this->target_size = target_size;
}

ButtonList::ButtonList()
{
	font = std::make_unique<Font>("Data/Font/MS Gothic.fnt", 1024 * 1);
	button_cursol = std::make_unique<ButtonCursol>();
}

ButtonList::~ButtonList()
{
	for (Button* button : buttons)
	{
		delete	button;
	}
	buttons.clear();
}

void ButtonList::Update(float elapsed_time)
{
	// ボタン移動
	GamePad& gamepad = Input::Instance().GetGamePad();
	if (direction == Direction::Vertical)
	{
		if (gamepad.GetButtonDown() & GamePad::BTN_LEFT)
			button_id++;
		if (gamepad.GetButtonDown() & GamePad::BTN_RIGHT)
			button_id++;
	}

	if (button_id < 0)
	{
		button_id = static_cast<int>(buttons.size() - 1);
		start_display_id = static_cast<int>(buttons.size() - max_display_count);
		if (start_display_id <= 0)
			start_display_id = 0;
	}

	if (button_id >= buttons.size())
	{
		button_id = 0;
		start_display_id = 0;
	}
	if (button_id < start_display_id)
	{
		start_display_id = button_id;
	}
	if (button_id >= start_display_id + max_display_count)
	{
		start_display_id = button_id + 1 - max_display_count;
	}
	ReplaceButtons(true);

	//	ボタンリスト更新
	for (Button* button : buttons)
	{
		button->Update(elapsed_time);
	}
	button_cursol->Update(elapsed_time);

}

void ButtonList::Render(ID3D11DeviceContext* context)
{
	font->Begin(context);

	for (int i = start_display_id; i - start_display_id < max_display_count && i < buttons.size(); ++i)
	{
		buttons[i]->Render(context, font.get());
	}

	button_cursol->Render(context, font.get());

	font->End(context);
}

bool ButtonList::Register(const std::string& name)
{
	for (auto& it : buttons)
	{
		if (it->GetName() == name)
			return false;
	}
	buttons.push_back(new Button(name));
	ReplaceButtons();
	return true;
}

bool ButtonList::Unregister(const std::string& name)
{
	auto it = buttons.begin();
	auto itE = buttons.end();
	while (it != itE)
	{
		if ((*it)->GetName() == name)
		{
			buttons.erase(it);
			ReplaceButtons();
			return true;
		}
	}
	return false;
}

void ButtonList::ReplaceButtons(bool target)
{
	DirectX::XMFLOAT2	position = this->position;
	for (int i = start_display_id; i - start_display_id < max_display_count && i < buttons.size(); ++i)
	{
		Button* button = buttons[i];
		button->SetPosition(position);
		button->SetSize(size);
		if (direction == Direction::Vertical)
			position.y += size.y + space.y;
		else
			position.x += size.x + space.x;
	}

	if (!buttons.empty())
	{
		Button* button = buttons[button_id];
		if (target)
		{
			button_cursol->SetTarget(button->GetPosition(), button->GetSize());
		}
		else
		{
			button_cursol->SetPosition(button->GetPosition());
			button_cursol->SetSize(button->GetSize());
		}
	}
}
