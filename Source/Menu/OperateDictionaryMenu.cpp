#include "OperateDictionaryMenu.h"
#include "Graphics.h"
#include "Input.h"
#include "MessageData.h"
#include "Messenger.h"

//---------------------------
// コンストラクタ
//---------------------------
OperateDictionaryMenu::OperateDictionaryMenu()
{
	ID3D11Device* device = Graphics::Instance().GetDevice();
	
	shader = std::make_unique<SpriteShader>(device);
	sprite = std::make_unique<Sprite>();

	texture = std::make_unique<Texture>();
	texture->Load("Data\\Sprite\\MessageWindow.png");
}

//---------------------------
// 更新処理
//---------------------------
void OperateDictionaryMenu::Update(float elapsed_time)
{
	GamePad& gamepad = Input::Instance().GetGamePad();

	if (gamepad.GetButtonDown() & GamePad::BTN_B)
	{
		MessageData::MenuCloseData data;
		Messenger::Instance().SendData(MessageData::MENUCLOSEEVENT, &data);
		return;
	}
}

//---------------------------
// 描画処理
//---------------------------
void OperateDictionaryMenu::Render(ID3D11DeviceContext* context)
{
	float screen_width = Graphics::Instance().GetScreenWidth();
	float screen_Height = Graphics::Instance().GetScreenHeight();
	
	float texture_width = (float)texture->GetWidth();
	float texture_height = (float)texture->GetHeight();
	DirectX::XMFLOAT2 screen_center = { screen_width * 0.5f, screen_Height * 0.5f };
	DirectX::XMFLOAT2 texture_center = { texture->GetWidth() * 0.5f, texture->GetHeight() * 0.5f };
	DirectX::XMFLOAT2 draw_pos = { screen_center.x - texture_center.x, screen_center.y - texture_center.y };
	shader->Begin(context);
	sprite->Render(context,
		texture.get(),
		draw_pos.x, draw_pos.y,
		texture_width, texture_height,
		0, 0,
		texture_width, texture_height);
	shader->End(context);
}
