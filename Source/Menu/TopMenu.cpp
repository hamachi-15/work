#include "TopMenu.h"
#include "Input.h"
#include "MessageData.h"
#include "Messenger.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "SceneLoading.h"
enum	Command
{
	TitleBackDictionary,
};
static	const	char* command_name[] =
{
	"タイトルに戻る"
};

//-----------------------------------------------
// コンストラクタ
//-----------------------------------------------
TopMenu::TopMenu()
{
	button_list = std::make_unique<ButtonList>();

	button_list->SetPosition({ 50, 50 });
	button_list->SetSize({ 250, 50 });
	for (auto& it : command_name)
	{
		button_list->Register(it);
	}
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void TopMenu::Update(float elapsed_time)
{
	GamePad& gamepad = Input::Instance().GetGamePad();
	// メニューを閉じる
	if (gamepad.GetButtonDown() & GamePad::BTN_B)
	{
		MessageData::MenuCloseData data;
		Messenger::Instance().SendData(MessageData::MENUCLOSEEVENT, &data);
		return;
	}

	button_list->Update(elapsed_time);

	if (gamepad.GetButtonDown() & GamePad::BTN_A)
	{
		if (button_list->GetSelectedButton() == command_name[Command::TitleBackDictionary])
		{
			// 直接タイトルへ戻る
			MessageData::MenuCloseData data;
			Messenger::Instance().SendData(MessageData::MENUCLOSEEVENT, &data);
			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle()));
		}
	}
}
// 描画処理
void TopMenu::Render(ID3D11DeviceContext* context)
{
	button_list->Render(context);
}
