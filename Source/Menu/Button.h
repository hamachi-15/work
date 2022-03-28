#pragma once
#include	<vector>
#include	<DirectXMath.h>
#include	<string>
#include	"Sprite.h"
#include	"Font.h"
// ボタン
class Button
{
public:
	Button(const std::string& name);
	~Button() {}

	void Update(float elapsed_time);
	void Render(ID3D11DeviceContext* context, Font* font);

	const std::string& GetName() { return name; }
	const DirectX::XMFLOAT2& GetPosition() { return position; }
	void SetPosition(const DirectX::XMFLOAT2& position) { this->position = position; }
	const DirectX::XMFLOAT2& GetSize() { return size; }
	void SetSize(const DirectX::XMFLOAT2& size) { this->size = size; }
private:
	std::string			name;
	std::unique_ptr<Sprite> sprite;
	DirectX::XMFLOAT2	position = { 0, 0 };
	DirectX::XMFLOAT2	size = { 100, 100 };

};

//	ボタンカーソル
class	ButtonCursol
{
public:
	ButtonCursol();
	~ButtonCursol();

	void	Update(float elapsed_time);
	void	Render(ID3D11DeviceContext* context, Font* font);

	const	DirectX::XMFLOAT2& GetPosition() { return position; }
	void	SetPosition(const DirectX::XMFLOAT2& position) { this->position = position; }

	const	DirectX::XMFLOAT2& GetSize() { return size; }
	void	SetSize(const DirectX::XMFLOAT2& size) { this->size = size; }

	void	SetTarget(DirectX::XMFLOAT2 target_pos, DirectX::XMFLOAT2 target_size);

private:
	std::unique_ptr<Sprite> sprite = nullptr;
	DirectX::XMFLOAT2	position = { 0, 0 };
	DirectX::XMFLOAT2	size = { 100, 100 };
	DirectX::XMFLOAT2	target_pos = { 0, 0 };
	DirectX::XMFLOAT2	target_size = { 100, 100 };
	float				alpha_timer = 0;
	float				alpha_timer_max = 0.5;
};

//	ボタンリスト
class	ButtonList
{
public:
	//	リストの並び方
	enum	class	Direction
	{
		Vertical,
		Horizontal
	};

public:
	ButtonList();
	~ButtonList();

	void	Update(float elapsed_time);
	void	Render(ID3D11DeviceContext* context);

	bool	Register(const std::string& name);
	bool	Unregister(const std::string& name);

	const Direction& GetDirection() { return direction; }
	void SetDirection(const Direction& direction) { this->direction = direction; ReplaceButtons(); }

	const DirectX::XMFLOAT2& GetPosition() { return position; }
	void SetPosition(const DirectX::XMFLOAT2& position) { this->position = position; ReplaceButtons(); }

	const DirectX::XMFLOAT2& GetSize() { return size; }
	void SetSize(const DirectX::XMFLOAT2& size) { this->size = size; ReplaceButtons(); }

	const DirectX::XMFLOAT2& GetSpace() { return space; }
	void SetSpace(const DirectX::XMFLOAT2& space) { this->space = space; ReplaceButtons(); }

	const std::string& GetSelectedButton() { return buttons[button_id]->GetName(); }
	void SetSelectedButton(const std::string& name)
	{
		for (int i = 0; i < buttons.size(); ++i)
		{
			if (buttons[i]->GetName() == name)
			{
				button_id = i;
			}
		}
	}

	const int& GetMaxDisplayCount() { return max_display_count; }
	void SetMaxDisplayCount(const int& max_display_count) { this->max_display_count = max_display_count; ReplaceButtons(); }

private:
	void	ReplaceButtons(bool target = false);

private:
	std::unique_ptr<Font> font;
	std::vector<Button*>	buttons;
	std::unique_ptr<ButtonCursol> button_cursol;
	int						button_id = 0;
	int						start_display_id = 0;
	int						max_display_count = 5;
	//	ボタン一つの情報
	Direction				direction = Direction::Vertical;
	DirectX::XMFLOAT2		position;
	DirectX::XMFLOAT2		size;
	DirectX::XMFLOAT2		space = { 10, 10 };
};
